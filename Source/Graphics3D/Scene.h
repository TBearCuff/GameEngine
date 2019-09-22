#ifndef SCENE_H
#define SCENE_H

#include "Geometry.h"
#include "SceneNodes.h"
#include <QStack>
#include "../MainWindow/OpenGLRenderWindow.h"

// Forward declarations
////////////////////////////////////////////////////
//
// SceneActorMap Description				- Chapter 16, page Y
//
//   An STL map that allows fast lookup of a scene node given an ActorId.
//
////////////////////////////////////////////////////

typedef QMap<ActorId, QSharedPointer<ISceneNode> > SceneActorMap;


////////////////////////////////////////////////////
//
// Scene Description						- Chapter 16, page 536
//
// A heirarchical container of scene nodes, which
// are classes that implemente the ISceneNode interface
//
////////////////////////////////////////////////////

class CameraNode;
class SkyNode;
class LightNode;
class LightManager;

class Scene
{
protected:
    QSharedPointer<SceneNode>       m_Root;
    QSharedPointer<CameraNode>      m_Camera;
    QSharedPointer<OpenGLRenderWindow>       m_Renderer;

    QStack<Mat4x4>           		*m_MatrixStack;

    AlphaSceneNodes         m_AlphaSceneNodes;
    SceneActorMap           m_ActorMap;

    LightManager            *m_LightManager;

    void RenderAlphaPass();

public:
    Scene(QSharedPointer<OpenGLRenderWindow> renderer);
    virtual ~Scene();

    void OnRender();
    void OnRestore();
    void OnLostDevice();
    void OnUpdate(const int deltaMilliseconds);
    QSharedPointer<ISceneNode> FindActor(ActorId id);
    bool AddChild(ActorId id, QSharedPointer<ISceneNode> kid);
    bool RemoveChild(ActorId id);
    // event delegates
//    void NewRenderComponentDelegate(IEventDataPtr pEventData);
//    void ModifiedRenderComponentDelegate(IEventDataPtr pEventData);			// added post-press!
//    void DestroyActorDelegate(IEventDataPtr pEventData);
//    void MoveActorDelegate(IEventDataPtr pEventData);

    void SetCamera(QSharedPointer<CameraNode> camera) { m_Camera = camera; }
    const QSharedPointer<CameraNode> GetCamera() const { return m_Camera; }


    void PushAndSetMatrix(const Mat4x4 &toWorld)
    {
        // Note this code carefully!!!!! It is COMPLETELY different
        // from some DirectX 9 documentation out there....
        // Scene::PushAndSetMatrix - Chapter 16, page 541

        m_MatrixStack->push(toWorld);
        Mat4x4 mat = m_MatrixStack->top();
        m_Renderer->VSetWorldTransform(&mat);
    }

    void PopMatrix()
    {
        // Scene::PopMatrix - Chapter 16, page 541

        Mat4x4 mat = m_MatrixStack->pop();
        m_Renderer->VSetWorldTransform(&mat);
    }

    const Mat4x4 GetTopMatrix()
    {
        // Scene::GetTopMatrix - Chapter 16, page 541
        return static_cast<const Mat4x4>(m_MatrixStack->top());
    }

    LightManager *GetLightManager() { return m_LightManager; }

    void AddAlphaSceneNode(AlphaSceneNode *asn) { m_AlphaSceneNodes.push_back(asn); }

    bool Pick(RayCast *pRayCast) { return m_Root->VPick(this, pRayCast); }

    QSharedPointer<OpenGLRenderWindow> GetRenderer() { return m_Renderer; }
};

#endif // SCENE_H
