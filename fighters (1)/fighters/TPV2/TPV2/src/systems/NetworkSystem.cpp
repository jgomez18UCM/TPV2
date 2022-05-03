// This file is part of the course TPV2@UCM - Samir Genaim

#include "NetworkSystem.h"

#include <iostream>

#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLNetUtils.h"
#include "../utils/Vector2D.h"
#include "../components/FighterInfo.h"
#include "GameCtrlSystem.h"
#include "BulletsSystem.h"
#include "FightersSystem.h"
#include "network_messages.h"


NetworkSystem::NetworkSystem() :
		host_(false), //
		fighter_(0), //
		sock_(), //
		p_(), //
		sockSet_(), //
		port_(), //
		connected_(false), //
		otherPlayerAddr_(),
		names(){

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
	case _m_GAME_START:
		tellOtherClientToStartGame();
		break;
	case _m_BULLET_HIT_FIGHTER:
		tellOtherClientGameOver(mngr_->getComponent<FighterInfo>(m.bullet_hit.fighter_)->id_);
		break;
	default:
		break;
	}
}

void NetworkSystem::initSystem()
{

}

bool NetworkSystem::connect() {

	char choice;
	bool done = false;
	bool success = false;
	std::string name;

	while (!done) {
		std::cout << "Enter your name" << std::endl;
		std::cin >> name;
		std::cout << "Do you want to be host, client or exit [h/c/e]? "
				<< std::endl;
		std::cin >> choice;
		switch (choice) {
		case 'h':
		case 'H':
			success = initHost();
			names[fighter_] = name;
			done = true;
			break;
		case 'c':
		case 'C':
			success = initClient(name);
			names[fighter_] = name;
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
		case net::_START_GAME_REQUEST:
			handleStartGameRequest();
			break;
		case net::_START_THE_GAME:
			handleStartTheGame();
			break;
		case net::_GAME_OVER:
			handleGameOver();
			break;
		case net::_DISCONNECTING:
			handleDisconnecting();
			break;
		case net::_BULLET_SHOT:
			handleBulletShot();
			break;
		case net::_FIGHTER_TR:
			handleFighterTr();
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

bool NetworkSystem::initClient(std::string& name) {

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

	net::ConnReqMsg m;

	m.id = net::_CONNECTION_REQUEST;
	string_to_chars(name, m.name);
	p_->address = otherPlayerAddr_;
	SDLNetUtils::serializedSend(m, p_, sock_);

	if (SDLNet_CheckSockets(sockSet_, 3000) > 0) {
		if (SDLNet_SocketReady(sock_)) {
			SDLNetUtils::deserializedReceive(m, p_, sock_);
			if (m.id == net::_REQUEST_ACCEPTED) {
				net::ReqAccMsg m;
				m.deserialize(p_->data);
				fighter_ = m.fighter;
				chars_to_string(names[1-m.fighter], m.name);
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

void NetworkSystem::handleConnectionRequest() {

	if (!connected_ && host_) {
		net::ConnReqMsg cReq;
		cReq.deserialize(p_->data);
		chars_to_string(names[1 - fighter_], cReq.name);
		mngr_->getComponent<FighterInfo>(mngr_->getEntities(ecs::_grp_FIGHTERS)[1 - fighter_])->id_ = names[1-fighter_];

		otherPlayerAddr_ = p_->address;
		connected_ = true;
		net::ReqAccMsg m;
		m.id = net::_REQUEST_ACCEPTED;
		m.fighter = 1 - fighter_;
		string_to_chars(names[fighter_], m.name);
		SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);
	}
}

void NetworkSystem::sendBulletTr(Transform* tr)
{
	net::BulletShotMsg m;
	m.id = net::_BULLET_SHOT;
	m.x = tr->pos_.getX();
	m.y = tr->pos_.getY();
	m.rot = tr->rot_;
	m.velX = tr->vel_.getX();
	m.velY = tr->vel_.getY();
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

void NetworkSystem::sendFighterTr(Transform* tr)
{
	net::FighterPosMsg m;
	m.id = net::_FIGHTER_TR;
	m.fighter = fighter_;
	m.x = tr->pos_.getX();
	m.y = tr->pos_.getY();
	m.rot = tr->rot_;
	
	p_->address = otherPlayerAddr_;
	SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);
}

void NetworkSystem::handleStartGameRequest() {
	mngr_->getSystem<GameCtrlSystem>()->startGame();
}


void NetworkSystem::handleStartTheGame() {
	assert(!host_);
	mngr_->getSystem<GameCtrlSystem>()->startGame();
}


void NetworkSystem::handleGameOver() {
	net::GameOverMsg m;
	m.deserialize(p_->data);
	std::string s;
	chars_to_string(s, m.name);
	mngr_->getSystem<GameCtrlSystem>()->gameOver(s);
}

void NetworkSystem::handleDisconnecting() {
	connected_ = false;
	host_ = true;
	mngr_->getSystem<GameCtrlSystem>()->stopTheGame();
}

void NetworkSystem::handleFighterTr()
{
	net::FighterPosMsg m;

	m.deserialize(p_->data);
	mngr_->getSystem<FightersSystem>()->setFighterPos(m.fighter, m.x, m.y, m.rot);
}

void NetworkSystem::handleBulletShot()
{
	net::BulletShotMsg m;
	m.deserialize(p_->data);

	mngr_->getSystem<BulletsSystem>()->placeBullet(m.x, m.y, m.rot, m.velX, m.velY);
}

void NetworkSystem::tellOtherClientToStartGame() {
	net::Message m;

	m.id = net::_START_THE_GAME;
	SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);
}

void NetworkSystem::tellOtherClientGameOver(std::string name)
{
	net::GameOverMsg m;
	m.id = net::_GAME_OVER;
	string_to_chars(name, m.name);
	SDLNetUtils::serializedSend(m, p_, sock_, otherPlayerAddr_);
}

void NetworkSystem::string_to_chars(std::string& str, char c_str[11])
{
	auto i = 0u;
	for (; i < str.size() && i < 10; i++) c_str[i] = str[i];
	c_str[i] = 0;
}

void NetworkSystem::chars_to_string(std::string& str, char c_str[11])
{
	c_str[10] = 0;
	str = std::string(c_str);
}

