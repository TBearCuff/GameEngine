#ifndef SHADERS_H
#define SHADERS_H

#include <QOpenGLShader>
#include "Geometry.h"
#include "Material.h"

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

//struct ID3DX11Effect;
//struct ID3DX11EffectTechnique;
//struct ID3DX11EffectPass;

class LineDraw_Hlsl_Shader
{
public:
    LineDraw_Hlsl_Shader();
    ~LineDraw_Hlsl_Shader();

//	HRESULT OnRestore(Scene *pScene);
//	HRESULT SetupRender(Scene *pScene);
//	HRESULT SetDiffuse(const std::string& textureName, const Color &color);
//	HRESULT SetTexture(ID3D11ShaderResourceView* const *pDiffuseRV, ID3D11SamplerState * const *ppSamplers);

protected:
//	ID3D11InputLayout*          m_pVertexLayout11;
//	ID3D11Buffer*               m_pcbRenderTargetSize;
//	ID3DX11Effect*				m_pEffect;

//	ID3DX11EffectTechnique*		m_EffectTechnique; // No need to be Release()-d.
//	ID3DX11EffectPass*			m_EffectPass; // No need to be Release()-d.

//	ID3D11Buffer*               m_pcbChangePerFrame;

//	ID3D11Buffer*               m_pcbDiffuseColor;

    std::string					m_textureResource;

};

#endif // SHADERS_H
