
#ifndef SIGNALDELEGATE_H
#define SIGNALDELEGATE_H



//#include <QObject>
#include "EventManager.h"

/******************************************************************************
*
* SignalDelegate
*
******************************************************************************/
class SignalDelegate : public QObject
{
    Q_OBJECT
public:
    explicit SignalDelegate(QObject *parent = nullptr);
    unsigned int m_connectionCount;

signals:
    void fireEvent(const IEventDataPtr& pEvent);

public slots:
};

#endif // SIGNALDELEGATE_H

