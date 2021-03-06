
#include "GameEngineStd.h"

#include "GameEngineApp.h"


#include "../Actor/ActorComponent.h"
#include "../Actor/RenderComponent.h"
#include "../Actor/TransformComponent.h"


#include "../MainWindow/OpenGLRenderWindow.h"
#include "Geometry.h"
#include "Lights.h"
//#include "Mesh.h
//#include "Raycast.h
#include "SceneNodes.h"
#include "Shaders.h"
#include "../ResourceCache/ResCache.h"

////////////////////////////////////////////////////
// SceneNodeProperties Implementation
////////////////////////////////////////////////////

SceneNodeProperties::SceneNodeProperties(void)
{
    m_ActorId = INVALID_ACTOR_ID;
    m_Radius = 0;
    m_RenderPass = RenderPass_0;
    m_AlphaType = AlphaOpaque;
}

//
// SceneNodeProperties::Transform			- Chapter 16, page 528
//
void SceneNodeProperties::Transform(Mat4x4 *toWorld, Mat4x4 *fromWorld) const
{
    if (toWorld)
        *toWorld = m_ToWorld;

    if (fromWorld)
        *fromWorld = m_FromWorld;
}

////////////////////////////////////////////////////
// SceneNode Implementation
////////////////////////////////////////////////////

SceneNode::SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent,  RenderPass renderPass,  const Mat4x4 *to, const Mat4x4 *from)
{
    m_pParent = NULL;
    m_Props.m_ActorId = actorId;
    m_Props.m_Name = (renderComponent) ? renderComponent->VGetName() : "SceneNode";
    m_Props.m_RenderPass = renderPass;
    m_Props.m_AlphaType = AlphaOpaque;
    m_RenderComponent = renderComponent;
    VSetTransform(to, from);
    SetRadius(0);

    // these lines were moved to VOnRestore() post press
    Color color = (renderComponent) ? renderComponent->GetColor() : g_White;
    m_Props.m_Material.SetDiffuse(color);
}

SceneNode::~SceneNode()
{

}

//
// SceneNode::VOnRestore					- Chapter 16, page 532
//
bool SceneNode::VOnRestore(Scene *pScene)
{
    Color color = (m_RenderComponent) ? m_RenderComponent->GetColor() : g_White;
    m_Props.m_Material.SetDiffuse(color);

    // This is meant to be called from any class
    // that inherits from SceneNode and overloads
    // VOnRestore()

    SceneNodeList::iterator i = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while (i!=end) {
        (*i)->VOnRestore(pScene);
        ++i;
    }
    return true;
}

//
// SceneNode::VOnLostDevice					- Chapter X, page Y
//
bool SceneNode::VOnLostDevice(Scene *pScene)
{
    // This is meant to be called from any class
    // that inherits from SceneNode and overloads
    // VOnRestore()

    SceneNodeList::iterator i = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while (i != end)
    {
        (*i)->VOnLostDevice(pScene);
        ++i;
    }
    return true;
}

//
// SceneNode::VSetTransform					- Chapter 16, page 531
//
void SceneNode::VSetTransform(const Mat4x4 *toWorld, const Mat4x4 *fromWorld)
{
    m_Props.m_ToWorld = *toWorld;
    if (!fromWorld)
    {
        m_Props.m_FromWorld = m_Props.m_ToWorld.Inverse();
    }
    else
    {
        m_Props.m_FromWorld = *fromWorld;
    }
}

//
// SceneNode::VPreRender					- Chapter 16, page 532
//
bool SceneNode::VPreRender(Scene *pScene)
{
    // This was added post press! Is is always ok to read directly from the game logic.
    //How else can we get the transform matrix for the current SceneNode being rendered?
    StrongActorPtr pActor = StrongActorPtr(g_pApp->GetGameLogic()->VGetActor(m_Props.m_ActorId));
    if (pActor)
    {
        QSharedPointer<TransformComponent> pTc = QSharedPointer<TransformComponent>(pActor->GetComponent<TransformComponent>(TransformComponent::g_Name));
        if (pTc)
        {
            m_Props.m_ToWorld = pTc->GetTransform();
        }
    }

    pScene->PushAndSetMatrix(m_Props.m_ToWorld);
    return true;
}

