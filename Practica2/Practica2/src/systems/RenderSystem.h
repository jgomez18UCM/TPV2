// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"
#include "../sdlutils/SDLUtils.h"

class RenderSystem: public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_RENDER)

	RenderSystem();
	virtual ~RenderSystem();
	void initSystem() override;
	void update() override;

private:
	void drawMsgs();
	void drawScore();
	void drawBall();
	void drawPaddles();

	void onRoundStart();
	void onRoundOver();
	void onGameStart();
	void onGameOver();

	Uint8 winner_; //0 - Ninguno, 1 - Asteroides, 2 - Jugador
	Uint8 state_; //Estado actual del juego
};

