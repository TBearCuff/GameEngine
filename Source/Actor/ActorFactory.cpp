
//#include "gameenginestd.h"
#include "ActorFactory.h"
#include "../ResourceCache/XMLResourceLoader.h"
#include "../Actor/TransformComponent.h"
//#include "ActorComponent.h"
#include "../Actor/RenderComponent.h"
#include "Actor.h"      //until we have actual actors...

//#include "../Utilities/string.h"

ActorFactory::ActorFactory(void)
{
    m_lastActorId = INVALID_ACTOR_ID;


    m_componentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::g_Name));
    m_componentFactory.Register<GridRenderComponent>(ActorComponent::GetIdFromName(GridRenderComponent::g_Name));
}

StrongActorPtr ActorFactory::CreateActor(QString actorResource, QDomElement overrides, const Mat4x4 *pInitialTransform, const ActorId serversActorId)
{
    //Grab the root XML node
    QDomElement pRoot = XmlResourceLoader::LoadAndReturnXmlElement(actorResource);
    if(pRoot.isNull())
    {
//        GCC_ERROR("Failed to create actor from resource: " + actorResource);
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
//        GCC_ERROR("Failed to initialize actor: " + std::string(actorResource));
        return StrongActorPtr();
    }

    //[TC] In inital code but seems to be unused here
//    bool initialTransformSet = false;

    //Loop through each child element and load the component
    for(QDomNode pNode = pRoot.firstChild(); !pNode.isNull(); pNode = pNode.nextSibling())
    {
        QDomElement pElement = pNode.toElement();
        if(pElement.isNull())
        {
            continue;
        }
        StrongActorComponentPtr pComponent(VCreateComponent(pElement));
        if(!pComponent.isNull())
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

    if(!overrides.isNull())
    {
//        ModifyActor(pActor, overrides);
    }
//[TC] Until we actually have the TransformComponent
#if 1
    // This is a bit of a hack to get the initial transform of the transform component set before the
    // other components (like PhysicsComponent) read it.
    QSharedPointer<TransformComponent> pTransformComponent = (pActor->GetComponent<TransformComponent>(TransformComponent::g_Name).toStrongRef());
    if(pInitialTransform && !pTransformComponent.isNull())
    {
        pTransformComponent->SetPosition(pInitialTransform->GetPosition());
    }
#endif
    // Now that the actor has been fully created, run the post init phase
    pActor->PostInit();

    return pActor;
}

StrongActorComponentPtr ActorFactory::VCreateComponent(QDomElement pData)
{
    const QString name = pData.nodeName();
    StrongActorComponentPtr pComponent(m_componentFactory.Create(ActorComponent::GetIdFromName(name)));

    //initialize component if we found one
    if(pComponent)
    {
        if(!pComponent->VInit(pData))
        {
//            GCC_ERROR("Component failed to initialize: " + std::string(name));
            return StrongActorComponentPtr();
        }
    }
    else
    {
//        GCC_ERROR("Couldn't find ActorComponent named " + std::string(name));
        return StrongActorComponentPtr();
    }

    // pComponent will be NULL if the component wasn't found.  This isn't necessarily an error since you might have a
    // custom CreateComponent() function in a sub class.
    return pComponent;
}
#if 0
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
#endif
