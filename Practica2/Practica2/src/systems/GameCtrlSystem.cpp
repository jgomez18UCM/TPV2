// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameCtrlSystem.h"

#include "../components/Health.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
GameCtrlSystem::GameCtrlSystem() : state_(NEWGAME), winner_(0) {
}

GameCtrlSystem::~GameCtrlSystem() {
}


void GameCtrlSystem::update() {
	if (state_ != RUNNING) {

		auto &inhdlr = ih();

		if (inhdlr.isKeyDown(SDL_SCANCODE_SPACE)) {
			switch (state_) {
			case NEWGAME:
			case PAUSED:
				startRound();
				break;
			case GAMEOVER:
				startGame();
				break;
			default:
				break;
			}
		}
	}
}



void GameCtrlSystem::recieve(const Message &m) {
	switch (m.id) {
	case _m_ASTEROID_EXTINCTION:
		onAsteroidsExtinction();
		break;
	case _m_COLLISSION_ASTEROID_FIGHTER:
		onCollission_FighterAsteroid();
		break;
	default:
		break;
	}
}

void GameCtrlSystem::startRound() {
	Message m;
	state_ = RUNNING;
	m.id = _m_ROUND_START;
	mngr_->send(m);
}

void GameCtrlSystem::startGame() {
	Message m;
	state_ = NEWGAME;
	m.id = _m_NEW_GAME;
	mngr_->send(m);
}

void GameCtrlSystem::roundOver() {
	Message m;
	state_ = PAUSED;
	m.id = _m_ROUND_OVER;
	mngr_->send(m);
}

void GameCtrlSystem::gameOver(int winner)
 {
	winner_ = winner;
	state_ = GAMEOVER;
	Message m;
	m.id = _m_GAME_OVER;
	m.game_over.winner = winner;
	mngr_->send(m);
}



void GameCtrlSystem::onCollission_FighterAsteroid()
{
	auto playerHealth = mngr_->getComponent<Health>(mngr_->getHandler(ecs::_hdlr_FIGHTER));
	playerHealth->getDamage(1);
	roundOver();
	if (playerHealth->getHealth() <= 0) {
		gameOver(1);
	}
}

void GameCtrlSystem::onAsteroidsExtinction()
{
	roundOver();
	gameOver(2);
}
