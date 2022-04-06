#pragma once
#include "../ecs/Component.h"

class Transform;

struct DisableOnExit : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_BORDERINTERACTION)
	DisableOnExit();
	~DisableOnExit();
	void initComponent() override;
	void update() override;
protected:
	Transform* tr_;
};

