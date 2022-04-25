// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

struct Transform;

class BallSystem: public ecs::System {
public:
	__SYSID_DECL__(ecs::_sys_BALL)

	void recieve(const Message&) override;
	BallSystem();
	virtual ~BallSystem();

	void changeBallPos(float x, float y);
	void changeBallVel(float x, float y);

	void initSystem() override;
	void update() override;
private:
	void resetBall();
	void moveBall();

	Transform *ballTr_;
};

