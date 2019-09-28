#include "GameEngineStd.h"

#include "../MainWindow/OpenGLRenderWindow.h"
#include "GameEngineApp.h"
//#include "../EventManager/EventManager.h"
//#include "../EventManager/Events.h"
//#include "../Utilities/string.h"
#include "Geometry.h"
#include "Lights.h"

#include "Scene.h"

////////////////////////////////////////////////////
// Scene Implementation
////////////////////////////////////////////////////


//
// Scene::Scene						- Chapter 16, page 539
//
//    Note: The shared_ptr<IRenderer> was added to allow for both D3D9 and D3D11 renderer implementations.
//          The book only describes D3D11, so to find all the differences, just search for m_Renderer!
//
Scene::Scene(QSharedPointer<OpenGLRenderWindow> renderer)
{
    m_Root.reset(GCC_NEW RootNode());
    m_Renderer = renderer;
//    m_LightManager = new LightManager;

    m_MatrixStack = GCC_NEW QStack<Mat4x4>();

    //Add event manager stuff here?
}

//
// Scene::~Scene					- Chapter 16, page 539
//
Scene::~Scene()
{
    SAFE_DELETE(m_MatrixStack);
//    SAFE_DELETE(m_LightManager);
}

//
// Scene::OnRender					- Chapter 16, page 539
//
void Scene::OnRender()
{
    // The render passes usually go like this
    // 1. Static objects & terrain
    // 2. Actors (dynamic objects that can move)
    // 3. The Sky
    // 4. Anything with Alpha

    if(m_Root && m_Camera)
    {
        // The scene root could be anything, but it
        // is usually a SceneNode with the identity
        // matrix

        m_Camera->SetViewTransform(this);

//        m_LightManager->CalcLighting(this);

        if(m_Root->VPreRender(this))
        {
            m_Root->VRender(this);
            m_Root->VRenderChildren(this);
            m_Root->VPostRender(this);
        }

        RenderAlphaPass();
    }
    return;
}

//
// Scene::OnLostDevice						- not in the book
//
//    All Scene nodes implement VOnLostDevice, which is called in the D3D9 renderer.
//
void Scene::OnLostDevice()
{
    if(m_Root)
    {
        m_Root->VOnLostDevice(this);
    }
    return;
}

bool Scene::OnUpdate(const int deltaMilliseconds)
{
    if (!m_Root)
        return true;

//    static DWORD lastTime = timeGetTime();
//    DWORD elapsedTime = 0;
//    DWORD now = timeGetTime();

//    elapsedTime = now - lastTime;
//    lastTime = now;

    return m_Root->VOnUpdate(this, deltaMilliseconds);
}

//
// Scene::OnRestore					- Chapter 16, page 540
//
void Scene::OnRestore()
{
    if(!m_Root)
    {
        return;
    }

    m_Renderer->VOnRestore();

    m_Root->VOnRestore(this);
    return;
}

bool Scene::AddChild(ActorId id, QSharedPointer<ISceneNode> kid)
{
    if(id != INVALID_ACTOR_ID)
    {
        //This allows us to search for this later based on actor is
        m_ActorMap[id] = kid;
    }

//    QSharedPointer<LightNode> pLight = dynamic_pointer_cast<LightNode>(kid);
//    if(pLight != NULL && m_LightManager->m_Lights.size()+1 < MAXIMUM_LIGHTS_SUPPORTED)
//    {
//        m_LightManager->m_Lights.push_back(pLight);
//    }
//    return m_Root->VAddChild(kid);
    return true;
}

bool Scene::RemoveChild(ActorId id)
{
    if(id == INVALID_ACTOR_ID)
    {
        return false;
    }

//    QSharedPointer<ISceneNode> kid = FindActor(id);
//    QSharedPointer<LightNode> pLight =  dynamic_pointer_cast<LightNode>(kid);
//    if(pLight != NULL)
//    {
//        m_LightManager->m_Lights.remove(pLight);
//    }
//    m_ActorMap.erase(id);
//    return m_Root->VRemoveChild(id);
    return true;
}

void Scene::PushAndSetMatrix(const Mat4x4 &toWorld)
{
    // Note this code carefully!!!!! It is COMPLETELY different
    // from some DirectX 9 documentation out there....
    // Scene::PushAndSetMatrix - Chapter 16, page 541

    m_MatrixStack->push(toWorld);
    Mat4x4 mat = m_MatrixStack->top();
    m_Renderer->VSetWorldTransform(&mat);
}

void Scene::PopMatrix()
{
    // Scene::PopMatrix - Chapter 16, page 541

    Mat4x4 mat = m_MatrixStack->pop();
    m_Renderer->VSetWorldTransform(&mat);
}

void Scene::RenderAlphaPass()
{
    QSharedPointer<IRenderState> alphaPass = m_Renderer->VPrepareAlphaPass();

    std::sort(m_AlphaSceneNodes.begin(), m_AlphaSceneNodes.end());
    //    m_AlphaSceneNodes.sort();
    while (!m_AlphaSceneNodes.empty())
    {
        AlphaSceneNodes::reverse_iterator i = m_AlphaSceneNodes.rbegin();
        PushAndSetMatrix((*i)->m_Concat);
        (*i)->m_pNode->VRender(this);
        delete (*i);
        PopMatrix();
        m_AlphaSceneNodes.pop_back();
    }
}




