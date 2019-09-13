#include "GameEngineStd.h"


#include "HumanView.h"

//#include "../Actors/rendercomponentinterface.h"

//#include "../Graphics3D/GLRenderer.h"
//#include "../Graphics3D/Scene.h"



const unsigned int SCREEN_REFRESH_RATE(1000/60);
const GameViewId gc_InvalidGameViewId = 0xffffffff;

//
// HumanView::HumanView - Chapter 10, page 272
//
HumanView::HumanView(QSharedPointer<IRenderer> renderer)
{
//    InitAudio();

    m_pProcessManager = GCC_NEW ProcessManager;

    m_PointerRadius = 1;  // we assume we are on a mouse enabled machine - if this were a tablet we should detect it here
    m_ViewId = gc_InvalidGameViewId;

    //Added post press for move, new and destroy actor events and others
//    RegisterAllDelegates();
    m_BaseGameState = BGS_Initializing;     //what is the curren game state

    if(renderer)
    {
        //Moved to the HumanView class post press
//        m_pScene.reset(GCC_NEW ScreenElementScene(renderer));

        Frustum frustum;
        frustum.Init(GCC_PI/4.0f, 1.0f, 1.0f, 100.0f);
//        m_pCamera.reset(GCC_NEW CameraNode(&Mat4x4::g_Identity, frustum));
//        GCC_ASSERT(m_pScene && m_pCamera && _T("Out of Memory"));

//        m_pScene->VAddChild(INVALID_ACTOR_ID, m_pCamera);
//        m_pScene->SetCamera(m_pCamera);
    }
}

HumanView::~HumanView()
{
    //RemoveAllDelegates was added post press to handle move, new, and destroy actor events
//    RemoveAllDelegates();

    while(!m_ScreenElements.empty())
    {
        m_ScreenElements.pop_front();
    }

    SAFE_DELETE(m_pProcessManager);

//    SAFE_DELETE(g_pAudio);
}

#if 0
bool HumanView::LoadGame(XMLElement *pLevelData)
{
    //call the delegate method
    return VLoadGameDelegate(pLevelData);
}
#endif
void HumanView::VOnRender(unsigned int uiTime, unsigned int uiElapsedTime)
{
//    m_currTick = g_pApp->timeGetTime();
    if(uiTime == m_lastDraw)
        return;

    //It is time to draw?
    if( m_runFullSpeed || ( (uiTime - m_lastDraw) > SCREEN_REFRESH_RATE) )
    {
        if(g_pApp->m_Renderer->VPreRender())
        {
//            m_ScreenElements.sort(SortBy_SharedPtr_Content<IScreenElement>());

            for(ScreenElementList::iterator i = m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
            {
                if((*i)->VIsVisible() )
                {
                    (*i)->VOnRender(uiTime, uiElapsedTime);
                }
            }

            VRenderText();

            //Let the console render
//            m_Console.Render();

            // record the last successful paint
            m_lastDraw = m_currTick;
        }

        g_pApp->m_Renderer->VPostRender();
    }
}

bool HumanView::VOnRestore()
{
    for(ScreenElementList::iterator i = m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
    {
        if( !(*i)->VOnRestore() ) return false;
    }

    return true;
}

bool HumanView::VOnLostDevice()
{
    for(ScreenElementList::iterator i = m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
    {
        if( !(*i)->VOnLostDevice() ) return false;
    }

    return true;
}

void HumanView::VOnUpdate(unsigned long deltaMilliseconds)
{
    m_pProcessManager->UpdateProcesses(deltaMilliseconds);

//    m_Console.Update(deltaMilliseconds);

    // This section of code was added post-press. It runs through the screenlist
    // and calls VOnUpdate. Some screen elements need to update every frame, one
    // example of this is a 3D scene attached to the human view.
    for(ScreenElementList::iterator i=m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
    {
        (*i)->VOnUpdate(deltaMilliseconds);
    }
}

void HumanView::VPushElement(QSharedPointer<IScreenElement> pElement)
{
    m_ScreenElements.push_front(pElement);
}

bool HumanView::VOnMsgProc(AppMsg msg)
{
    // Iterate through the screen layers first
    // In reverse order since we'll send input messages to the
    // screen on top
    for(ScreenElementList::iterator i=m_ScreenElements.end(); i!=m_ScreenElements.begin(); --i)
    {
        if( (*i)->VIsVisible() )
        {
            if( (*i)->VOnMsgProc( msg ) )
            {
                return true;
            }
        }
    }

    bool result = false;
    switch(msg.m_uMsg)
    {
    case QEvent::KeyPress:
/*        if(m_Console.IsActive())
        {
            //Let the console eat this
        }
        else */if(m_KeyboardHandler)
        {
//            QKeyEvent *ke = static_cast<QKeyEvent *>(msg);
            result = m_KeyboardHandler->VOnKeyDown(msg.m_param[0]);
        }
        break;
    case QEvent::KeyRelease:
/*        if(m_Console.IsActive())
        {
            //Let the console eat this
        }
        else*/ if(m_KeyboardHandler)
        {
//            QKeyEvent *ke = static_cast<QKeyEvent *>(msg);
            result = m_KeyboardHandler->VOnKeyUp(msg.m_param[0]);
        }
        break;
    case QEvent::MouseMove:
        if(m_PointerHandler)
            result = m_PointerHandler->VOnPointerMove(Point(msg.m_param[0],msg.m_param[1]), 1);
        break;
    case QEvent::MouseButtonPress:
        if(m_PointerHandler)
        {
            if(msg.m_param[2] == Qt::LeftButton)
            {
                result = m_PointerHandler->VOnPointerButtonDown(Point(msg.m_param[0],msg.m_param[1]), 1, "PointerLeft");
            }
            else if(msg.m_param[2] == Qt::RightButton)
            {
                result = m_PointerHandler->VOnPointerButtonDown(Point(msg.m_param[0],msg.m_param[1]), 1, "PointerRight");
            }
            else if(msg.m_param[2] == Qt::MidButton)
            {
                result = m_PointerHandler->VOnPointerButtonDown(Point(msg.m_param[0],msg.m_param[1]), 1, "PointerMiddle");
            }
        }
        break;
    case QEvent::MouseButtonRelease:
        if(m_PointerHandler)
        {
            if(msg.m_param[2] == Qt::LeftButton)
            {
                result = m_PointerHandler->VOnPointerButtonUp(Point(msg.m_param[0],msg.m_param[1]), 1, "PointerLeft");
            }
            else if(msg.m_param[2] == Qt::RightButton)
            {
                result = m_PointerHandler->VOnPointerButtonUp(Point(msg.m_param[0],msg.m_param[1]), 1, "PointerRight");
            }
            else if(msg.m_param[2] == Qt::MidButton)
            {
                result = m_PointerHandler->VOnPointerButtonUp(Point(msg.m_param[0],msg.m_param[1]), 1, "PointerMiddle");
            }
        }
        break;
    }

    return result;
}


