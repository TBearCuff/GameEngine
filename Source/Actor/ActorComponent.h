#ifndef ACTORCOMPONENT_H
#define ACTORCOMPONENT_H

#include "interfaces.h"
//#include "../Utilities/string.h"
//#include <cstdint>
//using namespace tinyxml2;

//class XMLElement;

class ActorComponent
{
    friend class ActorFactory;

protected:
    StrongActorPtr m_pOwner;

public:
    virtual ~ActorComponent(void) { m_pOwner.reset(); }

    //These functions are meant to be overridden by the implementation classes of the components.
    virtual bool VInit(QDomElement pData) = 0;
    virtual void VPostInit(void) { }
    virtual void VUpdate(int deltaMs) { }
    virtual void VOnChanged(void) { }    // -added post press?

    //for the editor
//    virtual QDomElement VGenerateXml(XMLDocument &outDoc) = 0;

    //This function should be overridden by the interface class.
    virtual ComponentId VGetId(void) const { return GetIdFromName(VGetName()); }
    virtual const char *VGetName() const = 0;

    static ComponentId GetIdFromName(const QString& componentStr)
    {
        return qHash(componentStr);
//        void* rawId = HashedString::hash_name(componentStr);
//        return reinterpret_cast<intptr_t>(rawId);
    }

private:
    void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};

#endif // ACTORCOMPONENT_H
