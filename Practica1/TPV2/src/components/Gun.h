#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"

class Transform;

#pragma once
class Gun : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_GUN)
	Gun();
	Gun(int shotCd, Texture* bulletTx);
	~Gun();
	void initComponent() override;
	void update() override;
	void generateBullet();
private:
	Texture* bulletTx_;
	Uint32 lastShot_;
	Uint32 shotCd_;
	Transform* tr_;
};

