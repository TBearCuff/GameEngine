#ifndef GAMEENGINEAPP_H
#define GAMEENGINEAPP_H

#include <QApplication>
#include "../MainWindow/MainWindow.h"

class GameEngineApp : public QApplication
{
    MainWindow* window;

public:
    GameEngineApp(int argc, char *argv[]);

    virtual bool InitInstance(int argc, char *argv[]);
//    virtual bool VLoadGame(void);

protected:
    virtual void VRegisterGameEvents(void) {}

private:
    void RegisterEngineEvents(void);

};

#endif // GAMEENGINEAPP_H
