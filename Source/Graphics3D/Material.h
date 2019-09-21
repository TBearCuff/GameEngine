#ifndef MATERIAL_H
#define MATERIAL_H

#include "Geometry.h"
#include "../ResourceCache/ResCache.h"
#include <QOpenGLTexture>

struct MaterialProperties
{
    Vec4 m_Ambient;
    Vec4 m_Diffuse;
    Vec4 m_Specular;
    float m_Shininess;
};


class Material
{
    MaterialProperties m_Properties;
public:
    Material();
    void SetAmbient(const Color &color);
    const Color GetAmbient() { return m_Properties.m_Ambient; }

    void SetDiffuse(const Color &color);
    const Color GetDiffuse() { return m_Properties.m_Diffuse; }

    void SetSpecular(const Color &color, const float power);
    void GetSpecular(Color &_color, float &_power) {_color = m_Properties.m_Specular; _power = m_Properties.m_Shininess; }

//    void SetEmissive(const Color &color);
//    const Color GetEmissive() { return m_Properties.Emissive; }

    void SetAlpha(const float alpha);
    bool HasAlpha() const { return GetAlpha() != fOPAQUE; }
    float GetAlpha() const { return m_Properties.m_Diffuse.w(); }
};

class GLTextureResourceExtraData : public IResourceExtraData
{
    friend class TextureResourceLoader;

public:
    GLTextureResourceExtraData();
    virtual ~GLTextureResourceExtraData() { SAFE_RELEASE(m_pTexture); }
    virtual QString VToString() { return "GLTextureResourceExtraData"; }
    QOpenGLTexture * const *GetTexture() { return &m_pTexture; }
protected:
    QOpenGLTexture *m_pTexture;

};


//
//  class TextureResourceLoader
//
class TextureResourceLoader : public IResourceLoader
{
public:
    virtual bool VUseRawFile() { return false; }
    virtual bool VDiscardRawBufferAfterLoad() { return true; }
    virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize);
    virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, QSharedPointer<ResHandle> handle);
};

#endif // MATERIAL_H
