
#include "GameEngineStd.h"

//#include "GameEngineApp.h"
#include "../MainLoop/Initialization.h"
#include "BaseGameLogic.h"
#include "../MainWindow/OpenGLRenderWindow.h"
//#include "../EventManager/EventManager.h"
#include "../ResourceCache/ResCache.h"
#include "../ResourceCache/XMLResourceLoader.h"
#include "../UserInterface/UserInterface.h"
#include "../UserInterface/HumanView.h"
#include <QStorageInfo>

#include <QDomDocument>
#include <QTimer>
#include <QStandardPaths>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTouchEvent>

#include "../EventManager/Events.h"

GameEngineApp *g_pApp = NULL;


GameEngineApp::GameEngineApp(int &argc, char **argv) : QApplication(argc, argv),
    m_bIsRunning(false),
    m_bQuitting(false),
    m_bQuitRequested(false),
    m_bIsEditorRunning(true),//Change to false once Assets.zip is ready or ...
    m_pGame(NULL)
{
    //Qt doesn't need this because there is a global instance of QApplication
    g_pApp = this;

    //Initialize member variables here or
//    m_screenSize = Point(0,0);
    m_pEventManager = NULL;

}

bool GameEngineApp::InitInstance(int argc, char *argv[])
{
    //Check for existing instance of the same window
    //I found this in windows code, but is unnecessary for
    //Embedded Linux eglfs because only allows one surface and context
#if 0
#ifndef _DEBUG
    //Note it can be really useful to debug network code to more than one instance of the
    //game up at one time - so feel free to comment these lines in or out as you wish!
    if(!IsOnlyInstance(VGetGameTitle()))
        return false;
#endif
#endif
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    //here is where we whould handle command line args
    //QCommandLineParser


    //We don't need the mouse cursor by default; let the game turn it on
    //Set or unset cursor()
    //checkout qt for embedded linux to research mouse cursor functionality

    //Check for adequate machine resources
    bool resourceCheck = false;

    while(!resourceCheck)
    {
        //how much RAM (is it enough?)
        //what is the processor speed (is it fast enough)

        const qint64 diskSpace = 10 * 1024*1024;

        if(!CheckStorage(diskSpace))
            return false;

//        const DWORD minCPUSpeed = 1300;         //1.3 GHz

        resourceCheck = true;
    }
    //register all events
    RegisterEngineEvents();
    VRegisterGameEvents();

    //Initialize the ResCache
    IResourceFile *zipFile = (m_bIsEditorRunning) ?
            new DevelopmentResourceZipFile("Assets.zip", DevelopmentResourceZipFile::Editor) :
            new ResourceZipFile("Assets.zip");

    m_ResCache = new ResCache(50, zipFile);

    if(!m_ResCache->Init())
    {
//        Q_ERROR("Failed to initialize resource cache! Are the paths set up correctly?");
        return false;
    }

    extern QSharedPointer<IResourceLoader> CreateXmlResourceLoader();

    m_ResCache->RegisterLoader(CreateXmlResourceLoader());

    //Load strings that will be presented to the player (localization)
    if(!LoadStrings("English"))
    {
        qDebug("Failed to load strings");
        return false;
    }
#if 0
    //Bring up Lua State manager now, and run the intial script
    if(!LuaStateManager::Create())
    {
        GCC_ERROR("Failed to intialize Lua");
        return false;
    }

    //Load the preinit file. this is within braces to create a scope and destroy the resource once it's loaded
    //We don't need to do anything with it; we just need to load it
    {
        Resource resource(SCRIPT_PREINIT_FILE);
        shared_ptr<ResHandle> pResourceHandle = m_ResCache->GetHandle(&resource); //loads the XML from the zip file
    }

    ScriptExports::Register();
    ScriptProcess::RegisterScriptClass();
#endif

    // The event manager should be created next so that subsystems can hook in as desired.
    // Discussed in Chapter 5, page 144
    m_pEventManager = new EventManager("GameApp Event Mgr", true );

    if(!m_pEventManager)
    {
        qDebug("Failed to create EventManager.");
        return false;
    }

    //Set the directory for save games and other temporary files
    m_saveGameDirectory = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) +
            VGetGameAppDirectory();
    QDir dir(m_saveGameDirectory);
    if(!dir.exists())
    {
        dir.mkpath(".");
    }
    //Initialize the applications window
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGLES);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,0);

    m_Renderer = QSharedPointer<OpenGLRenderWindow>( new OpenGLRenderWindow());
//    window->setTitle(VGetGameTitle());
//    window->setIcon(VGetIcon());
//    m_screenSize = Point(screenWidth, screenHeight);

    m_Renderer->setFormat(format);
    m_Renderer->resize(QSize(800, 600));
    m_Renderer->show();
    m_Renderer->installEventFilter(this);
//    window->showFullScreen();

    // initialize the directory location you can store save game files


    connect(m_Renderer.data(), &OpenGLRenderWindow::updateFrame, this, &GameEngineApp::onFrameRender);
    m_lastRender = 0;

//    m_Renderer = QSharedPointer<IRenderer>(GCC_NEW GLRenderer());
    m_Renderer->VSetBackgroundColor(0.2f,0.3f,0.3f,1.0f);
    m_Renderer->VOnRestore();


    //create game logic and views
    VCreateGameAndView();
    if(!m_pGame)
        return false;

    //Preload selected resources


    m_AppElapsedTimer.start();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameEngineApp::onGameUpdate);
    timer->start(5);

    m_LastTime = 0;

    connect(this, &QApplication::aboutToQuit, this, &GameEngineApp::OnClose);

    m_bIsRunning = true;
    return true;
}

