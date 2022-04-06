#include "Gun.h"
#include "Transform.h"
#include "Image.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"

Gun::Gun() : bulletTx_(), lastShot_(), shotCd_(), tr_()
{
}

Gun::Gun(int shotCd, Texture* bulletTx) : bulletTx_(bulletTx), shotCd_(shotCd), lastShot_(-shotCd), tr_()
{
}

Gun::~Gun()
{
}

void Gun::initComponent()
{
	tr_ = mngr_->getComponent<Transform>(ent_);
	assert(tr_ != nullptr);
}

void Gun::update()
{
	auto& ihdlr = ih();
	if (lastShot_ + shotCd_ > sdlutils().currRealTime()) return;
	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(SDL_SCANCODE_S)) {		
			generateBullet();
			lastShot_ = sdlutils().currRealTime();
			sdlutils().soundEffects().at("gunshot").play();		
		}
	}
}

void Gun::generateBullet()
{
	auto bullet = mngr_->addEntity(ecs::_grp_BULLETS);
	auto vel = tr_->vel_;
	auto pos = tr_->pos_;
	auto rot = tr_->rot_;
	auto w = tr_->width_;
	auto h = tr_->height_;
	auto bulletTr = mngr_->addComponent<Transform>(bullet);
	auto bulletPos = pos + Vector2D(w / 2.0f, h / 2.0f) - Vector2D(0.0f, h / 2.0f + 5.0f + 12.0f).rotate(rot) - Vector2D(2.0f, 10.0f);;
	auto bulletVel = Vector2D(0.0f, -1.0f).rotate(rot) * (vel.magnitude() + 5.0f);
	bulletTr->init(bulletPos, bulletVel, 5.0f, 20.0f, rot);
	mngr_->addComponent<Image>(ent_, bulletTx_);	

}
