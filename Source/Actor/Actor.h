#ifndef ACTOR_H
#define ACTOR_H
//#include "../GameEngineMain/interfaces.h"
#include "ActorComponent.h"

using namespace tinyxml2;

//class XMLElement;
typedef std::string ActorType;

class Actor
{
    friend class ActorFactory;
    friend class ActorComponent;

public:
    typedef std::map<ComponentId, StrongActorComponentPtr> ActorComponents;

private:
    ActorId m_id; //uniques id for the actor
    ActorComponents m_components; //all components this actor has
    ActorType m_type;

    //these were added post press as editor helpers, but will also be great for save game files if we ever make them
    std::string m_resource;   //the XML  file from which this actor was initialized (considered the "Archetype" file)

public:
    explicit Actor(ActorId id);
    ~Actor(void);

    bool Init(XMLElement* pData);
    void PostInit(void);
    void Destroy(void);
    void Update(int deltaMs);

    //editor functions

    std::string ToXML();

    //accessors
    ActorId GetId(void) const { return m_id; }
    ActorType GetType(void) const { return m_type; }

    //templace function for retrieving component
    template <class ComponentType>
    weak_ptr<ComponentType> GetComponent(ComponentId id)
    {
        ActorComponents::iterator findIt = m_components.find(id);
        if(findIt != m_components.end())
        {
            StrongActorComponentPtr pBase(findIt->second);
            shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));  // cast to subclass version of the pointer
            weak_ptr<ComponentType> pWeakSub(pSub);  // convert strong pointer to weak pointer
            return pWeakSub;  //return the weak pointer
        }
        else
        {
            return weak_ptr<ComponentType>();
        }
    }

    template <class ComponentType>
    weak_ptr<ComponentType> GetComponent(const char *name)
    {
        ComponentId id = ActorComponent::GetIdFromName(name);
        ActorComponents::iterator findIt = m_components.find(id);
        if(findIt != m_components.end())
        {
            StrongActorComponentPtr pBase(findIt->second);
            shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));  //cast to subclass version of the pointer
            weak_ptr<ComponentType> pWeakSub(pSub);  //convert strong pointer to weak pointer
            return pWeakSub;
        }
        else
        {
            return weak_ptr<ComponentType>();
        }
    }

    const ActorComponents* GetComponents() { return &m_components; }
    void AddComponent(StrongActorComponentPtr pComponent);
};

#endif // ACTOR_H
