#ifndef INTERFACES_H
#define INTERFACES_H

#include <QEvent>
#include <QKeyEvent>
#include <QSharedPointer>

class Actor;
class ActorComponent;

typedef unsigned int ActorId;
typedef unsigned int ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef QSharedPointer<Actor> StrongActorPtr;
typedef QWeakPointer<Actor> WeakActorPtr;
typedef QSharedPointer<ActorComponent> StrongActorComponentPtr;
typedef QWeakPointer<ActorComponent> WeakActorComponentPtr;

#if 1
template<class T>
struct SortBy_SharedPtr_Content
{
    bool operator()(const QSharedPointer<T> &lhs, const QSharedPointer<T> &rhs) const
        { return *lhs < *rhs; }
};
#endif

//
// class IScreenElement							- Chapter 10, page 285
//
class IScreenElement
{
public:
    virtual bool VOnRestore() = 0;
    virtual bool VOnLostDevice() = 0;
    virtual void VOnRender(double fTime, float fElapsedTime) = 0;
    virtual void VOnUpdate(int deltaMilliseconds) = 0;

    virtual int VGetZOrder() const = 0;
    virtual void VSetZOrder(int const zOrder) = 0;
    virtual bool VIsVisible() const = 0;
    virtual void VSetVisible(bool visible) = 0;

    virtual bool VOnMsgProc( AppMsg msg ) = 0;

    virtual ~IScreenElement() { }
    virtual bool const operator <(IScreenElement const &other) { return VGetZOrder() < other.VGetZOrder(); }
};

class IGamePhysics;

class IGameLogic
{
public:
    virtual WeakActorPtr VGetActor(const ActorId id)=0;
//    virtual StrongActorPtr VCreateActor(const QString &actorResource, XMLElement *overrides, const Mat4x4 *initialTransform=NULL, const ActorId serversActorId=INVALID_ACTOR_ID)=0;
//    virtual void VDestroyActor(const ActorId actorId)=0;
    virtual bool VLoadGame(const char* levelResource)=0;
    virtual void VSetProxy()=0;
    virtual void VOnUpdate(float time, float elapsedTime)=0;
//    virtual void VChangeState(enum BaseGameState newState)=0;
//    virtual void VMoveActor(const ActorId id, Mat4x4 const &mat)=0;
//    virtual QSharedPointer<IGamePhysics> VGetGamePhysics(void) = 0;
};

enum GameViewType
{
    GameView_Human,
    GameView_Remote,
    GameView_AI,
    GameView_Recorder,
    GameView_Other
};

typedef unsigned int GameViewId;
extern const GameViewId gc_InvalidGameViewId;

class IGameView
{
public:
    virtual bool VOnRestore()=0;
    virtual void VOnRender(unsigned int uiTime, unsigned int uiElapsedTime)=0;
    virtual bool VOnLostDevice()=0;
    virtual GameViewType VGetType()=0;
    virtual GameViewId VGetId() const=0;
    virtual void VOnAttach(GameViewId vid, ActorId aid)=0;

    virtual bool VOnMsgProc( AppMsg msg ) = 0;
    virtual void VOnUpdate(unsigned long deltaMs)=0;

    virtual ~IGameView() { }
};
#include <QLinkedList>

typedef std::list<QSharedPointer<IScreenElement>> ScreenElementList;
typedef QList<QSharedPointer<IGameView>> GameViewList;


////////////////////////////////////////////////////
//
// IKeyboardHandler Description		- Chapter 9, page 242
// IPointerHandler Description		- Chapter 9, page 242
// IJoystickHandler Description		- Chapter 9, page 242
// IGamepadHandler Description		- Chapter 9, page 242
//
// These are the public APIs for any object that implements reactions
// to events sent by hardware user interface devices.
//
// Note: IJoystickHandler and IGamepadHandler are not currently
//       implemented anywhere in the codebase. They are here
//       as examples, and could require modification to actually work!
//
////////////////////////////////////////////////////
class IKeyboardHandler
{
public:
    virtual bool VOnKeyDown(const unsigned char c) = 0;
    virtual bool VOnKeyUp(const unsigned char c) = 0;
};

