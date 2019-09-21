#include "GameEngineStd.h"
#include "OpenGLRenderWindow.h"

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

bool GLLineDrawer::OnRestore()
{
//    D3D11_BUFFER_DESC bd;
//	ZeroMemory( &bd, sizeof(bd) );
//    bd.Usage = D3D11_USAGE_DYNAMIC;
//    bd.ByteWidth = sizeof( D3D11Vertex_PositionColored ) * 2;
//    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//    D3D11_SUBRESOURCE_DATA InitData;
//	ZeroMemory( &InitData, sizeof(InitData) );
//    InitData.pSysMem = &m_Verts;
//	HRESULT hr;

//	V_RETURN ( DXUTGetD3D11Device()->CreateBuffer( &bd, &InitData, &m_pVertexBuffer ) );

//	shared_ptr<Scene> pScene = g_pApp->GetHumanView()->m_pScene;

//	V_RETURN (m_LineDrawerShader.OnRestore(&(*pScene)) );

    return true;
}



OpenGLRenderWindow::OpenGLRenderWindow(QWindow *parent)
    : QWindow(parent)
    , context(0)
    , m_program(0)
    , m_pLineDrawer(NULL)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

bool OpenGLRenderWindow::VPreRender()
{
    context->makeCurrent(this);
    glClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
//        glClearDepth(1);//initial value is 1
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->bind();
    m_object.bind();
    return true;
}

bool OpenGLRenderWindow::VPostRender()
{
    m_object.release();
    m_program->release();
    if(isExposed())
        context->swapBuffers(this);
    //send signal to fire off next render event
    requestUpdate();
    return true;
}

bool OpenGLRenderWindow::VOnRestore()
{
    if(context)
        return true;

    context = new QOpenGLContext(this);
    context->setFormat(requestedFormat());
    if(!context->create())
        return false;
    if(!context->makeCurrent(this))
        return false;
    initializeOpenGLFunctions();

    glViewport(0,0,this->width(),this->height());

    m_program = new QOpenGLShaderProgram();

    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                       "#version 100\n"
                                       "attribute vec3 position; \n"
                                       "attribute vec3 color; \n"
                                       "attribute vec2 texCoord; \n"
                                       "varying vec3 ourColor; \n"
                                       "varying vec2 TexCoord; \n"
                                       "void main() \n"
                                       "{ \n"
                                       " gl_Position = vec4(position, 1.0); \n"
                                       " ourColor = color; \n"
                                       " TexCoord = texCoord; \n"
                                       "} \n");

    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                       "#version 100\n"
                                       "precision mediump float; \n"
                                       "varying vec3 ourColor; \n"
                                       "varying vec2 TexCoord; \n"
                                       "uniform sampler2D ourTexture; \n"
                                       "void main() \n"
                                       "{ \n"
                                       " gl_FragColor = texture2D(ourTexture, TexCoord) * vec4(ourColor,1.0); \n"
                                       "} \n");



    m_program->link();
    m_program->bind();
    vertexLocation = m_program->attributeLocation("position");
    colorLocation = m_program->attributeLocation("color");
    texCoordLocation = m_program->attributeLocation("texCoord");
    uniformSamplerLocation = m_program->uniformLocation("ourTexture");

    m_program->release();

    requestUpdate();
    return true;

}

bool OpenGLRenderWindow::VPreRestore()
{
    m_program->bind();
    if(!m_object.create())
    {
        qDebug() << "VAO is not supported.";
        return false;
    }
    m_object.bind();
    return true;
}

bool OpenGLRenderWindow::VPostRestore()
{
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, 32);
    m_program->setAttributeBuffer(colorLocation, GL_FLOAT, 12, 3, 32);
    m_program->setAttributeBuffer(texCoordLocation, GL_FLOAT, 24, 2, 32);
    m_program->enableAttributeArray(vertexLocation);
    m_program->enableAttributeArray(colorLocation);
    m_program->enableAttributeArray(texCoordLocation);
    m_program->setUniformValue(uniformSamplerLocation, 0);
    m_object.release();
    m_program->release();
    return true;
}

void OpenGLRenderWindow::VDrawLine(const Vec3 &from, const Vec3 &to, const Color &color)
{
    if (!m_pLineDrawer)
    {
        m_pLineDrawer = GCC_NEW GLLineDrawer();
        m_pLineDrawer->OnRestore();
    }
    m_pLineDrawer->DrawLine(from, to, color);
}

bool OpenGLRenderWindow::CompileShader(const char *source, QOpenGLShader *shader)
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

bool OpenGLRenderWindow::CompileShaderFromFile(std::wstring szFileName, QOpenGLShader *shader)
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

void OpenGLRenderWindow::resizeEvent(QResizeEvent ev)
{
    glViewport(0,0,ev.size().width(),ev.size().height());
}

bool OpenGLRenderWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        emit updateFrame();
        return true;
    default:
        return QWindow::event(event);
    }

}
