// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameCtrl.h"

#include <algorithm>
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "FighterCtrl.h"
#include "AsteroidsManager.h"
#include "Transform.h"
#include "Health.h"

GameCtrl::GameCtrl() : gameState(NEWGAME) {
}

GameCtrl::~GameCtrl() {
}

void GameCtrl::initComponent() {
}

void GameCtrl::update() {
	auto &ihldr = ih();
	if (gameState == RUNNING) return;
	if (ihldr.keyDownEvent()) {
		if (ihldr.isKeyDown(SDL_SCANCODE_SPACE)) { // create start
			if (gameState == GAMEOVER || gameState == WIN) {
				restart();
			}
			else {
				newRound();
			}
		}
	}
}

void GameCtrl::render() {
	auto x = 0;
	auto y = 0;
	switch (gameState) {
	case NEWGAME:
		sdlutils().msgs().at("newgame").render(x, y);
		break;
	case PAUSED:
		sdlutils().msgs().at("paused").render(x, y);
		break;
	case WIN:
		sdlutils().msgs().at("win").render(x, y);
		break;
	case GAMEOVER:
		sdlutils().msgs().at("gameover").render(x, y);
		break;
	default:
		break;
	}
}

void GameCtrl::endGame(bool win)
{
	pause();
	gameState = win ? WIN : GAMEOVER;
}

void GameCtrl::pause()
{
	gameState = PAUSED;
	auto player = mngr_->getHandler(ecs::_hdlr_FIGHTER);
	auto asMan = mngr_->getHandler(ecs::_hdlr_ASTEROIDSMANAGER)->getComponent<AsteroidsManager>();
	//Manager destruir asteroides
	asMan->destroyAll();
	asMan->setActive(false);
	//Desactivar control
	player->getComponent<FighterCtrl>()->setMove(false);
	auto tr = player->getComponent<Transform>();
	//Reseteo posicion
	tr->init(Vector2D(sdlutils().width() / 2, sdlutils().height() / 2), Vector2D(), tr->getWidth(), tr->getHeight(), 0.0f);
	for (auto b : mngr_->getEntitiesByGroup(ecs::_grp_BULLETS)) {
		b->setAlive(false);
	}
}

void GameCtrl::newRound() {
	gameState = RUNNING;
	auto player = mngr_->getHandler(ecs::_hdlr_FIGHTER);
	auto asMan = mngr_->getHandler(ecs::_hdlr_ASTEROIDSMANAGER)->getComponent<AsteroidsManager>();
	asMan->startRound();
	asMan->setActive(true);
	//Activar Control
	player->getComponent<FighterCtrl>()->setMove(true);
}

void GameCtrl::restart()
{
	gameState = NEWGAME;
	auto player = mngr_->getHandler(ecs::_hdlr_FIGHTER);
	player->getComponent<Health>()->resetHealth();
}
