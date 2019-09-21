#include "GameEngineStd.h"

#include "GLRenderer.h"
//#include "Lights.h"
#include "../UserInterface/HumanView.h"



//
// class GLRendererAlphaPass			- Chapter 16, page 543
//
class GLRendererAlphaPass : public IRenderState
{
protected:
//    ID3D11BlendState* m_pOldBlendState;
    float m_OldBlendFactor[ 4 ];
    unsigned int m_OldSampleMask;

//    ID3D11BlendState* m_pCurrentBlendState;

public:
    GLRendererAlphaPass();
    ~GLRendererAlphaPass();
    QString VToString() { return "GLRendererAlphaPass"; }
};

//
// GLRendererAlphaPass::GLRendererAlphaPass			- Chapter 16, page 544
//
GLRendererAlphaPass::GLRendererAlphaPass()
{

}

//
// GLRendererAlphaPass::~GLRendererAlphaPass			- Chapter 16, page 544
//
GLRendererAlphaPass::~GLRendererAlphaPass()
{

}

//
// class D3DRendererSkyBoxPass11							- Chapter 16, page 548
//
class GLRendererSkyBoxPass : public IRenderState
{
protected:
//	ID3D11DepthStencilState* m_pOldDepthStencilState;
//	ID3D11DepthStencilState* m_pSkyboxDepthStencilState;

public:
    GLRendererSkyBoxPass();
    ~GLRendererSkyBoxPass();
    QString VToString() { return "GLRendererSkyBoxPass"; }
};

//
// GLRendererSkyBoxPass::GLRendererSkyBoxPass()		- Chapter 16, page 548
//
GLRendererSkyBoxPass::GLRendererSkyBoxPass()
{

}

//
// GLRendererSkyBoxPass::~GLRendererSkyBoxPass()		- Chapter 16, page 548
//
GLRendererSkyBoxPass::~GLRendererSkyBoxPass()
{

}

void GLLineDrawer::DrawLine(const Vec3& from,const Vec3& to,const Color& color)
{
#if 0
    HRESULT hr;

    shared_ptr<Scene> pScene = g_pApp->GetHumanView()->m_pScene;
    shared_ptr<IRenderer> pRenderer = pScene->GetRenderer();

    if ( FAILED(m_LineDrawerShader.SetupRender(&(*pScene)) ) )
        return;

    m_LineDrawerShader.SetDiffuse("art\\grid.dds", color);

    // Set vertex buffer
    UINT stride = sizeof( Vec3 );
    UINT offset = 0;

    D3D11_MAPPED_SUBRESOURCE MappedResource;
    V( DXUTGetD3D11DeviceContext()->Map( m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ) );

    Vec3 * pVerts = ( Vec3* )MappedResource.pData;
    pVerts[0] = from;
    pVerts[1] = to;
    DXUTGetD3D11DeviceContext()->Unmap( m_pVertexBuffer, 0 );

    DXUTGetD3D11DeviceContext()->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

    // Set primitive topology
    DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );

    DXUTGetD3D11DeviceContext()->Draw( 2, 0 );
#endif
}

void GLLineDrawer::OnRestore()
{

}

bool GLRenderer::VPreRender()
{
        glClearColor(m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3], m_backgroundColor[0]);
//        glClearDepth(1);//initial value is 1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        DXUTGetD3D11DeviceContext()->ClearRenderTargetView( DXUTGetD3D11RenderTargetView(), m_backgroundColor );

        //
        // Clear the depth buffer to 1.0 (max depth)
        //
//        DXUTGetD3D11DeviceContext()->ClearDepthStencilView( DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
        m_program->bind();
        m_object.bind();
    return true;
}

bool GLRenderer::VPostRender(void)
{
    m_object.release();
//    m_program->release();
    return true;
}

bool GLRenderer::VOnRestore()
{
    bool ret;
    delete m_program;

    m_program = new QOpenGLShaderProgram();
    m_program->create();
    ret = m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                       "#version 100\n"
                                       "attribute vec3 position; \n"
                                       "void main() \n"
                                       "{ \n"
                                       " gl_Position = vec4(position.x, position.y, position.z, 1.0); \n"
                                       "} \n");
    if(!ret)
    {
        QString error = m_program->log();
        qDebug() << error;
        return false;
    }

    ret = m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                       "#version 100\n"
                                       "precision mediump float; \n"
                                       "void main() \n"
                                       "{ \n"
                                       " gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0); \n"
                                       "} \n");

    if(!ret)
    {
        return false;
    }

    m_program->link();
    m_program->bind();
    m_object.create();
    m_object.bind();
    int vertexLocation = m_program->attributeLocation("position");
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, 12);
    m_object.release();
//    m_program->release();

    return true;
}


bool GLRenderer::CompileShader(const char *source, QOpenGLShader *shader)
{
    bool result = true;
    result = shader->compileSourceCode(source);

    //A little debuggery; might need improvement
    if(!result)
    {
        qWarning() << shader->log();
    }
    return result;
}

bool GLRenderer::CompileShaderFromFile(std::wstring szFileName, QOpenGLShader *shader)
{
    bool result = true;
    result = shader->compileSourceFile(QString::fromStdWString(szFileName));

    //A little debuggery; might need improvement
    if(!result)
    {
        qWarning() << shader->log();
    }
    return result;
}

void GLRenderer::VDrawLine(const Vec3 &from, const Vec3 &to, const Color &color)
{
    if(!m_pLineDrawer)
    {
        m_pLineDrawer = GCC_NEW GLLineDrawer();
        m_pLineDrawer->OnRestore();
    }
    m_pLineDrawer->DrawLine(from, to, color);
}
#if 0
QSharedPointer<IRenderState> GLRenderer::VPrepareAlphaPass()
{
    return QSharedPointer<IRenderState>(GCC_NEW GLRendererAlphaPass());
}

QSharedPointer<IRenderState> GLRenderer::VPrepareSkyBoxPass()
{
    return QSharedPointer<IRenderState>(GCC_NEW GLRendererSkyBoxPass());
}
#endif
