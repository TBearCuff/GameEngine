#ifndef USERINTERFACE_H
#define USERINTERFACE_H

//#include "../Graphics3D/Scene.h"
//#include "GameEngineStd.h" // until Graphics3D/Scene is available


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

#endif // USERINTERFACE_H
