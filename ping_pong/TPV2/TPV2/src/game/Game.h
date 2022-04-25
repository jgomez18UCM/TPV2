// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>

namespace ecs {
class Manager;
}

class BallSystem;
class PaddlesSystem;
class GameCtrlSystem;
class CollisionsSystem;
class RenderSystem;
class NetworkSystem;

class Game {
public:
	Game();
	virtual ~Game();
	bool init();
	void start();
private:
	ecs::Manager *mngr_;

	BallSystem *ballSys_;
	PaddlesSystem *paddlesSys_;
	GameCtrlSystem *gameCtrlSys_;
	CollisionsSystem *collisionsSys_;
	RenderSystem *renderSys_;
	NetworkSystem *netSys_;
};

