
#ifndef IEVENTMANAGER_H
#define IEVENTMANAGER_H


/** public includes files **/

#include <QString>
#include <QObject>
#include "IEventData.h"

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
    virtual void VQueueEvent(const IEventDataPtr& pEvent) = 0;

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
    static IEventManager* GetGlobal();

};


#endif // IEVENTMANAGER_H

