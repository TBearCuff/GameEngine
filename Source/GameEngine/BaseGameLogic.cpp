//#include "gameenginestd.h"


//#include "../Mainloop/initialization.h" // only for game options
//#include "../Mainloop/Process.h"

//#include "../ResourceCache/xmlresourceloader.h"

//#include "../Actors/Actor.h"
//#include "../Actors/ActorFactory.h"
//#include "../Utilities/string.h"


#include "BaseGameLogic.h"

//========================================================================
//
// BaseGameLogic implementation
//
//========================================================================

BaseGameLogic::BaseGameLogic()
{
    m_State = BGS_Initializing;
}

BaseGameLogic::~BaseGameLogic()
{

}

bool BaseGameLogic::Init()
{

}
