// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <array>
#include <cstdint>

#include "../ecs/System.h"
#include "../sdlutils/SDLUtils.h"

struct GameState;

class GameCtrlSystem: public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_GAMECTRL)

	enum State {
		NEWGAME = 0, // just before starting a new game
		PAUSED, // between rounds
		RUNNING, // playing
		GAMEOVER // game over
	};

	GameCtrlSystem();
	virtual ~GameCtrlSystem();

	inline State getState() {
		return state_;
	}

	bool getWinner() { return winner_; };
	void update() override;
	void recieve(const Message&) override;
	
private:

	void startRound();
	void startGame();
	void roundOver();
	void gameOver(int winner);

	void onCollission_FighterAsteroid();
	void onAsteroidsExtinction();

	Uint8 winner_; //0 - Ninguno, 1 - Asteroides, 2 - Jugador
	State state_;

};

