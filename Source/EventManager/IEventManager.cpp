
#include "IEventManager.h"
#include <QDebug>

static IEventManager* g_pEventMgr = NULL;


IEventManager::IEventManager(const QString &name, bool setAsGlobal)
{
    if(setAsGlobal)
    {
        if(g_pEventMgr)
        {
            qDebug() << "Attempting to create two global event managers! The old one will be overwritten.";
            delete g_pEventMgr;
        }

        g_pEventMgr = this;
    }
}

IEventManager::~IEventManager()
{
    if(g_pEventMgr == this)
        g_pEventMgr = NULL;
}

IEventManager *IEventManager::Get()
{
    Q_ASSERT(g_pEventMgr);
    return g_pEventMgr;
}

