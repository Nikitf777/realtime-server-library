#ifndef REALTIME_SERVER_HPP
#define REALTIME_SERVER_HPP

#include <sockpp/tcp_acceptor.h>
#include "rtserver/ClientSocket.hpp"
#include "rtserver/AutoMap.hpp"
#include "rtserver/EventHandler.hpp"
#include "Game.hpp"



class RealtimeServer
{
public:
	struct DataPackage {
		float x, y;
	};
	
private:
	sockpp::tcp_acceptor _acceptor;

	AutoMap<ClientSocket*> _clients;
	sf::safe_ptr<std::queue<std::function<ClientSocket*()>>> _connectActions;
	sf::safe_ptr<std::queue<ClientSocket*>> _justConnectedClients;
	std::vector<DataPackage> _dataToSend;
	const uint8_t _serverCapacity;
	EventHandler _eventHandler;
	Game _game;

	ByteStream _eventsStream;
	ByteStream _gameStateStream;

	bool _isListening;

	void onClientDisconnected(PlayerDisconnected event);

	void _listen();
	void _mainLoop();

public:


	RealtimeServer(int port, uint8_t serverCapacity);

	void start();

};

#endif