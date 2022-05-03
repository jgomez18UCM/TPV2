// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_net.h>
#include <SDL_stdinc.h>
#include <string>
#include <array>

#include "../ecs/System.h"

struct Transform;

class NetworkSystem: public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_NETWORK)

	NetworkSystem();
	virtual ~NetworkSystem();

	void recieve(const Message&) override;
	void initSystem() override;
	void update() override;

	bool connect();
	void disconnect();

	inline Uint8 getFighter() {
		return fighter_;
	}

	inline bool isReady() {
		return connected_;
	}

	inline bool isHost() {
		return host_;
	}

	inline Uint16 getPort() {
		return port_;
	}

	inline std::string getName(int i) {
		return names[i];
	}

	
	void sendBulletTr(Transform* tr);
	void sendStarGameRequest();
	void sendFighterTr(Transform* tr);
	

private:

	bool initHost();
	bool initClient(std::string&);
	bool initConnection(Uint16 port);

	void handleConnectionRequest();
	void handleStartGameRequest();
	void handleStartTheGame();
	
	
	void handleGameOver();
	void handleDisconnecting();
	void handleFighterTr();
	void handleBulletShot();

	
	void tellOtherClientToStartGame();
	void tellOtherClientGameOver(std::string);

	void string_to_chars(std::string & str, char c_str[11]);
	void chars_to_string(std::string& str, char c_str[11]);

	bool host_;
	Uint8 fighter_; // 0 left 1 right
	UDPsocket sock_;
	UDPpacket *p_;
	SDLNet_SocketSet sockSet_;
	Uint16 port_;

	bool connected_;
	IPaddress otherPlayerAddr_;

	std::string names[2];
};

