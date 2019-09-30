#ifndef ACTORFACTORY_H
#define ACTORFACTORY_H

#include "GameEngineStd.h"

class ActorFactory
{
    ActorId m_lastActorId;

protected:
    GenericObjectFactory<ActorComponent, ComponentId> m_componentFactory;

public:
    ActorFactory(void);

    StrongActorPtr CreateActor(QString actorResource, QDomElement overrides, const Mat4x4 *pInitialTransform, const ActorId serversActorId );
//    void ModifyActor(StrongActorPtr pActor, XMLElement* overrides);

//protected:
    // [rez] This function can be overridden by a subclass so you can create game-specific C++ components.  If you do
    // this, make sure you call the base-class version first.  If it returns NULL, you know it's not an engine component.
    virtual StrongActorComponentPtr VCreateComponent(QDomElement pData);

private:
    ActorId GetNextActorId(void) { ++m_lastActorId; return m_lastActorId; }
};

#endif // ACTORFACTORY_H
