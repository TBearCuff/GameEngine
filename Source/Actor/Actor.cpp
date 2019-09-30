
#include "GameEngineStd.h"

#include "Actor.h"
//#include "ActorComponent.h"
//#include "../Utilities/string.h"

Actor::Actor(ActorId id)
{
    m_id = id;
    m_type = "Unknown";

    //added post press - this is an editor helper
    m_resource = "Unknown";
}

Actor::~Actor()
{
//    GCC_LOG("Actor", std::string("Destroying Actor ") + std::to_string(m_id));
    Q_ASSERT(m_components.empty()); //if this assert fires, the actor was destroyed without calling Actor::Destroy()
}

bool Actor::Init(QDomElement pData)
{
//    GCC_LOG("Actor", std::string("Initializing Actor ") + std::to_string(m_id));
    qDebug() << QString("Actor: Initializing Actor %1").arg(m_id);
    m_type = pData.attribute("type");
    m_resource = pData.attribute("resource");
    return true;
}

void Actor::PostInit(void)
{
    for(ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
    {
        (*it)->VPostInit();
    }
}

void Actor::Destroy()
{
    m_components.clear();
}

void Actor::Update(int deltaMs)
{
    for(ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
    {
        it->data()->VUpdate(deltaMs);
    }
}
#if 0
std::string Actor::ToXML()
{
    XMLDocument outDoc;

    //Actor element
    XMLElement* pActorElement = outDoc.NewElement("Actor");
    pActorElement->SetAttribute("type", m_type.c_str());
    pActorElement->SetAttribute("resource", m_resource.c_str());

    //components
    for(auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        StrongActorComponentPtr pComponent = it->second;
        XMLElement* pComponentElement = pComponent->VGenerateXml(outDoc);
        pActorElement->LinkEndChild(pComponentElement);
    }
    outDoc.InsertFirstChild(pActorElement);
    XMLPrinter printer;
    outDoc.Accept(&printer);

    return printer.CStr();
}
#endif
void Actor::AddComponent(StrongActorComponentPtr pComponent)
{
    m_components[pComponent->VGetId()] = pComponent;
//    GCC_ASSERT(success.second);
}
