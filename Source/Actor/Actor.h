#ifndef ACTOR_H
#define ACTOR_H
//#include "../GameEngineMain/interfaces.h"
#include "ActorComponent.h" //Add this base clase temporarily to access object


typedef QString ActorType;

class Actor
{
    friend class ActorFactory;
    friend class ActorComponent;

public:
    typedef QMap<ComponentId, StrongActorComponentPtr> ActorComponents;

private:
    ActorId m_id; //uniques id for the actor
    ActorComponents m_components; //all components this actor has
    ActorType m_type;

    //these were added post press as editor helpers, but will also be great for save game files if we ever make them
    QString m_resource;   //the XML  file from which this actor was initialized (considered the "Archetype" file)

public:
    explicit Actor(ActorId id);
    ~Actor(void);

    bool Init(QDomElement pData);
    void PostInit(void);
    void Destroy(void);
    void Update(int deltaMs);

    //editor functions

//    std::string ToXML();

    //accessors
    ActorId GetId(void) const { return m_id; }
    ActorType GetType(void) const { return m_type; }

    //templace function for retrieving component
    template <class ComponentType>
    QWeakPointer<ComponentType> GetComponent(ComponentId id)
    {
        ActorComponents::iterator findIt = m_components.find(id);
        if(findIt != m_components.end())
        {
            StrongActorComponentPtr pBase(findIt->value());
            QSharedPointer<ComponentType> pSub(qSharedPointerCast<ComponentType>(pBase));  // cast to subclass version of the pointer
            QWeakPointer<ComponentType> pWeakSub(pSub);  // convert strong pointer to weak pointer
            return pWeakSub;  //return the weak pointer
        }
        else
        {
            return QWeakPointer<ComponentType>();
        }
    }

    template <class ComponentType>
    QWeakPointer<ComponentType> GetComponent(const char *name)
    {
        ComponentId id = ActorComponent::GetIdFromName(name);
        ActorComponents::iterator findIt = m_components.find(id);
        if(findIt != m_components.end())
        {
            StrongActorComponentPtr pBase(findIt.value());
            QSharedPointer<ComponentType> pSub(qSharedPointerCast<ComponentType>(pBase));  //cast to subclass version of the pointer
            QWeakPointer<ComponentType> pWeakSub(pSub);  //convert strong pointer to weak pointer
            return pWeakSub;
        }
        else
        {
            return QWeakPointer<ComponentType>();
        }
    }

    const ActorComponents* GetComponents() { return &m_components; }
    void AddComponent(StrongActorComponentPtr pComponent);
};

#endif // ACTOR_H
