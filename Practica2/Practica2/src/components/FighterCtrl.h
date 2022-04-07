#pragma once
#include <SDL_scancode.h>
#include <SDL_stdinc.h>

#include "../ecs/Component.h"
struct FighterCtrl : ecs::Component {

	__CMPID_DECL__(ecs::_PADDLECTRL)


		

	FighterCtrl() :
		up_(SDL_SCANCODE_UP), //
		left_(SDL_SCANCODE_LEFT), //
		right_(SDL_SCANCODE_RIGHT), //
		shoot_(SDL_SCANCODE_S), //
		thrust_(10.0f) //
	{
	}
	virtual ~FighterCtrl() {
	}

	inline void setKeys(SDL_Scancode up, SDL_Scancode down, SDL_Scancode stop) {
		up_ = up;
		left_ = down;
		right_ = stop;
	}

	inline void setThrust(float thrust) {
		thrust_ = thrust;
	}

	SDL_Scancode up_;
	SDL_Scancode left_;
	SDL_Scancode right_;
	SDL_Scancode shoot_;
	float thrust_;
};

