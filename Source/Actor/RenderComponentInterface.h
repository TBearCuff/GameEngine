#ifndef RENDERCOMPONENTINTERFACE_H
#define RENDERCOMPONENTINTERFACE_H

#include "ActorComponent.h"

class RenderComponentInterface : public ActorComponent
{
public:
    //RenderComponentInterface
    virtual QSharedPointer<SceneNode> VGetSceneNode(void) = 0;
};

#endif // RENDERCOMPONENTINTERFACE_H
