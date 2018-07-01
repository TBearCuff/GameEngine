#ifndef GAMEENGINEAPP_H
#define GAMEENGINEAPP_H

#include <QApplication>
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

protected:
    QMap<QString,QString> m_textResource;
    virtual void VRegisterGameEvents(void) {}

private:
    void RegisterEngineEvents(void);

};

#endif // GAMEENGINEAPP_H
