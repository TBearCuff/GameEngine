#ifndef OPENGLRENDERWINDOW_H
#define OPENGLRENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>
#include <QResizeEvent>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class OpenGLRenderWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLRenderWindow(QWindow *parent = 0);
//    ~OpenGLRenderWindow();

    virtual void VSetBackgroundColor(float fgR, float fgG, float fgB, float fgA)
    {
        m_backgroundColor[0] = fgR;
        m_backgroundColor[1] = fgG;
        m_backgroundColor[2] = fgB;
        m_backgroundColor[3] = fgA;
    }

    virtual bool VPreRender();
    virtual bool VPostRender();
    virtual bool VOnRestore();
    virtual bool PreRestore();
    virtual bool PostRestore();
//    virtual void VCalcLighting(Lights *lights, int maximumLights) { }

signals:
    void updateFrame();

protected:
    float m_backgroundColor[4];
    void resizeEvent(QResizeEvent ev);
    bool event(QEvent *event) override;

private:
    QOpenGLContext* context;
    QOpenGLShaderProgram *m_program;
    QOpenGLVertexArrayObject m_object;

    int vertexLocation;
};

#endif // OPENGLRENDERWINDOW_H
