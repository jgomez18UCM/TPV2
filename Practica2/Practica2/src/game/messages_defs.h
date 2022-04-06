// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <cstdint>
#include "../ecs/ecs.h"

using msgId_type = uint8_t;
enum msgId : msgId_type {
	_m_ROUND_START, //
	_m_ROUND_OVER, //
	_m_GAME_START, //
	_m_GAME_OVER, //
	_m_NEW_GAME, //
	_m_BALL_EXIT, //
	_m_BALL_HIT_PADDLE
};


struct Message {
	msgId_type id;

	union {

		struct {
			uint8_t side;
		} ball_exit;

		struct {
			ecs::Entity *e;
		} ball_hit_paddle;

		struct {
			ecs::Entity* a;
			ecs::Entity* b;
		} collision_bullet_asteroid;
		
		struct {
			Vector2D pos;
			Vector2D vel;
			double width;
			double height;
		} shoot;

		struct {} round_over;
		struct {} round_start;
		struct {} game_start;
		struct {} game_over;
		struct {} collision_fighter_asteroid;
		struct {} asteroids_extintion;
	};
};
