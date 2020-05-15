
/** public includes files **/

#include "SignalDelegate.h"

/******************************************************************************
* Function name : SignalDelegate::SignalDelegate
* Description   : Constructor
* Returns       : Nothing
* Inputs        : QObject *parent
* Author        : Troy Cuff for AID (troy.cuff@esterline.com)
******************************************************************************/
SignalDelegate::SignalDelegate(QObject *parent)
    : QObject(parent),
      m_connectionCount(0)
{

}

