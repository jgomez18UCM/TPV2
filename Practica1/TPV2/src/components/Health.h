#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"

class Transform;
#pragma once
class Health : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_HEALTH)
	Health();
	Health(int maxHealth, Texture* tex);
	~Health();
	void initComponent() override;
	void render() override;
	void getDamage(int damage);
	int getHealth();
	void resetHealth();
private:
	int health_, maxHealth_;
	Texture* tex_;
	Transform* tr_;
};