//
// SceneNode::VPostRender					- Chapter 16, page 532
//
bool SceneNode::VPostRender(Scene *pScene)
{
    pScene->PopMatrix();
    return true;
}

//
// SceneNode::VIsVisible				- Chapter 16, page 533
//
bool SceneNode::VIsVisible(Scene *pScene) const
{
    // transform the location of this node into the camera space
    // of the camera attached to the scene

    Mat4x4 toWorld, fromWorld;
    pScene->GetCamera()->VGet()->Transform(&toWorld, &fromWorld);

    Vec3 pos = GetWorldPosition();

    Vec3 fromWorldPos = fromWorld.Xform(pos);

    Frustum const &frustum = pScene->GetCamera()->GetFrustum();

    bool isVisible = frustum.Inside(fromWorldPos, VGet()->Radius());
    return isVisible;
}

const Vec3 SceneNode::GetWorldPosition() const
{
    Vec3 pos = GetPosition();
    if (m_pParent)
    {
        pos += m_pParent->GetWorldPosition();
    }
    return pos;
}


//
// SceneNode::VOnUpdate					- Chapter 16, page 532
//
bool SceneNode::VOnUpdate(Scene *pScene, const unsigned long elapsedMs)
{
    // This is meant to be called from any class
    // that inherits from SceneNode and overloads
    // VOnUpdate()

    SceneNodeList::iterator i = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();

    while (i != end)
    {
        (*i)->VOnUpdate(pScene, elapsedMs);
        ++i;
    }
    return true;
}

bool SceneNode::VRenderChildren(Scene *pScene)
{
    //Iterate through the children....
    SceneNodeList::iterator i = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();

    while(i != end)
    {
        if((*i)->VPreRender(pScene))
        {
            // You could short-circuit rendering
            // if an object returns false from
            // VPreRender()

            // Don't render this node if you can't see it
            if((*i)->VIsVisible(pScene))
            {
                float alpha = (*i)->VGet()->m_Material.GetAlpha();
                if(alpha == fOPAQUE)
                {
                    (*i)->VRender(pScene);
                }
                else if(alpha != fTRANSPARENT)
                {
                    //The object isn't totally transparent
                    AlphaSceneNode *asn = GCC_NEW AlphaSceneNode;
                    Q_ASSERT(asn);
                    asn->m_pNode = *i;
                    asn->m_Concat = pScene->GetTopMatrix();

                    Vec4 worldPos(asn->m_Concat.GetPosition());

                    Mat4x4 fromWorld = pScene->GetCamera()->VGet()->FromWorld();

                    Vec4 screenPos = fromWorld.Xform(worldPos);

                    asn->m_ScreenZ = screenPos.z();

                    pScene->AddAlphaSceneNode(asn);
                }

                //see comment just below
                (*i)->VRenderChildren(pScene);
            }
            // post-press fix - if the parent is not visible, the childrend
            //           shouldn't be visible either.
            //What if something is fired from the current SceneNode
            //(*i)->VRenderChildren(pScene);
        }
        (*i)->VPostRender(pScene);
        ++i;
    }
    return true;
}

//
// SceneNode::VAddChild					- Chapter 16, page 535
//
bool SceneNode::VAddChild(QSharedPointer<ISceneNode> ikid)
{
    m_Children.push_back(ikid);

    QSharedPointer<SceneNode> kid = qSharedPointerCast<SceneNode>(ikid);

    kid->m_pParent = this;

    //The radius of the sphere should be fixed right here
    Vec3 kidPos = kid->VGet()->ToWorld().GetPosition();

    // [mrmike] - Post-press fix. This was not correct! subtracting the parents's position from the kidPos
    //            created a HUGE radius, depending on the location of the parent, which could be anywhere
    //            in the game world.

    //Vec3 dir = kidPos - m_Props.ToWorld().GetPosition();
    //float newRadius = dir.Length() + kid->VGet()->Radius();

    float newRadius = kidPos.Length() + kid->VGet()->Radius();

    if(newRadius > m_Props.m_Radius)
        m_Props.m_Radius = newRadius;

    return true;
}

