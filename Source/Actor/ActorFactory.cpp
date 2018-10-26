
//#include "gameenginestd.h"
#include "ActorFactory.h"
//#include "../ResourceCache/xmlresourceloader.h"
//#include "ActorComponent.h"
//#include "RenderComponent.h"
//#include "Actor.h"      //until we have actual actors...

//#include "../Utilities/string.h"

ActorFactory::ActorFactory(void)
{
    m_lastActorId = INVALID_ACTOR_ID;
}

StrongActorPtr ActorFactory::CreateActor(QString actorResource, /*XMLElement *overrides, const Mat4x4 *pInitialTransform,*/ const ActorId serversActorId)
{
    //Grab the root XML node
    XMLElement* pRoot = XmlResourceLoader::LoadAndReturnXmlElement(actorResource);
    if(!pRoot)
    {
        GCC_ERROR("Failed to create actor from resource: " + std::string(actorResource));
        return StrongActorPtr();
    }

    //create the actor instance
    ActorId nextActorId = serversActorId;
    if(nextActorId == INVALID_ACTOR_ID)
    {
        nextActorId = GetNextActorId();
    }
    StrongActorPtr pActor(GCC_NEW Actor(nextActorId));
    if(!pActor->Init(pRoot))
    {
        GCC_ERROR("Failed to initialize actor: " + std::string(actorResource));
        return StrongActorPtr();
    }

    //[TC] In inital code but seems to be unused here
//    bool initialTransformSet = false;

    //Loop through each child element and load the component
    for(XMLElement* pNode = pRoot->FirstChildElement(); pNode; pNode->NextSiblingElement())
    {
        StrongActorComponentPtr pComponent(VCreateComponent(pNode));
        if(pComponent)
        {
            pActor->AddComponent(pComponent);
            pComponent->SetOwner(pActor);
        }
        else
        {
            // If an error occurs, we kill the actor and bail.  We could keep going, but the actor will only be
            // partially complete so it's not worth it.  Note that the pActor instance will be destroyed because it
            // will fall out of scope with nothing else pointing to it.
            return StrongActorPtr();
        }
    }

    if(overrides)
    {
        ModifyActor(pActor, overrides);
    }
//[TC] Until we actually have the TransformComponent
#if 0
    // This is a bit of a hack to get the initial transform of the transform component set before the
    // other components (like PhysicsComponent) read it.
    shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if(pInitialTransform && pTransformComponent)
    {
        pTransformComponent->SetPosition(pInitialTransform->GetPosition());
    }
#endif
    // Now that the actor has been fully created, run the post init phase
    pActor->PostInit();

    return pActor;
}

StrongActorComponentPtr ActorFactory::VCreateComponent(XMLElement *pData)
{
    const char* name = pData->Value();
    StrongActorComponentPtr pComponent(m_componentFactory.Create(ActorComponent::GetIdFromName(name)));

    //initialize component if we found one
    if(pComponent)
    {
        if(!pComponent->VInit(pData))
        {
            GCC_ERROR("Component failed to initialize: " + std::string(name));
            return StrongActorComponentPtr();
        }
    }
    else
    {
        GCC_ERROR("Couldn't find ActorComponent named " + std::string(name));
        return StrongActorComponentPtr();
    }

    // pComponent will be NULL if the component wasn't found.  This isn't necessarily an error since you might have a
    // custom CreateComponent() function in a sub class.
    return pComponent;
}

void ActorFactory::ModifyActor(StrongActorPtr pActor, XMLElement *overrides)
{
    //loop through each child element and load the component
    for(XMLElement* pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
    {
        ComponentId componentId = ActorComponent::GetIdFromName(pNode->Value());
        StrongActorComponentPtr pComponent = MakeStrongPtr(pActor->GetComponent<ActorComponent>(componentId));
        if(pComponent)
        {
            pComponent->VInit(pNode);
            // [mrmike] - added post press to ensure that components that need it have
            //            Events generated that can notify subsystems when changes happen.
            //            This was done to have SceneNode derived classes respond to RenderComponent
            //            changes.

            pComponent->VOnChanged();
        }
        else
        {
            pComponent = VCreateComponent(pNode);
            if(pComponent)
            {
                pActor->AddComponent(pComponent);
                pComponent->SetOwner(pActor);
            }
        }
    }
}
