
#include "GameEngineStd.h"
#include "EventManagerImpl.h"

//---------------------------------------------------------------------------------------------------------------------
// EventManager::EventManager
//---------------------------------------------------------------------------------------------------------------------
EventManager::EventManager(const QString &name, bool setAsGlobal) : IEventManager(name, setAsGlobal)
{
    m_activeQueue = 0;
}

//---------------------------------------------------------------------------------------------------------------------
// EventManager::~EventManager
//---------------------------------------------------------------------------------------------------------------------
EventManager::~EventManager()
{
    //
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VAddListener
//---------------------------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------------
// EventManager::VRemoveListener
//---------------------------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------------
// EventManager::VTriggerEvent
//---------------------------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------------
// EventManager::VQueueEvent
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::VQueueEvent(const IEventDataPtr &pEvent)
{
    Q_ASSERT(m_activeQueue >= 0);
    Q_ASSERT(m_activeQueue < EVENTMANAGER_NUM_QUEUES);

    //make sure the event is valid
    if(!pEvent)
    {
        qDebug() << "Invalid event in VQueueEvent()";
        return false;
    }

    qDebug() << "Events: Attempting to queue event: " << pEvent->GetName();

    if(m_eventSignals.contains(pEvent->VGetEventType()))
    {
        qDebug() << "Events: Successfully queued event: " << pEvent->GetName();
        m_queues[m_activeQueue].push_back(pEvent);
        return true;
    }
    else
    {
        qDebug() << "Skipping event since there are no delegates registered to receive it: " << pEvent->GetName();
        return false;
    }
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VAbortEvent
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::VAbortEvent(const EventType& inType, bool allOfType)
{
    Q_ASSERT(m_activeQueue >= 0);
    Q_ASSERT(m_activeQueue < EVENTMANAGER_NUM_QUEUES);

    bool success = false;

    if(m_eventSignals.contains(inType))
    {
        EventQueue& eventQueue = m_queues[m_activeQueue];
        auto it = eventQueue.begin();
        while (it != eventQueue.end())
        {
            // Removing an item from the queue will invalidate the iterator, so have it point to the next member.  All
            // work inside this loop will be done using thisIt.
            auto thisIt = it;
            ++it;

	        if ((*thisIt)->VGetEventType() == inType)
	        {
		        eventQueue.erase(thisIt);
		        success = true;
		        if (!allOfType)
			        break;
	        }
        }
    }

	return success;
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VTick
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::VUpdate(unsigned long maxMillis)
{
    unsigned long currMs = g_pApp->GetClockTick();
    unsigned long maxMs = ((maxMillis == 0xFFFFFFFF) ? (0xFFFFFFFF) : (currMs + maxMillis));

	// swap active queues and clear the new queue after the swap
    int queueToProcess = m_activeQueue;
	m_activeQueue = (m_activeQueue + 1) % EVENTMANAGER_NUM_QUEUES;
	m_queues[m_activeQueue].clear();

    qDebug() << "Processing the Event Queue #" << queueToProcess << ";" << m_queues[queueToProcess].size() << " events to process.";

	// Process the queue
	while (!m_queues[queueToProcess].empty())
	{
        // pop the front of the queue
		IEventDataPtr pEvent = m_queues[queueToProcess].front();
        m_queues[queueToProcess].pop_front();

        if(m_eventSignals.contains(pEvent->VGetEventType()))
        {
            qDebug() << "Events: Sending event " << pEvent->GetName() << "to attached slots.";
            SignalDelegate *sd = m_eventSignals[pEvent->VGetEventType()];
            sd->fireEvent(pEvent);
        }

        // check to see if time ran out
        currMs = g_pApp->GetClockTick();
        if (maxMillis != 0xFFFFFFFF && currMs >= maxMs)
        {
            qDebug() << "Aborting event processing; time ran out";
			break;
        }
	}
	
	// If we couldn't process all of the events, push the remaining events to the new active queue.
	// Note: To preserve sequencing, go back-to-front, inserting them at the head of the active queue
	bool queueFlushed = (m_queues[queueToProcess].empty());
	if (!queueFlushed)
	{
		while (!m_queues[queueToProcess].empty())
		{
			IEventDataPtr pEvent = m_queues[queueToProcess].back();
			m_queues[queueToProcess].pop_back();
			m_queues[m_activeQueue].push_front(pEvent);
		}
	}
	
	return queueFlushed;
}

