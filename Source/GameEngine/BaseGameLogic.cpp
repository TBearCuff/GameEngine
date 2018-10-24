//#include "gameenginestd.h"


//#include "../Mainloop/initialization.h" // only for game options
//#include "../Mainloop/Process.h"

//#include "../ResourceCache/xmlresourceloader.h"

//#include "../Actors/Actor.h"
//#include "../Actors/ActorFactory.h"
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
}

BaseGameLogic::~BaseGameLogic()
{

}

bool BaseGameLogic::Init()
{

}

void BaseGameLogic::VOnUpdate(float time, float elapsedTime)
{


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
#if 0
    // update all game views
    for (GameViewList::iterator it = m_gameViews.begin(); it != m_gameViews.end(); ++it)
    {
        (*it)->VOnUpdate(deltaMilliseconds);
    }

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
