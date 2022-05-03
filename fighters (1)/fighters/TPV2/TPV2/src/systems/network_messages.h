// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../sdlutils/SDLNetUtils.h"

namespace net {

template<typename T, std::size_t N>
struct array {
	T v[N];

	inline Uint8* serialize(Uint8* buf) {
		return SDLNetUtils::serialize_array(v, N, buf);
	}

	inline Uint8* deserialize(Uint8* buf) {
		return SDLNetUtils::deserialize_array(v, N, buf);
	}

	T& operator[](int i) {
		return v[i];
	}

	const T& operator[](int i) const {
		return v[i];
	}

	operator T* () {
		return v;
	}

	operator const T* () const {
		return v;
	}
};

enum MsgId : Uint8 {
	_CONNECTION_REQUEST, //
	_REQUEST_ACCEPTED, //
	_REQUEST_REFUSED, //	
	_START_GAME_REQUEST, //
	_START_THE_GAME, //
	_GAME_OVER, //
	_DISCONNECTING,
	_BULLET_SHOT,
	_FIGHTER_TR,
	_COLLISION_BULLET_FIGHTER,

};

struct Message {
	Uint8 id;

	//
	_IMPL_SERIALIAZION_(id)
};

struct ReqAccMsg: Message {
	Uint8 fighter;
	array<char, 11> name;

	//
	_IMPL_SERIALIAZION_WITH_BASE_(Message, fighter, name)
};

struct ConnReqMsg : Message {
	array<char, 11> name;

	_IMPL_SERIALIAZION_WITH_BASE_(Message, name)
};

struct StartRequestMsg: Message {

	Uint8 side;

	//
	_IMPL_SERIALIAZION_WITH_BASE_(Message,side)
};

struct BulletShotMsg : Message {
	float x;
	float y;
	float rot;
	float velX;
	float velY;

	_IMPL_SERIALIAZION_WITH_BASE_(Message, x, y, rot, velX, velY)
};

struct FighterPosMsg : Message {
	Uint8 fighter;
	float x;
	float y;
	float rot;

	_IMPL_SERIALIAZION_WITH_BASE_(Message, fighter,  x, y, rot)
};

struct GameOverMsg : Message {
	array<char,11> name;

	_IMPL_SERIALIAZION_WITH_BASE_(Message, name);
};
}

