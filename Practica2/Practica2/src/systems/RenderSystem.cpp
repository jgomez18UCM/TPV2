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

void RenderSystem::initSystem() {
}

void RenderSystem::update() {
	drawMsgs();
	if (state_ == GameCtrlSystem::RUNNING) {
		drawBall();
		drawPaddles();
		drawFighter();
		drawAsteroids();
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
	mngr_->getComponent<Image>(player)->render();
}

void RenderSystem::drawAsteroids()
{
	for (auto a : mngr_->getEntities(ecs::_grp_ASTEROIDS)) {
		mngr_->getComponent<FramedImage>(a)->render();
		mngr_->getComponent<FramedImage>(a)->update();
	}
}

void RenderSystem::drawBullets()
{
	for (auto b : mngr_->getEntities(ecs::_grp_BULLETS)) {
		mngr_->getComponent<Image>(b)->render();
	}
}

void RenderSystem::drawHealth()
{
	mngr_->getComponent<Health>(mngr_->getHandler(ecs::_hdlr_FIGHTER))->render();
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
	state_ = GameCtrlSystem::RUNNING;
}

void RenderSystem::onGameOver()
{
	state_ = GameCtrlSystem::GAMEOVER;
	winner_ = mngr_->getSystem<GameCtrlSystem>()->getWinner();
}
