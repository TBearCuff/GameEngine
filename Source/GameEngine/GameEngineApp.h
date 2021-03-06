#ifndef GAMEENGINEAPP_H
#define GAMEENGINEAPP_H

#include <QApplication>
#include <QElapsedTimer>

#include "../MainLoop/Initialization.h"
#include "BaseGameLogic.h"
#include "../Graphics3D/SceneNodes.h"
#include "../UserInterface/UserInterface.h"
#include "../EventManager/EventManagerImpl.h"

class Rect;
class Point;

class GameEngineApp : public QApplication
{
//    MainWindow* window;
protected:
//    bool m_bWindowedMode;					// true if the app is windowed, false if fullscreen
    bool m_bIsRunning;						// true if everything is initialized and the game is in the main loop
    bool m_bQuitting;						// true if the app is running the exit sequence
    bool m_bQuitRequested;					// true if the app should run the exit sequence

    Rect m_rcDesktop;						// current desktop size - not necessarilly the client window size
//    Point m_screenSize;					// game screen size
    bool m_bIsEditorRunning;				// true if the game editor is running


public:
    GameEngineApp(int &argc, char **argv);

    virtual bool InitInstance(int argc, char *argv[]);
//    virtual bool VLoadGame(void);
    BaseGameLogic *m_pGame;
    virtual BaseGameLogic *VCreateGameAndView()=0;
    virtual QString VGetGameAppDirectory()=0;

    // Main loop processing
    void AbortGame() { m_bQuitting = true; }
//	int GetExitCode() { return DXUTGetExitCode(); }
    bool IsRunning() { return m_bIsRunning; }
    void SetQuitting(bool quitting) { m_bQuitting = quitting; }

//    const Point &GetScreenSize()  { return m_screenSize; }
    BaseGameLogic* GetGameLogic(void) const { return m_pGame; }

    QSharedPointer<OpenGLRenderWindow> m_Renderer;

    // File and Resource System
    class ResCache *m_ResCache;

    QString m_saveGameDirectory;


    bool LoadStrings(QString language);
    QString GetString(QString sID);

    qint64 GetClockTick();
protected:
    QMap<QString,QString> m_textResource;
    QMap<QString,unsigned int> m_hotkeys;
    virtual void VRegisterGameEvents(void) {}
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    // Event manager
    EventManager* m_pEventManager;
    void RegisterEngineEvents(void);

    unsigned int MapCharToKeycode(const char);

    QElapsedTimer m_AppElapsedTimer;
    qint64 m_LastTime;
    qint64 m_lastRender;

private slots:
    void OnClose();
    void onGameUpdate();
    void onFrameRender();
};

extern GameEngineApp *g_pApp;

#endif // GAMEENGINEAPP_H
