#include "AsteroidsSystem.h"
#include "../ecs/Manager.h"
#include "../components/Follow.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"
#include "../components/Generations.h"
#include "../components/FramedImage.h"
#include "../components/ShowAtOppositeSide.h"
#include "../sdlutils/SDLUtils.h"

AsteroidsSystem::AsteroidsSystem() : numOfAsteroids_(0), active_(false), limitAsteroids_(30)
{
}

AsteroidsSystem::AsteroidsSystem(int limit) : numOfAsteroids_(0), active_(false), limitAsteroids_(limit)
{
}

void AsteroidsSystem::recieve(const Message& m)
{
	switch (m.id)
	{
		case _m_COLLISSION_ASTEROID_BULLET:	
			onCollission_AsteroidBullet(m.collission_asteroid_bullet.a);
			break;
		default:
			break;
	}
}

void AsteroidsSystem::initSystem()
{
}

void AsteroidsSystem::update()
{
	for (ecs::Entity* a : mngr_->getEntities(ecs::_grp_ASTEROIDS)) {
		//Seguimiento si es tipo B
		mngr_->getComponent<Transform>(a)->move();
	}
}

void AsteroidsSystem::onCollission_AsteroidBullet(ecs::Entity* a)
{
	divideAsteroid(a);
	mngr_->setAlive(a, false);
	numOfAsteroids_--;
}

void AsteroidsSystem::onRoundOver()
{
	for (ecs::Entity* a : mngr_->getEntities(ecs::_grp_ASTEROIDS)) {
		mngr_->setAlive(a, false);
	}
	numOfAsteroids_ = 0;
	active_ = false;
}

void AsteroidsSystem::onRoundStart()
{
	active_ = true;
	generateAsteroidsStart(10);
}

void AsteroidsSystem::generateAsteroidsStart(int n)
{
	for (int i = 0; i < n; i++) {
		bool typeB = sdlutils().rand().nextInt(0, 101) < 30;
		generateNewAsteroid(typeB);
	}
}

void AsteroidsSystem::generateNewAsteroid(bool typeB)
{
	if (numOfAsteroids_ >= limitAsteroids_) return;
	numOfAsteroids_++;
	//Utilidades
	auto& rand = sdlutils().rand();
	auto height = sdlutils().height();
	auto width = sdlutils().width();

	//Creacion de la entidad
	auto asteroid = mngr_->addEntity(ecs::_grp_ASTEROIDS);

	//Calculo de generaciones y tamaño
	auto gens = mngr_->addComponent<Generations>(asteroid);
	auto asGen = rand.nextInt(1, 4);
	gens->setGenerations(asGen);
	auto size = 10.0f + 5.0f * asGen;

	//Calculo de la posicion en los bordes
	auto border = rand.nextInt(0, 4);
	float x = rand.nextInt(0, width - size), y = rand.nextInt(0, height - size);
	switch (border) {
	case 0:
		x = width - size;
		break;
	case 1:
		x = 0;
		break;
	case 2:
		y = height - size;
		break;
	case 3:
		y = 0;
		break;
	}
	auto p = Vector2D(x, y);

	//Calculo de la velocidad
	auto r = Vector2D(rand.nextInt(-100, 100), rand.nextInt(-100, 100));
	auto c = Vector2D(sdlutils().width() / 2, sdlutils().height() / 2) + r;
	float speed = rand.nextInt(1, 10) / 10.0f;
	Vector2D v = (c - p).normalize() * speed;

	//Inicializacion del transform
	auto asTr = mngr_->addComponent<Transform>(asteroid);
	asTr->init(p, v, size, size, 0.0f);

	//Resto de componentes
	mngr_->addComponent<FramedImage>(asteroid, &sdlutils().images().at(typeB ? "asteroidB" : "asteroidA"), 85, 100);
	if (typeB) mngr_->addComponent<Follow>(asteroid);
	mngr_->addComponent<ShowAtoppositeSide>(asteroid);
}

void AsteroidsSystem::divideAsteroid(ecs::Entity* a)
{
	if (numOfAsteroids_ >= limitAsteroids_) return;
	numOfAsteroids_++;

	// Creacion de la entidad
	auto asteroid = mngr_->addEntity(ecs::_grp_ASTEROIDS);
	auto fTR = mngr_->getComponent<Transform>(a);
	auto fPos = fTR->pos_;
	auto fVel = fTR->vel_;
	auto fW = fTR->width_;
	auto fH = fTR->height_;
	auto r = sdlutils().rand().nextInt(0, 361);
	//Calculo de generaciones y tamaño
	auto gens = mngr_->addComponent<Generations>(asteroid);
	gens->setGenerations(mngr_->getComponent<Generations>(a)->getGenerations() - 1);
	auto tr = mngr_->addComponent<Transform>(asteroid);
	auto size = 10.0f + 5.0f * gens->getGenerations();
	auto pos = fPos + fVel.rotate(r) * 2 * std::max(fW, fH);
	auto vel = fVel.rotate(r) * 1.1f;
	tr->init(pos, vel, size, size, r);
	auto type = sdlutils().rand().nextInt(0, 101) <= 30;
	mngr_->addComponent<FramedImage>(asteroid, &sdlutils().images().at(type ? "asteroidB" : "asteroidA"), 85, 100);
	mngr_->addComponent<ShowAtoppositeSide>(asteroid);
	if (type) mngr_->addComponent<Follow>(asteroid);
}
