#include "FramedImage.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"


FramedImage::FramedImage() : tr_(nullptr), tex_(nullptr), frameWidth_(50), frameHeight_(50), x(0), y(0), lastUpdate_() {
}

FramedImage::FramedImage(Texture* tex, int frameWidth, int frameHeight) :
	tr_(nullptr), tex_(tex), frameWidth_(frameWidth), frameHeight_(frameHeight), x(0), y(0), lastUpdate_() {
}

FramedImage::~FramedImage()
{
}

void FramedImage::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void FramedImage::render()
{
	SDL_Rect dest = build_sdlrect(tr_->getPos().getX(), tr_->getPos().getY(), tr_->getWidth(),
		tr_->getHeight());

	SDL_Rect src = build_sdlrect(x, y, frameWidth_, frameHeight_);

	assert(tex_ != nullptr);
	tex_->render(src, dest );			
}

void FramedImage::update()
{
	if (lastUpdate_ + 50 > sdlutils().currRealTime()) {
		return;
	}

	lastUpdate_ = sdlutils().currRealTime();

	if (x < tex_->width() - frameWidth_) x += frameWidth_;
	else if (y < tex_->height() - frameHeight_) {
		y += frameHeight_;
		x = 0;
	}
	else {
		y = 0;
		x = 0;
	}
}
