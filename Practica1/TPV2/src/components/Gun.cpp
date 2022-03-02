#include "Gun.h"
#include "Transform.h"
#include "Image.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"

Gun::Gun() : bulletTx_(), lastShot_(), shotCd_(), tr_()
{
}

Gun::Gun(int shotCd, Texture* bulletTx) : bulletTx_(bulletTx), shotCd_(shotCd), lastShot_(), tr_()
{
}

Gun::~Gun()
{
}

void Gun::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void Gun::update()
{
	auto& ihdlr = ih();

	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(SDL_SCANCODE_S)) {
			if (lastShot_ + shotCd_ < sdlutils().currRealTime()) {
				generateBullet();
				lastShot_ = sdlutils().currRealTime();
				sdlutils().soundEffects().at("gunshot").play();
			}
		}
	}
}

void Gun::generateBullet()
{
	auto bullet = mngr_->addEntity();
	auto pos = tr_->getPos();
	auto rot = tr_->getRot();
	auto bulletTr = bullet->addComponent<Transform>();
	auto bulletPos = Vector2D(pos.getX(), pos.getY());
	auto bulletVel = Vector2D(0,-1).normalize().rotate(rot) * 10;
	bulletTr->init(bulletPos, bulletVel, 30, 30, rot);
	bullet->addComponent<Image>(bulletTx_);
}
