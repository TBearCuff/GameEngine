#include "GameEngineApp.h"


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
#if 0
    //Check for adequate machine resources
    bool resourceCheck = false;

    while(!resourceCheck)
    {
//        const DWORDLONG physicalRAM = 512 * MEGAunsigned char;
//        const DWORDLONG vitualRAM = 1024 * MEGAunsigned char;
        const DWORDLONG diskSpace = 10 * MEGABYTE;

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

    if(!LoadStrings("English"))
    {
        GCC_ERROR("Failed to load strings");
        return false;
    }

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
//    m_pEventManager = GCC_NEW EventManager("GameApp Event Mgr", true );
#if 0
    if(!m_pEventManager)
    {
        GCC_ERROR("Failed to create EventManager.");
        return false;
    }
#endif
#if 0
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);

    window = new MainWindow();
    window->setTitle(VGetGameTitle());
    window->setIcon(VGetIcon());
    window->setFormat(format);
    window->resize(QSize(800, 600));
    window->show();
//    window->showFullScreen();

    m_gameTimer.start();
    m_bIsRunning = true;
#endif
    return true;
}

