// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../components/GameCtrl.h"
#include "../components/Image.h"
#include "../components/FighterCtrl.h"
#include "../components/ShowAtOppositeSide.h"
#include "../components/DeAcceleration.h"
#include "../components/Transform.h"
#include "../components/FramedImage.h"
#include "../components/Health.h"
#include "../components/Gun.h"

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"


using ecs::Entity;
using ecs::Manager;

Game::Game() :
		mngr_(nullptr) {
}

Game::~Game() {
	delete mngr_;
}

void Game::init() {

	// initialise the SDLUtils singleton
	SDLUtils::init("Demo", 800, 600, "resources/config/asteroid.resources.json");

	// Create the manager
	mngr_ = new Manager();

	// create the PacMan entity
	//
	auto fighter = mngr_->addEntity();
	mngr_->setHandler(ecs::_hdlr_FIGHTER, fighter);
	auto tr = fighter->addComponent<Transform>();
	auto s = 50.0f;
	auto x = (sdlutils().width() - s) / 2.0f +100;
	auto y = (sdlutils().height() - s) / 2.0f;
	tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
	fighter->addComponent<Image>(&sdlutils().images().at("fighter"));
	fighter->addComponent<FighterCtrl>();
	fighter->addComponent<ShowAtoppositeSide>();
	fighter->addComponent<DeAcceleration>(0.995f);
	fighter->addComponent<Health>(3, &sdlutils().images().at("heart"));
	fighter->addComponent<Gun>(3000, &sdlutils().images().at("bullet"));

	auto asteroid = mngr_->addEntity();
	auto astTr = asteroid->addComponent<Transform>();
	auto as = 50.0f;
	auto ax = 100.0f;
	auto ay = 50.0f;
	astTr->init(Vector2D(ax, ay), Vector2D(), as, as, 0.0f);
	asteroid->addComponent<FramedImage>(&sdlutils().images().at("asteroidA"), 85, 100);
	// create the game info entity
	auto ginfo = mngr_->addEntity();
	mngr_->setHandler(ecs::_hdlr_GAMEINFO, ginfo);
	ginfo->addComponent<GameCtrl>();
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE) || ihdlr.closeWindowEvent()) {
			exit = true;
			continue;
		}

		mngr_->update();
		mngr_->refresh();

		checkCollisions();

		sdlutils().clearRenderer();
		mngr_->render();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

}

void Game::checkCollisions() {

	// the PacMan's Transform
	//
	auto pTR = mngr_->getHandler(ecs::_hdlr_FIGHTER)->getComponent<Transform>();

	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_ASTEROIDS)) {

		if (e->isAlive()) { // if the star is active (it might have died in this frame)

			// the Star's Transform
			//
			auto eTR = e->getComponent<Transform>();

			// check if PacMan collides with the Star (i.e., eat it)
			if (Collisions::collides(pTR->getPos(), pTR->getWidth(),
					pTR->getHeight(), //
					eTR->getPos(), eTR->getWidth(), eTR->getHeight())) {
				e->setAlive(false);
				mngr_->getHandler(ecs::_hdlr_GAMEINFO)->getComponent<GameCtrl>()->onStarEaten();

				// play sound on channel 1 (if there is something playing there
				// it will be cancelled
				sdlutils().soundEffects().at("pacman_eat").play(0, 1);
			}
		}
	}
}
