#pragma once
#include "../ecs/Component.h"

class Transform;

struct DeAcceleration : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_DEACCELERATION)

	DeAcceleration();
	DeAcceleration(float deAcc);
	virtual ~DeAcceleration();
	float deAcc_;
};

