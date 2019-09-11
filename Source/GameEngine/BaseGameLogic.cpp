#include "GameEngineStd.h"


//#include "../Mainloop/initialization.h" // only for game options
#include "../MainLoop/Process.h"

//#include "../ResourceCache/xmlresourceloader.h"

//#include "../Actors/Actor.h"
#include "../Actor/ActorFactory.h"
//#include "../Utilities/string.h"


#include "BaseGameLogic.h"

//========================================================================
//
// BaseGameLogic implementation
//
//========================================================================

BaseGameLogic::BaseGameLogic()
{
    m_State = BGS_Initializing;
    m_bProxy = false;
    m_pActorFactory = NULL;
}

BaseGameLogic::~BaseGameLogic()
{

//    SAFE_DELETE(m_pActorFactory);

    // destroy all actors
    for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
        it->data()->Destroy();
    m_actors.clear();
}

bool BaseGameLogic::Init()
{
    m_pActorFactory = VCreateActorFactory();
    return true;
}

void BaseGameLogic::VAddView(QSharedPointer<IGameView> pView, ActorId actorId)
{
    int viewId = static_cast<int>(m_gameViews.size());
    m_gameViews.push_back(pView);
    pView->VOnAttach(viewId, actorId);
    pView->VOnRestore();
}

void BaseGameLogic::VRemoveView(QSharedPointer<IGameView> pView)
{
    m_gameViews.removeOne(pView);
}

StrongActorPtr BaseGameLogic::VCreateActor(const QString &actorResource, QDomElement *overrides, const Mat4x4 *initialTransform, const ActorId serversActorId)
{
    Q_ASSERT(m_pActorFactory);
    if (!m_bProxy && serversActorId != INVALID_ACTOR_ID)
        return StrongActorPtr();

    if (m_bProxy && serversActorId == INVALID_ACTOR_ID)
        return StrongActorPtr();

    StrongActorPtr pActor = m_pActorFactory->CreateActor(actorResource, overrides, initialTransform, serversActorId);
    if (pActor)
    {
        m_actors[pActor->GetId()] = pActor;
        if (!m_bProxy && (m_State==BGS_SpawningPlayersActors || m_State==BGS_Running))
        {
 //           QSharedPointer<EvtData_Request_New_Actor> pNewActor(GCC_NEW EvtData_Request_New_Actor(actorResource, initialTransform, pActor->GetId()));
 //           IEventManager::Get()->VTriggerEvent(pNewActor);
        }
        return pActor;
    }
    else
    {
        // FUTURE WORK: Log error: couldn't create actor
        return StrongActorPtr();
    }
}

void BaseGameLogic::VDestroyActor(const ActorId actorId)
{
    // We need to trigger a synchronous event to ensure that any systems responding to this event can still access a
    // valid actor if need be.  The actor will be destroyed after this.
//    shared_ptr<EvtData_Destroy_Actor> pEvent(GCC_NEW EvtData_Destroy_Actor(actorId));
//    IEventManager::Get()->VTriggerEvent(pEvent);

    auto findIt = m_actors.find(actorId);
    if (findIt != m_actors.end())
    {
        findIt->data()->Destroy();
        m_actors.erase(findIt);
    }
}

WeakActorPtr BaseGameLogic::VGetActor(const ActorId actorId)
{
    ActorMap::iterator findIt = m_actors.find(actorId);
    if (findIt != m_actors.end())
        return findIt.value();
    return WeakActorPtr();
}

void BaseGameLogic::VOnUpdate(float time, float elapsedTime)
{
    int deltaMilliseconds = int(elapsedTime);
    m_Lifetime += elapsedTime;

    switch(m_State)
    {
        case BGS_Initializing:
            // If we get to here we're ready to attach players
            VChangeState(BGS_MainMenu);
            break;

        case BGS_MainMenu:
            break;

        case BGS_LoadingGameEnvironment:
/***
            // [mrmike] This was modified a little from what you see in the book - VLoadGame() is now called from
            //          BaseGameLogic::VChangeState()
            //
            if (!g_pApp->VLoadGame())
            {
                GCC_ERROR("The game failed to load.");
                g_pApp->AbortGame();
            }
***/
            break;

        case BGS_WaitingForPlayersToLoadEnvironment:
//			if (m_ExpectedPlayers + m_ExpectedRemotePlayers <= m_HumanGamesLoaded)
//			{
//				VChangeState(BGS_SpawningPlayersActors);
//			}
            break;

        case BGS_SpawningPlayersActors:
//			VChangeState(BGS_Running);
            break;

        case BGS_WaitingForPlayers:
#if 0
            if (m_ExpectedPlayers + m_ExpectedRemotePlayers == m_HumanPlayersAttached )
            {
                // The server sends us the level name as a part of the login message.
                // We have to wait until it arrives before loading the level
                if (!g_pApp->m_Options.m_Level.empty())
                {
                    VChangeState(BGS_LoadingGameEnvironment);
                }
            }
#endif
            break;

        case BGS_Running:
#if 0
            m_pProcessManager->UpdateProcesses(deltaMilliseconds);

            if(m_pPhysics && !m_bProxy)
            {
                m_pPhysics->VOnUpdate(elapsedTime);
                m_pPhysics->VSyncVisibleScene();
            }
#endif
            break;

        default:
//            GCC_ERROR("Unrecognized state.");
        break;
    }
    // update all game views
    for (GameViewList::iterator it = m_gameViews.begin(); it != m_gameViews.end(); ++it)
    {
        (*it)->VOnUpdate(deltaMilliseconds);
    }

#if 0
    // update game actors
    for (ActorMap::const_iterator it = m_actors.begin(); it != m_actors.end(); ++it)
    {
        it->second->Update(deltaMilliseconds);
    }
#endif
}

void BaseGameLogic::VChangeState(BaseGameState newState)
{
    m_State = newState;
}

WeakProcessPtr BaseGameLogic::AttachProcess(StrongProcessPtr pProcess)
{
    if(m_pProcessManager)
    {
        return m_pProcessManager->AttachProcess(pProcess);
    }
    else
    {
        return WeakProcessPtr();
    }
}

ActorFactory *BaseGameLogic::VCreateActorFactory()
{
    return GCC_NEW ActorFactory;

}
