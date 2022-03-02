#pragma once
#include <SDL_stdinc.h>
#include "../ecs/Component.h"

class Transform;
class Texture;

class FramedImage : public ecs::Component {
public:
	__CMPID_DECL__(ecs::_IMAGE)

	FramedImage();
	FramedImage(Texture* tex, int frameWidth, int frameHeight);
	virtual ~FramedImage();

	void setTexture(Texture* tex) {
		tex_ = tex;
	}

	void initComponent() override;
	void render() override;
	void update() override;

private:
	Transform* tr_;
	Texture* tex_;
	int frameWidth_, frameHeight_;
	int x, y;
	Uint32 lastUpdate_;
};

