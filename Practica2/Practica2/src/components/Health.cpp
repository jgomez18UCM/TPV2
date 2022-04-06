#include "Health.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

Health::Health() : maxHealth_(3), tex_(), tr_(){
	health_ = maxHealth_;
}

Health::Health(int maxHealth, Texture* tex):maxHealth_(maxHealth), health_(maxHealth), tex_(tex), tr_()
{
}

Health::~Health()
{
}

void Health::initComponent()
{
	tr_ = mngr_->getComponent<Transform>(ent_);
}

void Health::render()
{
	SDL_Rect dst = build_sdlrect(0, 0, tr_->width_, tr_->height_);
	for (int i = 0; i < health_; i++) {
		tex_->render(dst);
		dst.x += tr_->width_;
	}
}

void Health::getDamage(int damage)
{
	health_ -= damage;
}

int Health::getHealth()
{
	return health_;
}

void Health::resetHealth()
{
	health_ = maxHealth_;
}
