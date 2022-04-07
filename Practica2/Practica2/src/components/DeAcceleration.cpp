#include "DeAcceleration.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

DeAcceleration::DeAcceleration() : deAcc_(0.995f)
{
}

DeAcceleration::DeAcceleration(float deAcc) : deAcc_(deAcc)
{
}

DeAcceleration::~DeAcceleration()	
{
}


