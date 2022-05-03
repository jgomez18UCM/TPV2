// This file is part of the course TPV2@UCM - Samir Genaim

#include "NetworkSystem.h"

#include <iostream>

#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLNetUtils.h"
#include "../utils/Vector2D.h"
#include "BallSystem.h"
#include "GameCtrlSystem.h"
#include "network_messages.h"
#include "PaddlesSystem.h"



NetworkSystem::NetworkSystem() :
		host_(false), //
		fighter_(0), //
		sock_(), //
		p_(), //
		sockSet_(), //
		port_(), //
		connected_(false), //
		otherPlayerAddr_() {

}

NetworkSystem::~NetworkSystem() {
	if (p_ != nullptr) {
		SDLNet_UDP_Close(sock_);
		SDLNet_FreePacket(p_);
	}
}

void NetworkSystem::recieve(const Message &m) {
	if (!host_)
		return;

	switch (m.id) {
	case _m_ROUND_START:
		tellOtherClientToStartRound();
		break;
	case _m_NEW_GAME:
		tellOtherClientToStartGame();
		break;
	case _m_BALL_EXIT:
		tellOtherClientBallExit(
				static_cast<GameCtrlSystem::Side>(m.ball_exit.side));
		break;
	default:
		break;
	}
}

void NetworkSystem::initSystem() {
}

bool NetworkSystem::connect() {

	char choice;
	bool done = false;
	bool success = false;

	while (!done) {
		std::cout << "Do you want to be host, client or exit [h/c/e]? "
				<< std::endl;
		std::cin >> choice;
		switch (choice) {
		case 'h':
		case 'H':
			success = initHost();
			done = true;
			break;
		case 'c':
		case 'C':
			success = initClient();
			done = true;
			break;
		case 'e':
		case 'E':
			done = true;
			break;
		default:
			break;
		}
	}

	return success;
}

void NetworkSystem::disconnect() {
	if (!connected_)
		return;

	net::Message m;

	m.id = net::_DISCONNECTING;
	p_->address = otherPlayerAddr_;
	SDLNetUtils::serializedSend(m, p_, sock_);

}

void NetworkSystem::update() {
	net::Message m;
	while (SDLNetUtils::deserializedReceive(m, p_, sock_) > 0) {
		switch (m.id) {
		case net::_CONNECTION_REQUEST:
			handleConnectionRequest();
			break;
		case net::_PADDLE_POS:
			handlePaddlePos();
			break;
		case net::_BALL_POS:
			handleBallPos();
			break;
		case net::_BALL_VEL:
			handleBallVel();
			break;
		case net::_START_GAME_REQUEST:
			handleStartGameRequest();
			break;
		case net::_START_ROUND_REQUEST:
			handleStartRoundRequest();
			break;
		case net::_START_THE_GAME:
			handleStartTheGame();
			break;
		case net::_START_THE_ROUND:
			handleStartTheRound();
			break;
		case net::_BALL_EXIT:
			handleBallExit();
			break;
		case net::_DISCONNECTING:
			handleDisconnecting();
			break;
		default:
			break;
		}

	}

}

bool NetworkSystem::initConnection(Uint16 port) {
	sock_ = SDLNet_UDP_Open(port);
	if (!sock_) {
		SDLNetUtils::print_SDLNet_error();
		return false;
	}

	sockSet_ = SDLNet_AllocSocketSet(1);
	if (!sockSet_) {
		SDLNetUtils::print_SDLNet_error();
		return false;
	}

	SDLNet_UDP_AddSocket(sockSet_, sock_);
	p_ = SDLNet_AllocPacket(512);
	if (!p_) {
		SDLNetUtils::print_SDLNet_error();
		return false;
	}

	port_ = SDLNetUtils::getSocketPort(sock_);

	return true;
}

bool NetworkSystem::initHost() {

	Uint16 port;
	std::cout << "Enter a port number to use: ";
	if (!(std::cin >> port)) {
		std::cerr << "Invalid port" << std::endl;
		return false;
	}

	if (!initConnection(port))
		return false;

	host_ = true;
	fighter_ = 0;
	connected_ = false;
	return true;

}

bool NetworkSystem::initClient() {

	Uint16 port;
	std::string host;

	std::cout << "Enter the host and port (separated by space) of the host: "
			<< std::endl;
	if (!(std::cin >> host >> port)) {
		std::cerr << "Invalid host/port" << std::endl;
		return false;
	}

	if (SDLNet_ResolveHost(&otherPlayerAddr_, host.c_str(), port) < 0) {
		SDLNetUtils::print_SDLNet_error();
		return false;
	}

	host_ = false;

	initConnection(0);

	net::Message m;

	m.id = net::_CONNECTION_REQUEST;
	p_->address = otherPlayerAddr_;
	SDLNetUtils::serializedSend(m, p_, sock_);

	if (SDLNet_CheckSockets(sockSet_, 3000) > 0) {
		if (SDLNet_SocketReady(sock_)) {
			SDLNetUtils::deserializedReceive(m, p_, sock_);
			if (m.id == net::_REQUEST_ACCEPTED) {
				net::ReqAccMsg m;
				m.deserialize(p_->data);
				fighter_ = m.side;
				host_ = false;
				connected_ = true;
			}

		}
	}

	if (!connected_) {
		std::cout << "Could not connect to the other player " << std::endl;
		return false;
	}

	return true;

}

