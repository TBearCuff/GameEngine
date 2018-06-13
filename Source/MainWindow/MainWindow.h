#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include "Transform3D.h"

class QOpenGLShaderProgram;

class MainWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT
//OpenGL Events
public:
    MainWindow();
    ~MainWindow();

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void teardownGL();

protected slots:
    void update();

protected:
//    void event(QEvent *event);
//    void keyPressEvent(QKeyEvent *event);
//    void keyReleaseEvent(QKeyEvent *event);
//    void mousePressEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
//    void mouseDoubleClickEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event) override;

signals:

public slots:

private:
    //OpenGL State Information
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;

    //Shader Information
    int u_modelToWorld;
    int u_worldToView;
    QMatrix4x4 m_projection;
    Transform3D m_transform;

    //Private helpers
    void printContextInformation();
};

#endif // MAINWINDOW_H
