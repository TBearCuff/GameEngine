#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H


#include "RenderComponentInterface.h"
#include "../Graphics3D/SceneNodes.h"
#include "../Graphics3D/Lights.h"

class BaseRenderComponent : public RenderComponentInterface
{
protected:
    Color m_color;
    QSharedPointer<SceneNode> m_pSceneNode;

public:

    virtual bool VInit(QDomElement pData) override;
    virtual void VPostInit(void) override;
    virtual void VOnChanged(void) override;
//    virtual XMLElement* VGenerateXml(XMLDocument &outDoc) override;
    const Color GetColor() const { return m_color; }

protected:
    // loads the SceneNode specific data (represented in the <SceneNode> tag)
    virtual bool VDelegateInit(QDomElement pData) {Q_UNUSED(pData); return true; }
    virtual QSharedPointer<SceneNode> VCreateSceneNode(void) = 0;  // factory method to create the appropriate scene node
    Color LoadColor(QDomElement pData);

    //editor stuff
//    virtual XMLElement* VCreateBaseElement(void) { return GCC_NEW XMLElement(VGetName()); }
//    virtual void VCreateInheritedXmlElements(QDomElement pBaseElement) = 0;

private:
    virtual QSharedPointer<SceneNode> VGetSceneNode(void) override;

};

//---------------------------------------------------------------------------------------------------------------------
// Grids, which represent the world
//---------------------------------------------------------------------------------------------------------------------
class GridRenderComponent : public BaseRenderComponent
{
    QString m_textureResource;
    int m_squares;

public:
    static const char* g_Name;
    virtual const char* VGetName() const { return g_Name; }

    GridRenderComponent(void);
    const QString GetTextureResource() { return m_textureResource; }
    const int GetDivision() { return m_squares; }

protected:
    virtual bool VDelegateInit(QDomElement pData) override;
    virtual QSharedPointer<SceneNode> VCreateSceneNode(void) override;  // factory method to create the appropriate scene node

    // editor stuff
//    virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement);
};


//---------------------------------------------------------------------------------------------------------------------
// Lights
//---------------------------------------------------------------------------------------------------------------------
class LightRenderComponent : public BaseRenderComponent
{
    LightProperties m_Props;

public:
    static const char *g_Name;
    virtual const char *VGetName() const { return g_Name; }

    LightRenderComponent(void);

protected:
    virtual bool VDelegateInit(QDomElement pData) override;
    virtual QSharedPointer<SceneNode> VCreateSceneNode(void) override;  // factory method to create the appropriate scene node

    // editor stuff
//    virtual void VCreateInheritedXmlElements(QDomElement pBaseElement);
};


#endif // RENDERCOMPONENT_H
