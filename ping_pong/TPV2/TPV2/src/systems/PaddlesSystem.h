// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_stdinc.h>
#include <array>

#include "../ecs/System.h"

namespace ecs {
class Entity;
}
struct Transform;
struct PaddleCtrl;
class Vector2D;

class PaddlesSystem: public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_PADDLES)

	PaddlesSystem();
	virtual ~PaddlesSystem();
	void update() override;
	void initSystem() override;

	void changePaddlePos(Uint16 side, float x, float y);
private:
	void movePaddle(ecs::Entity*);
	void moveWithKeyboard(PaddleCtrl*, Transform*);
	void moveWithMouse(PaddleCtrl*, Transform*);
	void moveWithAI(PaddleCtrl*, Transform*);
	bool ballComingTowardsPaddle(const Vector2D&);

	Transform *ballTr_;
	std::array<ecs::Entity*, 2> paddles_;

};

