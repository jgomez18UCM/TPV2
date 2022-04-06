// This file is part of the course TPV2@UCM - Samir Genaim


#pragma once

// Components list - must have at least one element
//
#define _CMPS_LIST_ \
	_TRANSFORM, \
	_IMAGE, \
	_RECTANGLEVIEWER, \
	_PADDLECTRL, \
	_STOPONBORDER, \
	_BOUNCEONBORDER, \
	_GAMESTATE, \
	_GAMEINFOMSGS,\
	_DEACCELERATION, \
	_GUN, \
	_GENERATIONS,\
	_FOLLOW,\
	_BORDERINTERACTION,\
	_HEALTH


// Groups list - must have at least one element
//
#define _GRPS_LIST_ \
	_grp_PADDLES,\
	_grp_BULLETS,\
	_grp_ASTEROIDS

// Handlers list - must have at least one element
//
#define _HDLRS_LIST_ \
	_hdlr_BALL,\
	_hdlr_FIGHTER

// Systems list - must have at least one element
//
#define _SYS_LIST_ \
    _sys_BALL, \
	_sys_PADDLES, \
	_sys_RENDER, \
	_sys_GAMECTRL, \
	_sys_COLLISIONS, \
	_SYS_FIGHTER, \
	_SYS_ASTEROIDS, \
	_SYS_BULLETS


