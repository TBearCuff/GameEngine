#include "GameEngineStd.h"

#include "Material.h"

Material::Material()
{
    m_Properties.m_Diffuse = g_White;
    m_Properties.m_Ambient = Color(0.10f, 0.10f, 0.10f, 1.0f);
    m_Properties.m_Specular = g_White;
    m_Properties.m_Shininess = 0;
}

void Material::SetAmbient(const Color &color)
{
    m_Properties.m_Ambient = color;
}

void Material::SetDiffuse(const Color &color)
{
    m_Properties.m_Diffuse = color;
}

void Material::SetSpecular(const Color &color, const float power)
{
    m_Properties.m_Specular = color;
    m_Properties.m_Shininess = power;
}


void Material::SetAlpha(const float alpha)
{
    m_Properties.m_Diffuse.setW(alpha);
}

GLTextureResourceExtraData::GLTextureResourceExtraData() : m_pTexture(NULL)
{

}

unsigned int TextureResourceLoader::VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)
{
    Q_UNUSED(rawBuffer);
    Q_UNUSED(rawSize);
    // This will keep the resource cache from allocating memory for the texture, so OpenGL can manage it on it's own.
    return 0;
}

bool TextureResourceLoader::VLoadResource(char *rawBuffer, unsigned int rawSize, QSharedPointer<ResHandle> handle)
{
    Q_UNUSED(handle);
    QSharedPointer<GLTextureResourceExtraData> extra = QSharedPointer<GLTextureResourceExtraData>(GCC_NEW GLTextureResourceExtraData());
    QImage texture;
    if(texture.loadFromData((unsigned char *)rawBuffer, rawSize))
    {
        extra->m_pTexture = GCC_NEW QOpenGLTexture(texture);
    }
    else
    {
        return false;
    }

    //TODO: Address sampler code here?
    extra->m_pTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    extra->m_pTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    extra->m_pTexture->setWrapMode(QOpenGLTexture::Repeat);
    extra->m_pTexture->setComparisonFunction(QOpenGLTexture::CompareNever);
    //TODO: Experiment with various levels of detail
    extra->m_pTexture->setMinimumLevelOfDetail(0);

    handle->SetExtra(QSharedPointer<GLTextureResourceExtraData>(extra));
    return true;
}


