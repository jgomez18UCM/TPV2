#include "AsteroidsManager.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"

#include "Transform.h"
#include "Generations.h"
#include "Follow.h"
#include "FramedImage.h"
#include "ShowAtOppositeSide.h"

AsteroidsManager::AsteroidsManager() : asteroidLimit_(), asteroids_(), spawnDelay_(), lastSpawn_()
{
}

AsteroidsManager::AsteroidsManager(int asteroidLimit, int delay): asteroidLimit_(asteroidLimit), asteroids_(),
	spawnDelay_(delay), lastSpawn_()
{
}

AsteroidsManager::~AsteroidsManager()
{
}

void AsteroidsManager::update()
{
	if (lastSpawn_ + spawnDelay_ > sdlutils().currRealTime()) return;
	std::cout << "spawn" << std::endl;
	bool typeB = sdlutils().rand().nextInt(0, 101) < 30;
	generateNewAsteroid(typeB);
	lastSpawn_ = sdlutils().currRealTime();
	
}

void AsteroidsManager::onCollision(ecs::Entity* e)
{
	asteroids_--;
	e->setAlive(false);
	if (e->getComponent<Generations>()->getGenerations() > 1) {
		crashAsteroid(e);
		crashAsteroid(e);
	}	
}

void AsteroidsManager::startRound()
{
	for (int i = 0; i < 10; i++) {
		bool typeB = sdlutils().rand().nextInt(0, 101) < 30;
		generateNewAsteroid(typeB);
	}
}

void AsteroidsManager::generateNewAsteroid(bool typeB)
{
	if (asteroids_ >= asteroidLimit_) return;
	asteroids_++;
	//Utilidades
	auto& rand = sdlutils().rand();
	auto height = sdlutils().height();
	auto width = sdlutils().width();

	//Creacion de la entidad
	auto asteroid = mngr_->addEntity();

	//Calculo de generaciones y tamaño
	auto gens = asteroid->addComponent<Generations>();
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
	auto asTr = asteroid->addComponent<Transform>();
	asTr->init(p, v, size, size, 0.0f);
	
	//Resto de componentes
	asteroid->addComponent<FramedImage>(&sdlutils().images().at(typeB ? "asteroidB" : "asteroidA"), 85,100);
	if(typeB) asteroid->addComponent<Follow>();
	asteroid->addComponent<ShowAtoppositeSide>();

	asteroid->addToGroup(ecs::_grp_ASTEROIDS);
}

void AsteroidsManager::crashAsteroid(ecs::Entity* e)
{
	if (asteroids_ >= asteroidLimit_) return;
	asteroids_++;

	// Creacion de la entidad
	auto asteroid = mngr_->addEntity();
	auto fTR = e->getComponent<Transform>();
	auto fPos = fTR->getPos();
	auto fVel = fTR->getVel();
	auto fW = fTR->getWidth();
	auto fH = fTR->getHeight();
	auto r = sdlutils().rand().nextInt(0, 361);
	//Calculo de generaciones y tamaño
	auto gens = asteroid->addComponent<Generations>();
	gens->setGenerations(e->getComponent<Generations>()->getGenerations() - 1);
	auto tr = asteroid->addComponent<Transform>();
	auto size = 10.0f + 5.0f * gens->getGenerations();
	auto pos = fPos + fVel.rotate(r) * 2 * std::max(fW, fH);
	auto vel = fVel.rotate(r) * 1.1f;
	tr->init(pos, vel, size, size, r);
	auto type = sdlutils().rand().nextInt(0, 101) <= 30;
	asteroid->addComponent<FramedImage>(&sdlutils().images().at(type? "asteroidB":"asteroidA"), 85,100);
	asteroid->addComponent<ShowAtoppositeSide>();
	if (type) asteroid->addComponent<Follow>();
}
