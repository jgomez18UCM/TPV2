// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <cstdint>
#include "../ecs/ecs.h"

using msgId_type = uint8_t;
enum msgId : msgId_type {
	_m_ROUND_START, //
	_m_ROUND_OVER, //     
	_m_GAME_OVER, //
	_m_NEW_GAME, //
	_m_COLLISSION_ASTEROID_FIGHTER,
	_m_COLLISSION_ASTEROID_BULLET,
	_m_SHOOT,
	_m_ASTEROID_EXTINCTION
};


struct Message {
	msgId_type id;

	union {

		struct {
			ecs::Entity* b;
			ecs::Entity* a;
		} collission_asteroid_bullet;

		struct {
			double posX;
			double posY;
			double velX;
			double velY;
			double width;
			double height;
		} shoot;

		struct {
			int winner;
		} game_over;
	};
};
