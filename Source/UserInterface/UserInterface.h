#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "../Graphics3D/Scene.h"

enum
{
    //Lower numbers get drawn first
    ZORDER_BACKGROUND,
    ZORDER_LAYER1,
    ZORDER_LAYER2,
    ZORDER_LAYER3,
    ZORDER_TOOLTIP
};

enum MessageBox_Questions
{
    QUESTION_WHERES_THE_CD,
    QUESTION_QUIT_GAME,
};

class FontHandler;
class BaseUI;

// This class is a group of user interface controls.
// It can be modal or modeless.

//
// class BaseUI									- Chapter 10, page 286
//
// This was factored to create a common class that
// implements some of the IScreenElement class common
// to modal/modeless dialogs
//
class BaseUI : public IScreenElement
{
protected:
    int     m_PosX, m_PosY;
    int     m_Width, m_Height;
    optional<int>   m_Result;
    bool    m_bIsVisible;
public:
    BaseUI() { m_bIsVisible = true; m_PosX = m_PosY = 0; m_Width = 100; m_Height = 100; }
    virtual void VOnUpdate(int) { }
    virtual bool vOnLostDevice() { return true; }
    virtual bool VIsVisible() const { return m_bIsVisible; }
    virtual void VSetVisible(bool visible) { m_bIsVisible = visible; }
};

//
// class ScreenElementScene						- Chapter X, page Y
//
class ScreenElementScene : public IScreenElement, public Scene
{
public:
    ScreenElementScene(QSharedPointer<OpenGLRenderWindow> renderer) : Scene(renderer) { }
    virtual ~ScreenElementScene(void)
    {
//        GCC_WARNING("~ScreenElementScene()");
    }

    // IScreenElement Implementation
    virtual void VOnUpdate(int deltaMS) { OnUpdate(deltaMS); }
    virtual bool VOnRestore()
        { OnRestore(); return true; }
    virtual void VOnRender(double fTime, float fElapsedTime)
        {   Q_UNUSED(fTime); Q_UNUSED(fElapsedTime); OnRender(); }
    virtual bool VOnLostDevice()
        { OnLostDevice(); return true; }
    virtual int VGetZOrder() const { return 0; }
    virtual void VSetZOrder(int const zOrder) {Q_UNUSED(zOrder); }

    // Don't handle any messages
    virtual bool VOnMsgProc( AppMsg msg ) { Q_UNUSED(msg); return false; }

    virtual bool VIsVisible() const { return true; }
    virtual void VSetVisible(bool visible) {Q_UNUSED(visible); }
    virtual bool VAddChild(ActorId id, QSharedPointer<ISceneNode> kid) { return Scene::AddChild(id, kid); }
};

#endif // USERINTERFACE_H
