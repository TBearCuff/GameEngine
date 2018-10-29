
#include "gameenginestd.h"

#include "Actor.h"
#include "ActorComponent.h"
#include "../Utilities/string.h"

Actor::Actor(ActorId id)
{
    m_id = id;
    m_type = "Unknown";

    //added post press - this is an editor helper
    m_resource = "Unknown";
}

Actor::~Actor()
{
    GCC_LOG("Actor", std::string("Destroying Actor ") + std::to_string(m_id));
    GCC_ASSERT(m_components.empty()); //if this assert fires, the actor was destroyed without calling Actor::Destroy()
}

bool Actor::Init(XMLElement* pData)
{
    GCC_LOG("Actor", std::string("Initializing Actor ") + std::to_string(m_id));

    m_type = pData->Attribute("type");
    m_resource = pData->Attribute("resource");
    return true;
}

void Actor::PostInit(void)
{
    for(ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
    {
        it->second->VPostInit();
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
        it->second->VUpdate(deltaMs);
    }
}

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

void Actor::AddComponent(StrongActorComponentPtr pComponent)
{
    std::pair<ActorComponents::iterator, bool> success = m_components.insert(std::make_pair(pComponent->VGetId(), pComponent));
    GCC_ASSERT(success.second);
}
