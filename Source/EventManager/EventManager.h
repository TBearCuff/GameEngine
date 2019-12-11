#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H


/** public includes files **/

#include <QMap>
#include <QTextStream>
#include <QSharedPointer>
#include <QEvent>
//#include "../Utilities/templates.h"

//---------------------------------------------------------------------------------------------------------------------
// Forward declaration & typedefs
//---------------------------------------------------------------------------------------------------------------------
class IEventData;

typedef unsigned long EventType;
typedef QSharedPointer<IEventData> IEventDataPtr;

//---------------------------------------------------------------------------------------------------------------------
// Macro for event registration
//---------------------------------------------------------------------------------------------------------------------
//extern GenericObjectFactory<IEventData, EventType> g_eventFactory;
//#define REGISTER_EVENT(eventClass) g_eventFactory.Register<eventClass>(eventClass::sk_EventType)
//#define CREATE_EVENT(eventType) g_eventFactory.Create(eventType)


/******************************************************************************
*
* IEventData
*
******************************************************************************/
class IEventData : public QEvent
{

public:
    IEventData(QEvent::Type type) : QEvent(type) {}
    ~IEventData() {}

    virtual EventType VGetEventType() const { return type(); }
    virtual float GetTimeStamp(void) const = 0;
    virtual void VSerialize(QTextStream& out) const = 0;
    virtual void VDeserialize(QTextStream& in) = 0;
    virtual IEventDataPtr VCopy(void) const = 0;
    virtual QString GetName(void) const = 0;

//    bool fireEvent(const IEventDataPtr& pEvent);
};

/******************************************************************************
*
* BaseEventData Abstract Object
*
******************************************************************************/
template<typename T>
class BaseEventData : public IEventData
{
    const float m_timeStamp;
protected:
    static QEvent::Type m_eventType;

public:
    explicit BaseEventData(const float timeStamp = 0.0f)
        : IEventData(RegisteredType()), m_timeStamp(timeStamp){ }
    virtual ~BaseEventData() { }

    float GetTimeStamp(void) const { return m_timeStamp; }

    // Serializing for network input / output
    virtual void VSerialize(QTextStream &) const	{ }
    virtual void VDeserialize(QTextStream& ) { }

    static QEvent::Type RegisteredType()
    {
        if(m_eventType == QEvent::None)
        {
            int generatedType = QEvent::registerEventType();
            m_eventType = static_cast<QEvent::Type>(generatedType);
        }
        return m_eventType;
    }

};

template<typename _T>
QEvent::Type BaseEventData<_T>::m_eventType = QEvent::None;

/******************************************************************************
*
* IEventManager interface
*
******************************************************************************/
class IEventManager
{
public:
    explicit IEventManager(const QString& name, bool setAsGlobal);
    virtual ~IEventManager();

    //Registers a delegate function when that will get called when the event type is triggered.
    //Returns true if successful
    virtual bool VAddListener(const QObject *listener, const char* slot, const EventType& type) = 0;

    //Removes a delegate / event type pairing from the internal tables. Returns false if
    //the pairing was not found
    virtual bool VRemoveListener(const QObject *listener, const char* slot, const EventType& type) = 0;

    //Fire off the event right now. Bypasses queue
    virtual bool VTriggerEvent(const IEventDataPtr& pEvent) const = 0;

    //Fire off event. This uses the queue
    virtual bool VQueueEvent(const IEventDataPtr& pEvent) = 0;

    //Find the next-available instance of the event type and remove it from the processing queue. This
    //may be done up to the point that it is actively being processed; it is safe to happen during event
    //processing itself
    //-If allOfType is true, then all events of that type are cleared from the queue
    //returns true if the event was found and removed, returns false otherwise
    virtual bool VAbortEvent(const EventType& type, bool allOfType = false) = 0;

    //Allow for processing of any queued events, optionally specify a processing time limit so that the
    //event processing does not take too long. Note the danger of using this artificial limiter is that all
    //messages may not get processed. This function may do nothing, if Qt owns the event queue.
    //returns true if all events ready for processing were completed, false otherwise (e.g. timeout)
    virtual bool VUpdate(unsigned long maxMillis = 0xFFFFFFFF) = 0;

    //Getter for the main global event manager. It is not valid to have more than one global event manager.
    static IEventManager* Get();

};



#endif // EVENTMANAGER_H

