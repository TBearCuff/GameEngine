
#ifndef EVENTMANAGERIMPL_H
#define EVENTMANAGERIMPL_H
#include "EventManager.h"
#include "SignalDelegate.h"

const int EVENTMANAGER_NUM_QUEUES = 2;
typedef QMap<EventType, SignalDelegate*> SignalDelegateMap;

/******************************************************************************
*
* EventManager
*
******************************************************************************/
class EventManager : public IEventManager
{
    SignalDelegateMap m_eventSignals;

    typedef QList<IEventDataPtr> EventQueue;

    int m_activeQueue;  // index of actively processing queue; events enque to the opposing queue
    EventQueue m_queues[EVENTMANAGER_NUM_QUEUES];

public:
    explicit EventManager(const QString &name, bool setAsGlobal);
    virtual ~EventManager();

    virtual bool VAddListener(const QObject *listener, const char *slot, const EventType& type);
    virtual bool VRemoveListener(const QObject *listener, const char* slot, const EventType& type);

    virtual bool VTriggerEvent(const IEventDataPtr &pEvent) const;
    virtual bool VQueueEvent(const IEventDataPtr &pEvent);
    virtual bool VAbortEvent(const EventType &, bool );

    virtual bool VUpdate(unsigned long maxMillis = 0xFFFFFFFF);

};

#endif //EVENTMANAGERIMPL_H