void GameEngineApp::OnClose()
{
    if(m_pGame != NULL)
        delete m_pGame;

//    if(m_pEventManager != NULL)
//        delete m_pEventManager;

    qDebug() << "I cleaned up my mess; exiting application";
    exit(0);
}

bool GameEngineApp::LoadStrings(QString language)
{
    QString languageFile = QStringLiteral("Assets/Strings/");
    languageFile += language;
    languageFile += QStringLiteral(".xml");

    QDomDocument doc("strings");

    QFile file(languageFile);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    if(!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();

    QDomElement pRoot = doc.documentElement();

    if(!pRoot.isElement())
    {
        qDebug("Strings are missing.");
        return false;
    }

    //Loop through each child element and load the component
    for(QDomNode pElem = pRoot.firstChild(); !pElem.isNull(); pElem = pElem.nextSibling())
    {
        QDomElement e = pElem.toElement();

        QString pKey = e.attribute("id");
        QString pText = e.attribute("value");
        QString pHotkey = e.attribute("hotkey");

        qDebug() << pKey << " " << pText << " " << pHotkey;

        //key and text are not empy strings, add to map
        if(pKey != QString("") && pText != QString(""))
        {
            m_textResource[pKey] = pText;

            //add hotkey if exists
            if(pHotkey != QString(""))
            {
                QByteArray ba = pHotkey.toLocal8Bit();
                m_hotkeys[pKey] = MapCharToKeycode(ba.at(0));
            }
        }
    }
    return true;
}

unsigned int GameEngineApp::MapCharToKeycode(const char pHotkey)
{
    if(pHotkey >= '0' && pHotkey <= '9')
        return 0x30 + pHotkey - '0';

    if(pHotkey >= 'A' && pHotkey <= 'Z')
        return 0x41 + pHotkey - 'A';

    Q_ASSERT(0 && "Platform specific hotkey is not defined!");
    return 0;
}

void GameEngineApp::onGameUpdate()
{
    qint64 elapsedTime = 0;
    if(m_AppElapsedTimer.isValid()) //should always be valid (I think) May be able to remove check
    {
        elapsedTime = m_AppElapsedTimer.elapsed() - m_LastTime;
        m_LastTime += elapsedTime;
    }

    if(m_bQuitting)
    {
        OnClose();
    }

    if(m_pGame)
    {
//        IEventManager::GetGlobal()->VUpdate();

        m_pGame->VOnUpdate(m_LastTime, elapsedTime);
    }


}

void GameEngineApp::onFrameRender()
{
    qint64 elapsedTime = 0;
    if(m_AppElapsedTimer.isValid()) //should always be valid (I think) May be able to remove check
    {
        elapsedTime = m_AppElapsedTimer.elapsed() - m_lastRender;
        m_lastRender += elapsedTime;
    }

    BaseGameLogic *pGame = g_pApp->m_pGame;

    for(GameViewList::iterator i=pGame->m_gameViews.begin(),
        end=pGame->m_gameViews.end(); i!=end; ++i)
    {
        (*i)->VOnRender(m_lastRender, elapsedTime);
    }

//    g_pApp->m_pGame->VRenderDiagnostics();
}

//GameEngineApp::GetString
//
//creates a string from a string resource ID in the string table
//stored in a special DLL, LANG.DLL, so game text string can be
//language independent
//
QString GameEngineApp::GetString(QString sID)
{
    auto localizedString = m_textResource.find(sID);
    if(localizedString == m_textResource.end())
    {
//        GCC_ASSERT(0 && "String not found!");
        return QString("");
    }
    return localizedString.value();
}

qint64 GameEngineApp::GetClockTick()
{
    return m_AppElapsedTimer.elapsed();
}

bool GameEngineApp::eventFilter(QObject *obj, QEvent *ev)
{
    AppMsg msg;
    bool processInput = false;

    switch (ev->type())
    {
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    {
        QMouseEvent *me = static_cast<QMouseEvent*>( ev );
        msg.m_uMsg = ev->type();
        msg.m_param[0] = me->x();
        msg.m_param[1] = me->y();
        msg.m_param[2] = me->button();
        processInput = true;
        qDebug() << ev->type() << me->x() << ", " << me->y();


        break;
    }
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    {
        QKeyEvent *ke = static_cast<QKeyEvent*>(ev);
        msg.m_uMsg = ev->type();
        msg.m_param[0] = ke->key();
        processInput = true;
        qDebug() << ev->type() << " " << QString("%1").arg(ke->key(),0,16);

        break;
    }
    case QEvent::TouchBegin:
    case QEvent::TouchEnd:
    case QEvent::TouchUpdate:
    case QEvent::TouchCancel:
    {
        QTouchEvent *te = static_cast<QTouchEvent*>(ev);
        te->accept();
        qDebug() << ev->type();

        break;
    }
    default:
        break;
    }

    if(processInput)
    {
        //
        // See Chapter 10, page 278 for more explanation of this code.
        //
        if (g_pApp->m_pGame)
        {
            bool result = false;
            BaseGameLogic *pGame = g_pApp->m_pGame;
            // Note the reverse order! User input is grabbed first from the view that is on top,
            // which is the last one in the list.
//            AppMsg msg;
//            msg.m_hWnd = hWnd;
//            msg.m_uMsg = uMsg;
//            msg.m_wParam = wParam;
//            msg.m_lParam = lParam;
            for(GameViewList::iterator i=pGame->m_gameViews.end()-1; i!=pGame->m_gameViews.begin(); --i)
            {
                if ( (*i)->VOnMsgProc( msg ) )
                {
                    result = true;
                    break;				// WARNING! This breaks out of the for loop.
                }
            }
            return result;
        }

    }
    return QApplication::eventFilter(obj,ev);
}



void GameEngineApp::RegisterEngineEvents()
{

}

