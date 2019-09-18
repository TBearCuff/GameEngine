#ifndef SHADERS_H
#define SHADERS_H

#include <QOpenGLShader>


class GLSL_VertexShader
{
public:
    GLSL_VertexShader();
    ~GLSL_VertexShader();

    bool OnRestore();
    void EnableLights(bool enableLights) { m_enableLights = enableLights; }

protected:
    QOpenGLShader* m_shader;
    QString m_vertexAtrribName;
    bool m_enableLights;

};

class GLSL_FragmentShader
{
public:
    GLSL_FragmentShader();
    ~GLSL_FragmentShader();

    bool OnRestore();

protected:
    QOpenGLShader* m_shader;

};

#endif // SHADERS_H