class IPointerHandler
{
public:
    virtual bool VOnPointerMove(const Point &pos, const int radius) = 0;
    virtual bool VOnPointerButtonDown(const Point &pos, const int radius, const QString &buttonName) = 0;
    virtual bool VOnPointerButtonUp(const Point &pos, const int radius, const QString &buttonName) = 0;
};

class IJoystickHandler
{
public:
    virtual bool VOnButtonDown(const QString &buttonName, int const pressure)=0;
    virtual bool VOnButtonUp(const QString &buttonName)=0;
    virtual bool VOnJoystick(float const x, float const y)=0;
};

class IGamepadHandler
{
public:
    virtual bool VOnTrigger(const QString &triggerName, float const pressure)=0;
    virtual bool VOnButtonDown(const QString &buttonName, int const pressure)=0;
    virtual bool VOnButtonUp(const QString &buttonName)=0;
    virtual bool VOnDirectionalPad(const QString &direction)=0;
    virtual bool VOnThumbstick(const QString &stickName, float const x, float const y)=0;
};




////////////////////////////////////////////////////
//
// class IResourceFile						- Chapter 8, page 222
// The core of a resource cache system
//
////////////////////////////////////////////////////

class Resource;
class IResourceFile;
class ResHandle;

//
// class IResourceLoader					- Chapter 8, page 224
//
class IResourceLoader
{
public:
    virtual QString VGetPattern()=0;
    virtual bool VUseRawFile()=0;
    virtual bool VDiscardRawBufferAfterLoad()=0;
    virtual bool VAddNullZero() { return false; }
    virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)=0;
    virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, QSharedPointer<ResHandle> handle)=0;
};

class IResourceFile
{
public:
    virtual bool VOpen()=0;
    virtual ~IResourceFile(){}
    virtual int VGetRawResourceSize(const Resource &r)=0;
    virtual int VGetRawResource(const Resource &r, char *buffer)=0;
    virtual int VGetNumResources() const = 0;
    virtual QString VGetResourceName(int num) const = 0;
    virtual bool VIsUsingDevelopmentDirectories(void) const = 0;
};

#if 1
/////////////////////////////////////////////////////////////////////////////
// enum RenderPass							- Chapter 16, page 529
//
//   3D scenes are drawn in passes - this enum defines the render passes
//   supported by the 3D scene graph created by class Scene.
/////////////////////////////////////////////////////////////////////////////

enum RenderPass
{
    RenderPass_0,
    RenderPass_Static = RenderPass_0,
    RenderPass_Actor,
    RenderPass_Sky,
    RenderPass_NotRendered,
    RenderPass_Last
};




class Scene;
class SceneNodeProperties;
class RayCast;
class LightNode;

typedef QList<QSharedPointer<LightNode> > Lights;

#endif

class IRenderState
{
public:
    virtual QString VToString()=0;
};

//
// class IRenderer				- not described in the 4th edition
//
// class IRenderer abstracts the renderer implementation so the engine
//   can use D3D9 or D3D11.
//
class IRenderer
{
public:
    virtual void VSetBackgroundColor(unsigned char bgA, unsigned char bgR, unsigned char bgG, unsigned char bgB)=0;
    virtual bool VOnRestore()=0;
//    virtual void VShutdown()=0;
    virtual bool VPreRender()=0;
    virtual bool VPostRender()=0;
    virtual void VCalcLighting(Lights *lights, int maximumLights)=0;
    virtual void VSetWorldTransform(const Mat4x4 *m)=0;
    virtual void VSetViewTransform(const Mat4x4 *m)=0;
    virtual void VSetProjectionTransform(const Mat4x4 *m)=0;
//    virtual QSharedPointer<IRenderState> VPrepareAlphaPass()=0;
//    virtual QSharedPointer<IRenderState> VPrepareSkyBoxPass()=0;
    virtual void VDrawLine(const Vec3& from, const Vec3& to, const Color& color)=0;
};
/////////////////////////////////////////////////////////////////////////////
// class ISceneNode					- Chapter 16, page 524
//
//   This is the public interface for nodes in a 3D scene graph.
/////////////////////////////////////////////////////////////////////////////

