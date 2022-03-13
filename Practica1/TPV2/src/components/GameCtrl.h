// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"

class GameCtrl: public ecs::Component {
public:

	__CMPID_DECL__(ecs::_GAMECTRL)
	enum State {
		NEWGAME,
		RUNNING,
		PAUSED,
		GAMEOVER,
		WIN
	};
	GameCtrl();
	virtual ~GameCtrl();

	inline void onFighterHit() {
		
	}

	void initComponent() override;
	void update() override;
	void render() override;
	void endGame(bool win);
	void pause();

private:
	void newRound();
	void restart();
	State gameState;
};

