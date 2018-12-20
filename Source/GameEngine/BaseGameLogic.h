#ifndef BASEGAMELOGIC_H
#define BASEGAMELOGIC_H


#include "../MainLoop/ProcessManager.h"

//#include "../EventManager/EventManager.h"
#include "../Actor/Actor.h"
//#include <QVector>
#include "interfaces.h"
#include "../Graphics3D/Geometry.h"

class PathingGraph;
class ActorFactory;
class LevelManager;

enum BaseGameState
{
    BGS_Invalid,
    BGS_Initializing,
    BGS_MainMenu,
    BGS_WaitingForPlayers,
    BGS_LoadingGameEnvironment,
    BGS_WaitingForPlayersToLoadEnvironment,
    BGS_SpawningPlayersActors,
    BGS_Running
};

typedef QMap<ActorId, StrongActorPtr> ActorMap;
typedef QString Level;

class LevelManager
{
public:
    const QVector<Level> &GetLevels() const { return m_Levels; }
    const int GetCurrentLevel () const { return m_CurrentLevel; }
    bool Initialize(QVector<QString> &levels);

protected:
    QVector<Level> m_Levels;
    int m_CurrentLevel;
};

class BaseGameLogic : public IGameLogic
{
    friend class GameEngineApp;       //This is only to gain access to view list

protected:
//    float m_Lifetime;   //indicates how long this game has been in session
    ProcessManager* m_pProcessManager;  //a game logic entity
//    GCCRandom m_random;     //our RNG
    ActorMap m_actors;
    ActorId m_LastActorId;
    BaseGameState m_State;  //game state: loading, running etc
//    int m_ExpectedPlayers;  //how many local human players
//    int m_ExpectedRemotePlayers;    //expected remote human players
//    int m_ExpectedAI;   //how many AI players
//    int m_HumanPlayersAttached;
//    int m_AIPlayersAttached;
//    int m_HumanGamesLoaded;
    GameViewList m_gameViews;       //views that are attached to our game
//    shared_ptr<PathingGraph> m_pPathingGraph;   //the pathing graph
    ActorFactory* m_pActorFactory;

    bool m_bProxy;      //set if this is a proxy game logic, not a real one
//    int m_remotePlayerId;       //if we are a remote player - what is our socket number on the server

//    bool m_RenderDiagnostics;       //Are we rendering diagnostics

//    shared_ptr<IGamePhysics> m_pPhysics;

    LevelManager* m_pLevelManager;      //Manages loading and chaining levels

public:
    BaseGameLogic();
    virtual ~BaseGameLogic();
    bool Init(void);

    void SetProxy(bool isProxy)
    {
        m_bProxy = isProxy;
    }
    const bool IsProxy() const { return m_bProxy; }

    // FUTURE WORK - Perhaps the scripts can have a marker or even a special place in the resource file for any scripts that can run on remote clients
//    const bool CanRunLua() const { return !IsProxy() || GetState()!=BGS_Running; }

    ActorId GetNewActorId( void )
    {
        return ++m_LastActorId;
    }

//    shared_ptr<PathingGraph> GetPathingGraph(void) { return m_pPathingGraph; }
//    GCCRandom& GetRNG(void) { return m_random; }

    virtual void VAddView(QSharedPointer<IGameView> pView, ActorId actorId = INVALID_ACTOR_ID);
    virtual void VRemoveView(QSharedPointer<IGameView> pView);

    virtual StrongActorPtr VCreateActor(const QString &actorResource, QDomElement *overrides, const Mat4x4* initialTransform=NULL, const ActorId serversActorId=INVALID_ACTOR_ID);  //don't store this strong pointer outside of this class
    virtual void VDestroyActor(const ActorId actorId);
    virtual WeakActorPtr VGetActor(const ActorId id);
    virtual void VMoveActor(const ActorId, Mat4x4 const &) {}

    //save VModifyActor for Game Editor
    //editor functions
//    std::string GetActorXml(const ActorId id);
//    virtual void VModifyActor(const ActorId actorId, XMLElement *overrides);



    //Level management
//    const LevelManager* GetLevelManager() { return m_pLevelManager; }
//    virtual bool VLoadGame(const char *levelResource) override; //subclasses shouldn't override this function; use VLoadGameDelegate() instead
//    virtual void VSetProxy();

    //Logic update
    virtual void VOnUpdate(float time, float elapsedTime);

    //Changing Game Logic State
    virtual void VChangeState(BaseGameState newState);
    const BaseGameState GetState() const { return m_State; }

    //Render Diagnostics
//    void ToggleRenderDiagnostices() { m_RenderDiagnostics = !m_RenderDiagnostics; }
//    virtual void VRenderDiagnostics();
//    virtual shared_ptr<IGamePhysics> VGetGamePhysics() { return m_pPhysics; }

    WeakProcessPtr AttachProcess(StrongProcessPtr pProcess);

    //event delegates
//    void RequestDestroyActorDelegate(IEventDataPtr pEventData);

protected:
    virtual ActorFactory* VCreateActorFactory(void);

    //Override this function to do any game-specific loading.
//    virtual bool VLoadGameDelegate(XMLElement* pLevelData) { return true; }

//    void MoveActorDelegate(IEventDataPtr pEventData);
//    void RequestNewActorDelegate(IEventDataPtr pEventData);
};

#endif // BASEGAMELOGIC_H
