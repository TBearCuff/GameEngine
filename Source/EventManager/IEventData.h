
#ifndef IEVENTDATA_H
#define IEVENTDATA_H



#include <QString>
#include <QEvent>
#include <QTextStream>
#include <QSharedPointer>

//forward class declaration
class IEventData;

typedef unsigned short EventType;
typedef QSharedPointer<IEventData> IEventDataPtr;

/******************************************************************************
*
* IEventData interface
*
******************************************************************************/
class IEventData : public QEvent
{

public:
    IEventData(QEvent::Type type) : QEvent(type) {}
    ~IEventData() {}

    virtual EventType VGetEventType() const { return type(); }
    virtual float VGetTimeStamp() const = 0;

    //QTextStreams are human readable if intercepted
    //if privacy of the event content is important, think about using QDataStream instead
    virtual void VSerialize(QTextStream& out) const = 0;
    virtual void VDeserialize(QTextStream& in) = 0;

    virtual IEventDataPtr VCopy() const = 0;
    virtual QString GetName() const = 0;

    bool fireEvent(const IEventDataPtr& pEvent);
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

    float VGetTimeStamp() const { return m_timeStamp; }

    //Serializing for network out event
    virtual void VSerialize(QTextStream& ) const { }
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

#endif // IEVENTDATA_H

