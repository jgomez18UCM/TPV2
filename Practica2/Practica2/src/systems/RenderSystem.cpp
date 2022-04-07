// This file is part of the course TPV2@UCM - Samir Genaim

#include "RenderSystem.h"

#include <SDL_rect.h>
#include "../components/Image.h"
#include "../components/RectangleViewer.h"
#include "../components/Transform.h"
#include "../components/FramedImage.h"
#include "../components/Health.h"
#include "../ecs/Manager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "GameCtrlSystem.h"

RenderSystem::RenderSystem() {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::recieve(const Message& m)
{
	switch (m.id) {
		case _m_ROUND_OVER:
			onRoundOver();
			break;
		case _m_ROUND_START:
			onRoundStart();
			break;
		case _m_GAME_OVER:
			onGameOver();
			break;
		case _m_GAME_START:
			onGameStart();
			break;
		default:
			break;
	}
}

void RenderSystem::initSystem() {
}

void RenderSystem::update() {
	drawMsgs();
	if (state_ == GameCtrlSystem::RUNNING) {
		drawBall();
		drawPaddles();
		drawFighter();
		drawAsteroids();
		drawBullets();
	}
}

void RenderSystem::drawMsgs() {
	auto state = mngr_->getSystem<GameCtrlSystem>()->getState();

	// message when game is not running
	if (state != GameCtrlSystem::RUNNING) {

		// game over message
		if (state == GameCtrlSystem::GAMEOVER) {
			auto &t = sdlutils().msgs().at(winner_ == 1 ? "gameover" : "win");
			t.render((sdlutils().width() - t.width()) / 2,
					(sdlutils().height() - t.height()) / 2);
		}

		// new game message
		if (state == GameCtrlSystem::NEWGAME) {
			auto &t = sdlutils().msgs().at("newgame");
			t.render((sdlutils().width() - t.width()) / 2,
					sdlutils().height() / 2 + t.height() * 2);
		} else {
			auto &t = sdlutils().msgs().at("paused");
			t.render((sdlutils().width() - t.width()) / 2,
					sdlutils().height() / 2 + t.height() * 2);
		}
	}
}


void RenderSystem::drawBall() {
	auto ball = mngr_->getHandler(ecs::_hdlr_BALL);
	auto ballTr = mngr_->getComponent<Transform>(ball);
	auto ballImg = mngr_->getComponent<Image>(ball);

	SDL_Rect dest = build_sdlrect(ballTr->pos_, ballTr->width_,
			ballTr->height_);

	assert(ballImg->tex_ != nullptr);
	ballImg->tex_->render(dest, ballTr->rot_);

}

void RenderSystem::drawPaddles() {
	for (auto e : mngr_->getEntities(ecs::_grp_PADDLES)) {
		auto paddleTr_ = mngr_->getComponent<Transform>(e);

		SDL_Rect dest = build_sdlrect(paddleTr_->pos_.getX(),
				paddleTr_->pos_.getY(), paddleTr_->width_, paddleTr_->height_);

		mngr_->getComponent<RectangleViewer>(e)->draw(sdlutils().renderer(),
				dest);
	}
}

void RenderSystem::drawFighter()
{
	auto player = mngr_->getHandler(ecs::_hdlr_FIGHTER);
	auto playerTr = mngr_->getComponent<Transform>(player);
	auto playerImg = mngr_->getComponent<Image>(player)->tex_;

	SDL_Rect dst = build_sdlrect(playerTr->pos_, playerTr->width_, playerTr->width_);
	
	assert(playerImg != nullptr);
	playerImg->render(dst, playerTr->rot_);
}

void RenderSystem::drawAsteroids()
{
	for (auto a : mngr_->getEntities(ecs::_grp_ASTEROIDS)) {
		mngr_->getComponent<FramedImage>(a)->draw();
		mngr_->getComponent<FramedImage>(a)->updateFrame();
	}
}

void RenderSystem::drawBullets()
{
	for (auto b : mngr_->getEntities(ecs::_grp_BULLETS)) {
		auto bulletTr = mngr_->getComponent<Transform>(b);
		auto bulletImg = mngr_->getComponent<Image>(b)->tex_;

		SDL_Rect dst = build_sdlrect(bulletTr->pos_, bulletTr->width_, bulletTr->width_);

		assert(bulletImg != nullptr);
		bulletImg->render(dst, bulletTr->rot_);
	}
}

void RenderSystem::drawHealth()
{
	mngr_->getComponent<Health>(mngr_->getHandler(ecs::_hdlr_FIGHTER))->draw();
}

void RenderSystem::onRoundStart()
{
	state_ = GameCtrlSystem::RUNNING;
}

void RenderSystem::onRoundOver()
{
	state_ = GameCtrlSystem::PAUSED;
}

void RenderSystem::onGameStart()
{
	state_ = GameCtrlSystem::NEWGAME;
}

void RenderSystem::onGameOver()
{
	state_ = GameCtrlSystem::GAMEOVER;
	winner_ = mngr_->getSystem<GameCtrlSystem>()->getWinner();
}
