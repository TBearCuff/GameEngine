#ifndef GAMEENGINEAPP_H
#define GAMEENGINEAPP_H

#include <QApplication>
#include <QElapsedTimer>

#include "../MainWindow/MainWindow.h"
#include "BaseGameLogic.h"

class GameEngineApp : public QApplication
{
    MainWindow* window;

public:
    GameEngineApp(int argc, char *argv[]);

    virtual bool InitInstance(int argc, char *argv[]);
//    virtual bool VLoadGame(void);
    BaseGameLogic *m_pGame;
//    virtual BaseGameLogic *VCreateGameAndView()=0;
    BaseGameLogic* GetGameLogic(void) const { return m_pGame; }

    // File and Resource System
    class ResCache *m_ResCache;

    // Event manager
//	EventManager *m_pEventManager;


    bool LoadStrings(QString language);
    QString GetString(QString sID);
protected:
    QMap<QString,QString> m_textResource;
    QMap<QString,unsigned int> m_hotkeys;
    virtual void VRegisterGameEvents(void) {}

private:
    void RegisterEngineEvents(void);

    unsigned int MapCharToKeycode(const char);
    void onGameUpdate();

    QElapsedTimer m_AppElapsedTimer;
    qint64 m_LastTime;
};

#endif // GAMEENGINEAPP_H
