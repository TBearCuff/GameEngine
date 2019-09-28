#include "GameEngineStd.h"

#include "GameEngineApp.h"
#include "../MainWindow/OpenGLRenderWindow.h"
#include "Geometry.h"
#include "Lights.h"
//#include "Mesh.h"
#include "SceneNodes.h"
#include "../ResourceCache/ResCache.h"
//#include "Shaders.h"

static const char *vertexShaderSource =
        "#version 100\n"
        "attribute vec3 position; \n"
        "void main() \n"
        "{ \n"
        " gl_Position = vec4(position.x, position.y, position.z, 1.0); \n"
        "} \n";

static const char *fragmentShaderSource =
        "#version 100\n"
        "precision mediump float; \n"
        "void main() \n"
        "{ \n"
        " gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0); \n"
        "} \n";


GLSL_VertexShader::GLSL_VertexShader()
{
    m_shader = new QOpenGLShader(QOpenGLShader::Vertex);
}

GLSL_VertexShader::~GLSL_VertexShader()
{
    delete m_shader;
}

bool GLSL_VertexShader::OnRestore()
{
    m_shader->compileSourceCode(vertexShaderSource);
    return true;
}

GLSL_FragmentShader::GLSL_FragmentShader()
{
    m_shader = new QOpenGLShader(QOpenGLShader::Fragment);
}

GLSL_FragmentShader::~GLSL_FragmentShader()
{
    delete m_shader;
}

bool GLSL_FragmentShader::OnRestore()
{
    m_shader->compileSourceCode(fragmentShaderSource);
    return true;
}
