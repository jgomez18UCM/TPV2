// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"
#include <string>

class GameCtrlSystem: public ecs::System {
public:

	enum GameState : uint8_t {
		_RUNNING, _STOPPED
	};

	__SYSID_DECL__(ecs::_sys_GAMECTRL)

	GameCtrlSystem();
	virtual ~GameCtrlSystem();


	void initSystem() override;
	void update() override;
	void recieve(const Message &m) override;

	void startRound() {};
	void startGame();
	void stopTheGame();
	void gameOver(std::string s);

private:
	void requestToStartTheGame();
	void handleBulletHitFighter(const Message&);

	uint8_t state_;
};

