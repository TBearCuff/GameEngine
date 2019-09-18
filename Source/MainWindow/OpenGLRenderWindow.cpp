#include "OpenGLRenderWindow.h"

OpenGLRenderWindow::OpenGLRenderWindow(QWindow *parent) : QWindow(parent),
    context(0)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

bool OpenGLRenderWindow::VPreRender()
{
    context->makeCurrent(this);
    glClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
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

bool OpenGLRenderWindow::VPostRender()
{
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
                                       "void main() \n"
                                       "{ \n"
                                       " gl_Position = vec4(position.x, position.y, position.z, 1.0); \n"
                                       "} \n");

    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                       "#version 100\n"
                                       "precision mediump float; \n"
                                       "void main() \n"
                                       "{ \n"
                                       " gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0); \n"
                                       "} \n");



    m_program->link();
    m_program->bind();
//    m_object.bind();
    vertexLocation = m_program->attributeLocation("position");


//    m_object.release();
    m_program->release();

    requestUpdate();
    return true;

}

bool OpenGLRenderWindow::PreRestore()
{
//    context->makeCurrent(this);
    m_program->bind();
    return true;
}

bool OpenGLRenderWindow::PostRestore()
{
    if(!m_object.create())
    {
        qDebug() << "VAO is not supported.";
        return false;
    }
    m_object.bind();
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3);
    m_object.release();
    m_program->release();
    return true;
}

void OpenGLRenderWindow::resizeEvent(QResizeEvent ev)
{
    glViewport(0,0,ev.size().width(),ev.size().height());
}

bool OpenGLRenderWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        emit updateFrame();
        return true;
    default:
        return QWindow::event(event);
    }

}