//
// SceneNode::VRemoveChild					- not in the book
//
//   If an actor is destroyed it should be removed from the scene graph.
//   Generally the HumanView will recieve a message saying the actor has been
//   destroyed, and it will then call Scene::RemoveChild which will traverse
//   the scene graph to find the child that needs removing.
//
bool SceneNode::VRemoveChild(ActorId id)
{
    for(SceneNodeList::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        const SceneNodeProperties* pProps = (*i)->VGet();
        if(pProps->ActorID() != INVALID_ACTOR_ID && id == pProps->ActorID())
        {
            i = m_Children.erase(i);        //this can be expensive for vectors
            return true;
        }
    }
    return false;
}

bool SceneNode::VPick(Scene *pScene, RayCast *pRayCast)
{
    for(SceneNodeList::const_iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        bool result = (*i)->VPick(pScene, pRayCast);
        if(result == false)
            return false;
    }
    return true;
}

// This was changed post press - it was convenient to be able to set alpha on a parent node and
// have it propagate to all its children, since materials are set in the SceneNodes's properties, and
// not as a SceneNode that changes renderstate by itself.
void SceneNode::SetAlpha(float alpha)
{
    m_Props.SetAlpha(alpha);
    for(SceneNodeList::const_iterator i=m_Children.begin(); i!=m_Children.end(); ++i)
    {
        QSharedPointer<SceneNode> sceneNode = qSharedPointerCast<SceneNode>(*i);
        sceneNode->SetAlpha(alpha);
    }
}


////////////////////////////////////////////////////
// RootNode Implementation
////////////////////////////////////////////////////

//
// RootNode::RootNode					- Chapter 16, page 545
//
RootNode::RootNode()
: SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0, &Mat4x4::g_Identity)
{
    m_Children.reserve(RenderPass_Last);

    QSharedPointer<SceneNode> staticGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Static, &Mat4x4::g_Identity));
    m_Children.push_back(staticGroup);  //RenderPass_Static = 0;

    QSharedPointer<SceneNode> actorGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Actor, &Mat4x4::g_Identity));
    m_Children.push_back(actorGroup);  //RenderPass_Actor = 1;

    QSharedPointer<SceneNode> skyGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Sky, &Mat4x4::g_Identity));
    m_Children.push_back(skyGroup);  //RenderPass_Sky = 2;

    QSharedPointer<SceneNode> invisibleGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_NotRendered, &Mat4x4::g_Identity));
    m_Children.push_back(invisibleGroup);  //RenderPass_NotRendered = 3;
}

//
// RootNode::VAddChild					- Chapter 16, page 546
//
bool RootNode::VAddChild(QSharedPointer<ISceneNode> kid)
{
    // The Root node has children that divide the scene graph into render passes.
    // Scene nodes will get added to these children based on the value of the
    // render pass member variable.
    RenderPass pass = kid->VGet()->Renderpass();
    if((unsigned)pass >= m_Children.size() || !m_Children[pass])
    {
        Q_ASSERT(0 && ("There is no such render pass"));
        return false;
    }

    return m_Children[pass]->VAddChild(kid);
}

//
// RootNode::VRemoveChild						- not described in the book
//
//   Returns false if nothing was removed
//
bool RootNode::VRemoveChild(ActorId id)
{
    bool anythingRemoved = false;
    for(int i = RenderPass_0; i < RenderPass_Last; ++i)
    {
        if(m_Children[i]->VRemoveChild(id))
        {
            anythingRemoved = true;
        }
    }
    return anythingRemoved;
}

