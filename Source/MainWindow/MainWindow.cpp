//#include "gameenginestd.h"

#include "MainWindow.h"
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
#include <QResizeEvent>
#include <QDir>
//#include <QApplication>

#include "../MainWindow/vertex.h"

//Front Vertices
#define VERTEX_FTR Vertex( QVector3D( 0.5f, 0.5f, 0.5f ), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_FTL Vertex( QVector3D(-0.5f, 0.5f, 0.5f ), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_FBL Vertex( QVector3D(-0.5f,-0.5f, 0.5f ), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_FBR Vertex( QVector3D( 0.5f,-0.5f, 0.5f ), QVector3D( 0.0f, 0.0f, 0.0f ) )
//Front Vertices
#define VERTEX_BTR Vertex( QVector3D( 0.5f, 0.5f,-0.5f ), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_BTL Vertex( QVector3D(-0.5f, 0.5f,-0.5f ), QVector3D( 0.0f, 1.0f, 1.0f ) )
#define VERTEX_BBL Vertex( QVector3D(-0.5f,-0.5f,-0.5f ), QVector3D( 1.0f, 0.0f, 1.0f ) )
#define VERTEX_BBR Vertex( QVector3D( 0.5f,-0.5f,-0.5f ), QVector3D( 1.0f, 1.0f, 1.0f ) )


//Create a colored CUBED
static const Vertex sg_vertexes[] = {
    // Face 1 (Front)
    VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
    VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
    // Face 2 (Back)
    VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
    VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
    // Face 3 (TOP)
    VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
    VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
    // Face 4 (BOTTOM)
    VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
    VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
    // Face 5 (LEFT)
    VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
    VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
    // Face 6 (RIGHT)
    VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
    VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR

MainWindow::MainWindow()
{
    m_transform.translate(0.0f, 0.0f, -5.0f);
}

MainWindow::~MainWindow()
{
    makeCurrent();
    teardownGL();
}

void MainWindow::initializeGL()
{
    //Initializa OpenGL Backend
    initializeOpenGLFunctions();
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    printContextInformation();

    //Set Global information
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    // Application-specific initialization
    {
        //Create Shader (Do not release until VAO is created)
        m_program = new QOpenGLShaderProgram();
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/simple.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/simple.frag");
        m_program->link();
        m_program->bind();

        // Cache Uniform Locations
        u_modelToWorld = m_program->uniformLocation("modelToWorld");
        u_worldToView = m_program->uniformLocation("worldToView");

        // Create Buffer (Do not release until VAO is created)
        m_vertex.create();
        m_vertex.bind();
        m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_vertex.allocate(sg_vertexes, sizeof(sg_vertexes));

        //Create Vertex Array Object
        m_object.create();
        m_object.bind();
        m_program->enableAttributeArray(0);
        m_program->enableAttributeArray(1);
        m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
        m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

        //Release (unbind) all
        m_object.release();
        m_vertex.release();
        m_program->release();
    }
}
#if 0
void MainWindow::event(QEvent *event)
{
    if(event->type() == QEvent::Resize)
    {
        QResizeEvent *re = static_cast<QResizeEvent *>(event);
    }
}
#endif
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QOpenGLWindow::resizeEvent(event);
//    g_pApp->OnDisplayChange(event->size().width(), event->size().height() );
}

void MainWindow::resizeGL(int width, int height)
{
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
}

void MainWindow::paintGL()
{
    //Clear
    glClear(GL_COLOR_BUFFER_BIT);

    // Render using our shader
    m_program->bind();
    m_program->setUniformValue(u_worldToView, m_projection);
    {
        m_object.bind();
        m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
        glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));
        m_object.release();
    }
    m_program->release();
}

void MainWindow::update()
{
    //Update instance information
    m_transform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));

    //Schedule a redraw
    QOpenGLWindow::update();
}

void MainWindow::teardownGL()
{
    //Actually destroy our OpenGL information
    m_object.destroy();
    m_vertex.destroy();
    delete m_program;
}

void MainWindow::printContextInformation()
{
    //let's hold off on this part for now
}
