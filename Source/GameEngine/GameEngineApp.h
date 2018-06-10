#ifndef GAMEENGINEAPP_H
#define GAMEENGINEAPP_H

#include <QApplication>

class GameEngineApp : public QApplication
{

public:
    GameEngineApp(int argc, char *argv[]);

    virtual bool InitInstance(int argc, char *argv[]);
};

#endif // GAMEENGINEAPP_H
