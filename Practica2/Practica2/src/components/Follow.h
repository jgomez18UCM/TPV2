#pragma once
#include "../ecs/Component.h"

class Transform;

struct Follow : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_FOLLOW)
	Follow();
	~Follow();
	void initComponent() override;
	void update() override;
protected:
	Transform* tr_;
	Transform* player_;
};

