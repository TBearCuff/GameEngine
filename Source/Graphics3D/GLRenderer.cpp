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

////////////////////////////////////////////////////
// D3DRenderer11 Implementation
//
//    Not described in the book - but it abstracts
//    some of the calls to get the game engine to run
//    under DX11.
////////////////////////////////////////////////////
#if 0
void GLRenderer::VOnRestore()
{
    SAFE_DELETE(D3DRenderer::g_pTextHelper);
    D3DRenderer::g_pTextHelper = GCC_NEW CDXUTTextHelper( DXUTGetD3D11Device(), DXUTGetD3D11DeviceContext(), &g_DialogResourceManager, 15 );

    return;
}
#endif

bool GLRenderer::VPreRender()
{
//    if (DXUTGetD3D11DeviceContext() && DXUTGetD3D11RenderTargetView())
    {
        glClearColor(m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3], m_backgroundColor[0]);
//        glClearDepth(1);//initial value is 1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        DXUTGetD3D11DeviceContext()->ClearRenderTargetView( DXUTGetD3D11RenderTargetView(), m_backgroundColor );

        //
        // Clear the depth buffer to 1.0 (max depth)
        //
//        DXUTGetD3D11DeviceContext()->ClearDepthStencilView( DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
    }
    return true;
}

bool GLRenderer::VPostRender(void)
{
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
