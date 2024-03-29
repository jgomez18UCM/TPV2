// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class Transform;

struct ShowAtoppositeSide: public ecs::Component {
public:

	__CMPID_DECL__(ecs::_BORDERINTERACTION)

	ShowAtoppositeSide();
	virtual ~ShowAtoppositeSide();
	void initComponent() override;
	void check();
private:
	Transform *tr_;
};

