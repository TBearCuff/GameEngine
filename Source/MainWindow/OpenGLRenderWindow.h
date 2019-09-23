#ifndef OPENGLRENDERWINDOW_H
#define OPENGLRENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>
#include <QResizeEvent>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include "../Graphics3D/Geometry.h"
typedef QVector4D Color;

//
// struct ConstantBuffer_Matrices
//
struct ConstantBuffer_Matrices
{
    Mat4x4 m_WorldViewProj;
    Mat4x4 m_World;
};

//
// struct ConstantBuffer_Material
//
struct ConstantBuffer_Material
{
    Vec4 m_vDiffuseObjectColor;
    Vec4 m_vAmbientObjectColor;
    bool m_bHasTexture;
    Vec3 m_vUnused;
};

#define MAXIMUM_LIGHTS_SUPPORTED (8)

//
// struct ConstantBuffer_Lighting
//
struct ConstantBuffer_Lighting
{
    Vec4 m_vLightDiffuse[MAXIMUM_LIGHTS_SUPPORTED];
    Vec4 m_vLightDir[MAXIMUM_LIGHTS_SUPPORTED];
    Vec4 m_vLightAmbient;
    unsigned int m_nNumLights;
    Vec3 m_vUnused;
};

class GLLineDrawer
{
public:
    GLLineDrawer() { m_pVertexBuffer = NULL; }
    ~GLLineDrawer() {
        if(m_pVertexBuffer)
        {
            m_pVertexBuffer->destroy();
            delete m_pVertexBuffer;
        };
    }

    void DrawLine(const Vec3& from, const Vec3& to, const Color& color);
    bool OnRestore();

protected:
    Vec3            m_Verts[2];
//    LineDraw_Hlsl_Shader		m_LineDrawerShader;     //Shaders.h
    QOpenGLBuffer* m_pVertexBuffer;
};
class IRenderState;
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
    virtual bool VPreRestore();
    virtual bool VPostRestore();
//    virtual void VCalcLighting(Lights *lights, int maximumLights) { }

    // These three functions are done for each shader, not as a part of beginning the render - so they do nothing in D3D11.
    virtual void VSetWorldTransform(const Mat4x4 *m) { Q_UNUSED(m) }
    virtual void VSetViewTransform(const Mat4x4 *m) { Q_UNUSED(m) }
    virtual void VSetProjectionTransform(const Mat4x4 *m) {Q_UNUSED(m)  }

    virtual void VDrawLine(const Vec3& from,const Vec3& to,const Color& color);

    virtual QSharedPointer<IRenderState> VPrepareAlphaPass();
    virtual QSharedPointer<IRenderState> VPrepareSkyBoxPass();

    bool CompileShader(const char *source, QOpenGLShader *shader);
    bool CompileShaderFromFile( std::wstring szFileName, QOpenGLShader *shader );

    // You should leave this global - it does wacky things otherwise.
//    static CDialogResourceManager g_DialogResourceManager;
//	static CDXUTTextHelper* g_pTextHelper;

signals:
    void updateFrame();

protected:
    float m_backgroundColor[4];
    void resizeEvent(QResizeEvent ev);
    bool event(QEvent *event) override;

    GLLineDrawer *m_pLineDrawer;


private:
    QOpenGLContext* context;
    QOpenGLShaderProgram *m_program;
    QOpenGLVertexArrayObject m_object;
    Mat4x4 *model;
    Mat4x4 *view;
    Mat4x4 *projection;

    int vertexLocation;
    int colorLocation;
    int texCoordLocation;
    int uniformModelLocation;
    int uniformViewLocation;
    int uniformProjectionLocation;
    int uniformSamplerLocation;
};

#endif // OPENGLRENDERWINDOW_H
