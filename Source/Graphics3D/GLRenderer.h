#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

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
    ~GLLineDrawer() { SAFE_RELEASE(m_pVertexBuffer); }

    void DrawLine(const Vec3& from, const Vec3& to, const Color& color);
    void OnRestore();

protected:
    Vec3            m_Verts[2];
//    LineDraw_Hlsl_Shader		m_LineDrawerShader;     //Shaders.h
    QOpenGLBuffer* m_pVertexBuffer;
};

//
// class D3DRenderer11								- Chapter 10, page 270
//
//   The D3DRenderer and D3DRenderer9 classes are not discussed in the book. The D3DRenderer class is designed to
//   implement the IRenderer interface, which abstracts the implentation of the renderer technology, which for this
//   engine can be either D3D9 or D3D11. It also encapsulates the usefulness of CDXUTDialogResourceManager
//   and CDXUTTextHelper for user interface tasks whether D3D9 or D3D11 is being used.
//

class GLRenderer : public IRenderer
{
public:
    GLRenderer() { m_pLineDrawer = NULL; }

//    virtual void VShutdown() { /*SAFE_DELETE(g_pTextHelper);*/ SAFE_DELETE(m_pLineDrawer); }

    virtual void VSetBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB)
    {
        m_backgroundColor[0] = float(bgA) / 255.0f;
        m_backgroundColor[1] = float(bgR) / 255.0f;
        m_backgroundColor[2] = float(bgG) / 255.0f;
        m_backgroundColor[3] = float(bgB) / 255.0f;
    }

    virtual bool VPreRender();
    virtual bool VPostRender();
    virtual void VOnRestore() { }
    virtual void VCalcLighting(Lights *lights, int maximumLights) { }

    // These three functions are done for each shader, not as a part of beginning the render - so they do nothing in D3D11.
    virtual void VSetWorldTransform(const Mat4x4 *m) { }
    virtual void VSetViewTransform(const Mat4x4 *m) { }
    virtual void VSetProjectionTransform(const Mat4x4 *m ) { }

    virtual void VDrawLine(const Vec3& from, const Vec3& to, const Color& color);

//    virtual shared_ptr<IRenderState> VPrepareAlphaPass();
//    virtual shared_ptr<IRenderState> VPrepareSkyBoxPass();

    bool CompileShader(const char *source, QOpenGLShader *shader);
    bool CompileShaderFromFile( std::wstring szFileName, QOpenGLShader *shader );

    // You should leave this global - it does wacky things otherwise.
//    static CDialogResourceManager g_DialogResourceManager;
//	static CDXUTTextHelper* g_pTextHelper;
protected:
    float m_backgroundColor[4];

    GLLineDrawer *m_pLineDrawer;
};

#endif // GLRENDERER_H
