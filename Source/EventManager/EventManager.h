
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H


/** public includes files **/

#include "IEventManager.h"
#include "SignalDelegate.h"
#include <QMap>

/******************************************************************************
*
* EventManager
*
******************************************************************************/
class EventManager : public IEventManager
{
    typedef QMap<EventType, SignalDelegate*> SignalDelegateMap;

    SignalDelegateMap m_eventSignals;

public:
    explicit EventManager(const QString &name, bool setAsGlobal);
    virtual ~EventManager();

    virtual bool VAddListener(const QObject *listener, const char *slot, const EventType& type);
    virtual bool VRemoveListener(const QObject *listener, const char* slot, const EventType& type);

    virtual bool VTriggerEvent(const IEventDataPtr &pEvent) const;
    virtual void VQueueEvent(const IEventDataPtr &pEvent);
    virtual bool VAbortEvent(const EventType &type, bool allOfType) {return true;}

    virtual bool VUpdate(unsigned long maxMillis) {return true;}

};

#endif // EVENTMANAGER_H