//
// RootNode::VRenderChildren					- Chapter 16, page 547
//
bool RootNode::VRenderChildren(Scene *pScene)
{
    // This code creates fine control of the render passes.

    for(int pass = RenderPass_0; pass < RenderPass_Last; ++pass)
    {
        switch(pass)
        {
        case RenderPass_Static:
        case RenderPass_Actor:
            m_Children[pass]->VRenderChildren(pScene);
            break;
        case RenderPass_Sky:
        {
            QSharedPointer<IRenderState> skyPass = pScene->GetRenderer()->VPrepareSkyBoxPass();
            m_Children[pass]->VRenderChildren(pScene);
            break;
        }
        }
    }
    return true;
}

////////////////////////////////////////////////////
// CameraNode Implementation
////////////////////////////////////////////////////


//
// CameraNode::VRender					- Chapter 16, page 550
//
bool CameraNode::VRender(Scene *pScene)
{
    if(m_DebugCamera)
    {
        pScene->PopMatrix();

        //this will require a modern openGL function to render
//        m_Frustum.Render();

        pScene->PushAndSetMatrix(m_Props.ToWorld());
    }
    return true;
}

//
// CameraNode::VOnRestore				- Chapter 16, page 550
//
bool CameraNode::VOnRestore(Scene *pScene)
{
    //Where do we get window width and height?
//    m_Frustum.SetAspect((float)g_pApp->GetScreenSize().GetX() / (float)g_pApp->GetScreenSize().GetY());
    m_Projection.perspective(m_Frustum.m_Fov, m_Frustum.m_Aspect, m_Frustum.m_Near, m_Frustum.m_Far);
    pScene->GetRenderer()->VSetProjectionTransform(&m_Projection);
    return true;
}

//
// CameraNode::SetView					- Chapter 16, page 550
//
//    Note: this is incorrectly called CameraNode::SetView in the book
//
void CameraNode::SetViewTransform(Scene *pScene)
{
    //If there is a target, make sure the camera is
    //rigidly attached right behind the target
    if(m_pTarget)
    {
        Mat4x4 mat = m_pTarget->VGet()->ToWorld();
        Vec4 at = m_CamOffsetVector;
        Vec4 atWorld = mat.Xform(at);
        Vec3 pos = mat.GetPosition() + Vec3(atWorld);
        mat.SetPosition(pos);
        VSetTransform(&mat);
    }

    m_View = VGet()->FromWorld();

    pScene->GetRenderer()->VSetViewTransform(&m_View);
    return;
}

//
// CameraNode::GetWorldViewProjection			- not described in the book
//
//    Returns the concatenation of the world and view projection, which is generally sent into vertex shaders
//
Mat4x4 CameraNode::GetWorldViewProjection(Scene *pScene)
{
    Q_UNUSED(pScene);
    //TODO: REPLACE THIS WITH STUFF THAT WILL WORK FOR OPENGL
    Mat4x4 world = pScene->GetTopMatrix();
//    Mat4x4 world;//tHIS WILL BE JUST AN IDENTITY MATRIX FOR NOW
    Mat4x4 view = VGet()->FromWorld();
    Mat4x4 worldView = world * view;
    return worldView * m_Projection;
}


// TestObject::g_CubeVerts
//        - Chapter 14, page 495

Vec3 Cube::g_CubeVerts[] =
{
    Vec3( 0.5,0.5,-0.5),		// Vertex 0.
    Vec3(-0.5,0.5,-0.5),		// Vertex 1.
    Vec3(-0.5,0.5,0.5),  		// And so on.
    Vec3(0.5,0.5,0.5),
    Vec3(0.5,-0.5,-0.5),
    Vec3(-0.5,-0.5,-0.5),
    Vec3(-0.5,-0.5,0.5),
    Vec3(0.5,-0.5,0.5)
};

