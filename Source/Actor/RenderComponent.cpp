#include "GameEngineStd.h"

//#include "../Utilities/String.h"
#include "../EventManager/Events.h"

#include "RenderComponent.h"
#include "TransformComponent.h"

//const char* MeshRenderComponent::g_Name = "MeshRenderComponent";
//const char* SphereRenderComponent::g_Name = "SphereRenderComponent";
//const char* TeapotRenderComponent::g_Name = "TeapotRenderComponent";
const char* GridRenderComponent::g_Name = "GridRenderComponent";
const char* LightRenderComponent::g_Name = "LightRenderComponent";
//const char* SkyRenderComponent::g_Name = "SkyRenderComponent";

//---------------------------------------------------------------------------------------------------------------------
// RenderComponent
//---------------------------------------------------------------------------------------------------------------------
bool BaseRenderComponent::VInit(QDomElement pData)
{
    // color
    QDomElement pColorNode = pData.firstChildElement("Color");
    if (!pColorNode.isNull())
        m_color = LoadColor(pColorNode);

    return VDelegateInit(pData);
}

void BaseRenderComponent::VPostInit()
{
//    QSharedPointer<SceneNode> pSceneNode(VGetSceneNode());
//    QSharedPointer<EvtData_New_Render_Component> pEvent(GCC_NEW EvtData_New_Render_Component(m_pOwner->GetId(), pSceneNode));
//    IEventManager::Get()->VTriggerEvent(pEvent);
}

void BaseRenderComponent::VOnChanged()
{
//    QSharedPointer<Evt_Data_Modified_Render_Component> pEvent(GCC_NEW EvtData_Modified_Render_Component(m_pOwner->GetId()));
//    IEventManager::Get()->VTriggerEvent(pEvent);
}


QSharedPointer<SceneNode> BaseRenderComponent::VGetSceneNode(void)
{
    if (!m_pSceneNode)
        m_pSceneNode = VCreateSceneNode();
    return m_pSceneNode;
}

Color BaseRenderComponent::LoadColor(QDomElement pData)
{
    Color color;

    double r = 1.0;
    double g = 1.0;
    double b = 1.0;
    double a = 1.0;

    r = pData.attribute("r").toDouble();
    g = pData.attribute("g").toDouble();
    b = pData.attribute("b").toDouble();
    a = pData.attribute("a").toDouble();

    color.setX((float)r);
    color.setY((float)g);
    color.setZ((float)b);
    color.setW((float)a);

    return color;
}


//---------------------------------------------------------------------------------------------------------------------
// GridRenderComponent
//---------------------------------------------------------------------------------------------------------------------
GridRenderComponent::GridRenderComponent(void)
{
    m_textureResource = "";
    m_squares = 0;
}

bool GridRenderComponent::VDelegateInit(QDomElement pData)
{
    QDomElement pTexture = pData.firstChildElement("Texture");
    if (!pTexture.isNull())
    {
        m_textureResource = pTexture.firstChild().nodeValue();
    }

    QDomElement pDivision = pData.firstChildElement("Division");
    if (!pDivision.isNull())
    {
        m_squares = pDivision.firstChild().nodeValue().toInt();
    }

    return true;
}

QSharedPointer<SceneNode> GridRenderComponent::VCreateSceneNode()
{
    QSharedPointer<TransformComponent> pTransformComponent = (m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name)).toStrongRef();
    if (pTransformComponent)
    {
        WeakBaseRenderComponentPtr weakThis(this);
        Mat4x4 xForm = pTransformComponent->GetTransform();

        return QSharedPointer<SceneNode>(GCC_NEW Grid(m_pOwner->GetId(), weakThis, &xForm));
    }

    return QSharedPointer<SceneNode>();
}


//---------------------------------------------------------------------------------------------------------------------
// LightRenderComponent
//---------------------------------------------------------------------------------------------------------------------
LightRenderComponent::LightRenderComponent(void)
{
}

bool LightRenderComponent::VDelegateInit(QDomElement pData)
{
    QDomElement pLight = pData.firstChildElement("Light");

    double temp;
    QDomElement  pAttenuationNode = pLight.firstChildElement("Attenuation");
    if(!pAttenuationNode.isNull())
    {
        double temp;
        temp = pAttenuationNode.attribute("const").toDouble();
        m_Props.m_Attenuation[0] = (float)temp;

        temp = pAttenuationNode.attribute("linear").toDouble();
        m_Props.m_Attenuation[1] = (float)temp;

        temp = pAttenuationNode.attribute("exp").toDouble();
        m_Props.m_Attenuation[2] = (float)temp;
    }

    QDomElement pShapeNode = pLight.firstChildElement("Shape");
    if(!pShapeNode.isNull())
    {
        temp = pShapeNode.attribute("range").toDouble();
        m_Props.m_Range = (float)temp;
        temp = pShapeNode.attribute("falloff").toDouble();
        m_Props.m_Falloff = (float)temp;
        temp = pShapeNode.attribute("theta").toDouble();
        m_Props.m_Theta = (float)temp;
       temp =  pShapeNode.attribute("phi").toDouble();
        m_Props.m_Phi = (float)temp;
    }
    return true;
}

QSharedPointer<SceneNode> LightRenderComponent::VCreateSceneNode()
{
    QSharedPointer<TransformComponent> pTransformComponent = QSharedPointer<TransformComponent>(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if(pTransformComponent)
    {
        WeakBaseRenderComponentPtr weakThis(this);
        Mat4x4 xForm = pTransformComponent->GetTransform();
        return QSharedPointer<SceneNode>(GCC_NEW GLLightNode(m_pOwner->GetId(), weakThis, m_Props, &xForm));
    }
    return QSharedPointer<SceneNode>();
}

