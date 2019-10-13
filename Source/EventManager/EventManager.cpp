

/** public includes files **/

#include "GameEngineStd.h"
#include "EventManager.h"
#include "IEventData.h"
#include <QDebug>


EventManager::EventManager(const QString &name, bool setAsGlobal) : IEventManager(name, setAsGlobal)
{

}

EventManager::~EventManager()
{

}

bool EventManager::VAddListener(const QObject *listener, const char* slot, const EventType& type)
{
//    qDebug() << "Events: Attempting to add listener delegate to event type: " << type;

    if(!m_eventSignals.contains(type))
    {
        m_eventSignals[type] = new SignalDelegate();
//        qDebug() << "Events: First listener for event type: " << type;
    }

    SignalDelegate *sd = m_eventSignals[type];

//    qDebug() << "Events: Connecting listener to signal delegate";

    QObject::connect(sd, SIGNAL(fireEvent(IEventDataPtr)) , listener, slot, Qt::UniqueConnection);
    sd->m_connectionCount++;

    return true;
}

bool EventManager::VRemoveListener(const QObject *listener, const char *slot, const EventType &type)
{
//    qDebug() << "Events: Attempting to remove listener delegate to event type: " << type;
    bool processed = false;
    auto findIt = m_eventSignals.find(type);
    if(findIt != m_eventSignals.end())
    {
        SignalDelegate *sd = findIt.value();
        if(sd->m_connectionCount != 0)
        {
            //a connection exists
            if(QObject::disconnect(sd, SIGNAL(fireEvent(IEventDataPtr)), listener, slot))
            {
                sd->m_connectionCount--;
                processed = true;
            }
        }

        if(sd->m_connectionCount == 0)
        {
//            qDebug() << "Events: No more listeners of event type: " << type << "/nErasing signal delegate.";
            m_eventSignals.erase(findIt);
        }
    }
    return processed;
}

bool EventManager::VTriggerEvent(const IEventDataPtr& pEvent) const
{
//    qDebug() << "Events: Attempting to trigger event: " << pEvent->GetName();
    bool processed = false;

    if(m_eventSignals.contains(pEvent->VGetEventType()))
    {
//        qDebug() << "Events: Sending event " << pEvent->GetName() << "to attached slots.";
        SignalDelegate *sd = m_eventSignals[pEvent->VGetEventType()];
        sd->fireEvent(pEvent);

        processed = true;
    }
    return processed;
}

/*******************************************************INCOMPLETE
******************************************************************************/
void EventManager::VQueueEvent(const IEventDataPtr &pEvent)
{
    QSharedPointer<QEvent> pQEvent = qSharedPointerCast<QEvent, IEventData>(pEvent);
    QCoreApplication::instance()->postEvent(QCoreApplication::instance(), pQEvent.data());
}

