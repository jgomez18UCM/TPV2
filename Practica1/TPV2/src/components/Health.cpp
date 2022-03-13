#include "Health.h"
#include "Transform.h"
#include "../ecs/Entity.h"

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
	tr_ = ent_->getComponent<Transform>();
}

void Health::render()
{
	SDL_Rect dst = build_sdlrect(0, 0, tr_->getWidth(), tr_->getHeight());
	for (int i = 0; i < health_; i++) {
		tex_->render(dst);
		dst.x += tr_->getWidth();
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