unsigned int Cube::g_TestObjectIndices[][3] =
{
    { 0,1,2 },                // Face 0 has three vertices.
    { 0,2,3 },                // And so on.
    { 0,4,5 },
    { 0,5,1 },
    { 1,5,6 },
    { 1,6,2 },
    { 2,6,7 },
    { 2,7,3 },
    { 3,7,4 },
    { 3,4,0 },
    { 4,7,6 },
    { 4,6,5 }
};



Cube::Cube(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const Mat4x4 *pMatrix)
    : SceneNode(actorId, renderComponent, RenderPass_Actor, pMatrix)
    , m_pVerts(0)
    , m_pIndices(0)
    , m_numVerts(0)
    , m_numPolys(0)
    , m_bTextureHasAlpha(false)
{

}

Cube::~Cube()
{
    if(m_pIndices)
    {
        m_pIndices->destroy();
        delete m_pIndices;
    }

    if(m_pVerts)
    {
        m_pVerts->destroy();
        delete m_pVerts;
    }
}

bool Cube::VOnRestore(Scene *pScene)
{
    SceneNode::VOnRestore(pScene);

    m_pVerts = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    if(!m_pVerts->create())
    {
        qDebug() << "Vertex buffer not created";
        return false;
    }
    if(!m_pVerts->bind())
    {
        qDebug() << "Vertex buffer not bound to context";
        return false;
    }
    m_pVerts->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_pVerts->allocate(g_CubeVerts, sizeof(g_CubeVerts));

    m_pIndices = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_pIndices->create();
    m_pIndices->bind();
    m_pIndices->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_pIndices->allocate(g_TestObjectIndices, sizeof(g_TestObjectIndices));

//    QImage *image = new QImage("container.jpg");
//    texture = new QOpenGLTexture(image->mirrored());
    //    texture->bind();
    return true;
}

bool Cube::VRender(Scene *pScene)
{
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLvoid*)0);
    return true;
}


Grid::Grid(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const Mat4x4 *pMatrix)
    : SceneNode(actorId, renderComponent, RenderPass_0, pMatrix)
{
    m_bTextureHasAlpha = false;
    m_numVerts = m_numPolys = 0;
    m_pVertexBuffer = NULL;
    m_pIndexBuffer = NULL;
}

Grid::~Grid()
{
    if(m_pIndexBuffer)
    {
        m_pIndexBuffer->release();
        delete m_pIndexBuffer;
    }
    if(m_pVertexBuffer)
    {
        m_pVertexBuffer->release();
        delete m_pVertexBuffer;
    }
}

bool Grid::VOnRestore(Scene *pScene)
{
    SceneNode::VOnRestore(pScene);

    if(m_pVertexBuffer)
    {
        m_pVertexBuffer->destroy();
    }
    else
    {
        m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    }
    if(m_pIndexBuffer)
    {
        m_pIndexBuffer->destroy();
    }
    else
    {
        m_pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    }

    m_pVertexBuffer->create();
    m_pIndexBuffer->create();

    //Shader stuff here?
//    V_RETURN (m_VertexShader.OnRestore(pScene) );
//	V_RETURN (m_PixelShader.OnRestore(pScene) );

    GridRenderComponent* grc = static_cast<GridRenderComponent*>(m_RenderComponent);

    int squares = grc->GetDivision();

    SetRadius( sqrt(squares * squares / 2.0f) );

    // Create the vertex buffer - we'll need enough verts
    // to populate the grid. If we want a 2x2 grid, we'll
    // need 3x3 set of verts.
    m_numVerts = (squares+1)*(squares+1);    // Create vertex buffer

    // Fill the vertex buffer. We are setting the tu and tv texture
    // coordinates, which range from 0.0 to 1.0
//    D3D11Vertex_UnlitTextured *pVerts = GCC_NEW D3D11Vertex_UnlitTextured[m_numVerts];
//	GCC_ASSERT(pVerts && "Out of memory in D3DGrid11::VOnRestore()");
//	if (!pVerts)
//		return E_FAIL;

    return true;
}

bool Grid::VRender(Scene *pScene)
{
    return true;
}
