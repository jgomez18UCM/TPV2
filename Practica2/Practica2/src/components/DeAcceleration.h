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
	void initComponent() override;
	void update() override;
private:
	Transform* tr_;
	float deAcc_;
};