void NetworkSystem::sendPaddlePosition(Transform *tr) {
	if (!connected_)
		return;

	net::PaddelPosMsg m;
	m.id = net::_PADDLE_POS;
	m.side = fighter_;
	m.x = tr->pos_.getX();
	m.y = tr->pos_.getY();
	p_->address = otherPlayerAddr_;
	SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);
}

void NetworkSystem::sendBallPosition(Transform *tr) {
	if (!connected_)
		return;

	net::BallPosMsg m;
	m.id = net::_BALL_POS;
	m.side = fighter_;
	m.x = tr->pos_.getX();
	m.y = tr->pos_.getY();
	p_->address = otherPlayerAddr_;
	SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);
}

void NetworkSystem::sendBallVelocity(Transform *tr) {
	if (!connected_ || !host_)
		return;

	net::BallVelMsg m;
	m.id = net::_BALL_VEL;
	m.side = fighter_;
	m.x = tr->vel_.getX();
	m.y = tr->vel_.getY();

	p_->address = otherPlayerAddr_;
	SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);
}

void NetworkSystem::handleConnectionRequest() {

	if (!connected_ && host_) {
		otherPlayerAddr_ = p_->address;
		connected_ = true;
		net::ReqAccMsg m;
		m.id = net::_REQUEST_ACCEPTED;
		m.side = 1 - fighter_;
		SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);
	}
}

void NetworkSystem::sendStarRoundtRequest() {
	assert(!isHost());

	net::StartRequestMsg m;

	m.id = net::_START_ROUND_REQUEST;
	m.side = fighter_;
	p_->address = otherPlayerAddr_;
	SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);

}

void NetworkSystem::sendStarGameRequest() {
	assert(!isHost());

	net::StartRequestMsg m;

	m.id = net::_START_GAME_REQUEST;
	m.side = fighter_;
	p_->address = otherPlayerAddr_;
	SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);

}

void NetworkSystem::handlePaddlePos() {
	net::PaddelPosMsg m;
	m.deserialize(p_->data);
	mngr_->getSystem<PaddlesSystem>()->changePaddlePos(m.side, m.x, m.y);
}

void NetworkSystem::handleBallPos() {
	assert(!host_);
	net::BallPosMsg m;
	m.deserialize(p_->data);
	mngr_->getSystem<BallSystem>()->changeBallPos(m.x, m.y);
}

void NetworkSystem::handleBallVel() {
	assert(!host_);
	net::BallVelMsg m;
	m.deserialize(p_->data);
	mngr_->getSystem<BallSystem>()->changeBallVel(m.x, m.y);
}

void NetworkSystem::handleStartGameRequest() {
	mngr_->getSystem<GameCtrlSystem>()->startGame();
}

void NetworkSystem::handleStartRoundRequest() {
	mngr_->getSystem<GameCtrlSystem>()->startRound();
}

void NetworkSystem::handleStartTheGame() {
	assert(!host_);
	mngr_->getSystem<GameCtrlSystem>()->startGame();
}

void NetworkSystem::handleStartTheRound() {
	assert(!host_);
	mngr_->getSystem<GameCtrlSystem>()->startRound();
}

void NetworkSystem::handleGameOver() {
	assert(!host_);
	mngr_->getSystem<GameCtrlSystem>()->gameOver();
}

void NetworkSystem::handleBallExit() {
	net::BallExitMsg m;
	m.deserialize(p_->data);
	mngr_->getSystem<GameCtrlSystem>()->onBallExit(m.side);
}

void NetworkSystem::handleDisconnecting() {
	connected_ = false;
	host_ = true;
	mngr_->getSystem<GameCtrlSystem>()->stopTheGame();
}

void NetworkSystem::tellOtherClientToStartRound() {
	net::Message m;

	m.id = net::_START_THE_ROUND;
	SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);

}

void NetworkSystem::tellOtherClientToStartGame() {
	net::Message m;

	m.id = net::_START_THE_GAME;
	SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);
}

void NetworkSystem::tellOtherClientBallExit(Uint8 side) {
	net::BallExitMsg m;

	m.id = net::_BALL_EXIT;
	m.side = side;
	SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);

}

