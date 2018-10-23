#include "GameEngineApp.h"
#include "../MainLoop/Initialization.h"
#include <QStorageInfo>

#include <QDomDocument>
#include <QTimer>


GameEngineApp::GameEngineApp(int argc, char *argv[]) : QApplication(argc, argv)
{
    //Qt doesn't need this because there is a global instance of QApplication
//    g_pApp = this;

    //Initialize member variables here or

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

#if 1
        const qint64 diskSpace = 10 * 1024*1024;

        if(!CheckStorage(diskSpace))
            return false;

//        const DWORD minCPUSpeed = 1300;         //1.3 GHz
#endif

        resourceCheck = true;
    }
    //register all events
    RegisterEngineEvents();
    VRegisterGameEvents();

#if 0
    //Initialize the ResCache
    IResourceFile *zipFile = (m_bIsEditorRunning) ?
            GCC_NEW DevelopmentResourceZipFile("Assets.zip", DevelopmentResourceZipFile::Editor) :
            GCC_NEW ResourceZipFile("Assets.zip");

    m_ResCache = GCC_NEW ResCache(50, zipFile);

    if(!m_ResCache->Init())
    {
        GCC_ERROR("Failed to initialize resource cache! Are the paths set up correctly?");
        return false;
    }

    extern shared_ptr<IResourceLoader> CreateXmlResourceLoader();

    m_ResCache->RegisterLoader(CreateXmlResourceLoader());
#endif
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

    // The event manager should be created next so that subsystems can hook in as desired.
    // Discussed in Chapter 5, page 144
//    m_pEventManager = GCC_NEW EventManager("GameApp Event Mgr", true );

    if(!m_pEventManager)
    {
        GCC_ERROR("Failed to create EventManager.");
        return false;
    }
#endif

    //Initialize the applications window
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGLES);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,0);

    window = new MainWindow();
//    window->setTitle(VGetGameTitle());
//    window->setIcon(VGetIcon());
    window->setFormat(format);
    window->resize(QSize(800, 600));
//    window->show();
//    window->showFullScreen();
#if 0

    //create game logic and views
    VCreateGameAndViews();

    //Set the directory for save games and other temporary files

    //Preload selected resources

#endif

    m_AppElapsedTimer.start();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameEngineApp::onGameUpdate);
    timer->start(500);

//    m_bIsRunning = true;
    return true;
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
    file.close();
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

//    if(m_pCamera)
//    {
////        IEventManager::GetGlobal()->VUpdate();

////        m_pCamera->VOnUpdate(m_LastTime, elapsedTime);
//    }


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



void GameEngineApp::RegisterEngineEvents()
{

}