class ISceneNode
{
public:
    virtual const SceneNodeProperties * const VGet() const=0;

    virtual void VSetTransform(const Mat4x4 *toWorld, const Mat4x4 *fromWorld=NULL)=0;

    virtual bool VOnUpdate(Scene *pScene, unsigned long const elapsedMs)=0;
    virtual bool VOnRestore(Scene *pScene)=0;

    virtual bool VPreRender(Scene *pScene)=0;
    virtual bool VIsVisible(Scene *pScene) const=0;
    virtual bool VRender(Scene *pScene)=0;
    virtual bool VRenderChildren(Scene *pScene)=0;
    virtual bool VPostRender(Scene *pScene)=0;

    virtual bool VAddChild(QSharedPointer<ISceneNode> kid)=0;
    virtual bool VRemoveChild(ActorId id)=0;
    virtual bool VOnLostDevice(Scene *pScene)=0;
    virtual bool VPick(Scene *pScene, RayCast *pRayCast)=0;


    virtual ~ISceneNode() { }
};

#if 0


/////////////////////////////////////////////////////////////////////////////
// class IGamePhysics							- Chapter 17, page 589
//
//   The interface defintion for a generic physics API.
/////////////////////////////////////////////////////////////////////////////

class IGamePhysics
{
public:

    // Initialiazation and Maintenance of the Physics World
    virtual bool VInitialize()=0;
    virtual void VSyncVisibleScene() = 0;
    virtual void VOnUpdate( float deltaSeconds ) = 0;

    // Initialization of Physics Objects
    virtual void VAddSphere(float radius, WeakActorPtr actor, /*const Mat4x4& initialTransform, */const std::string& densityStr, const std::string& physicsMaterial)=0;
//    virtual void VAddBox(const Vec3& dimensions, WeakActorPtr gameActor, /*const Mat4x4& initialTransform, */ const std::string& densityStr, const std::string& physicsMaterial) = 0;
//    virtual void VAddPointCloud(Vec3 *verts, int numPoints, WeakActorPtr gameActor, /*const Mat4x4& initialTransform, */ const std::string& densityStr, const std::string& physicsMaterial)=0;
    virtual void VRemoveActor(ActorId id)=0;

    // Debugging
    virtual void VRenderDiagnostics() = 0;

    // Physics world modifiers
//    virtual void VCreateTrigger(WeakActorPtr pGameActor, const Vec3 &pos, const float dim)=0;
//    virtual void VApplyForce(const Vec3 &dir, float newtons, ActorId aid)=0;
//    virtual void VApplyTorque(const Vec3 &dir, float newtons, ActorId aid)=0;
//    virtual bool VKinematicMove(const Mat4x4 &mat, ActorId aid)=0;

    // Physics actor states
    virtual void VRotateY(ActorId actorId, float angleRadians, float time) = 0;
    virtual float VGetOrientationY(ActorId actorId) = 0;
    virtual void VStopActor(ActorId actorId) = 0;
//    virtual Vec3 VGetVelocity(ActorId actorId) = 0;
//    virtual void VSetVelocity(ActorId actorId, const Vec3& vel) = 0;
//    virtual Vec3 VGetAngularVelocity(ActorId actorId) = 0;
//    virtual void VSetAngularVelocity(ActorId actorId, const Vec3& vel) = 0;
//    virtual void VTranslate(ActorId actorId, const Vec3& vec) = 0;

//    virtual void VSetTransform(const ActorId id, const Mat4x4& mat) = 0;
//    virtual Mat4x4 VGetTransform(const ActorId id) = 0;

    virtual ~IGamePhysics() { };
};


/////////////////////////////////////////////////////////////////////////////
// IScriptManager Description
//
//   This is the public interface for the scripting system.
/////////////////////////////////////////////////////////////////////////////
class IScriptManager
{
public:
    virtual ~IScriptManager(void) {}
    virtual bool VInit(void) = 0;
    virtual void VExecuteFile(const char* resource) = 0;
    virtual void VExecuteString(const char* str) = 0;
};
#endif

#endif // INTERFACES_H
