#define DEBUG
#define DISABLE_EVENTS_SENDING
// #define DISABLE_STATE_SENDING


#include "rtserver/RealtimeServer.hpp"


constexpr int EVENTS_BUFFER_SIZE = 65536;
constexpr int STATE_BUFFER_SIZE = 16384;
// using namespace clserv;

void RealtimeServer::onClientDisconnected(PlayerDisconnected event)
{
	_clients.remove(event.id);
}

RealtimeServer::RealtimeServer(int port, uint8_t serverCapacity) :
	_acceptor(port),
	_serverCapacity(serverCapacity),
	_eventsStream(EVENTS_BUFFER_SIZE),
	_gameStateStream(STATE_BUFFER_SIZE)
{
	std::cout << "Hello from realtime server\n";
	//_clients.reserve(_serverCapacity);
}

void RealtimeServer::start()
{
	std::thread([this]{_listen();}).detach();
	std::thread([this]{_mainLoop();}).detach();
}

void RealtimeServer::_listen()
{
	_isListening = true;
	_acceptor.listen();
	
	while (_isListening)
	{
		auto result = _acceptor.accept();
		if (result.is_error()){
			std::cerr << "Error when accepting\n";
			break;
		}
		
		ClientSocket* newClient = new ClientSocket(std::move(result.release()));
		size_t newId = _clients.add(newClient);
		std::cout << "Client connected and got id = " << newId << std::endl;
		newClient->readyEvent = [this](ClientSocket* socket){
			this->_justConnectedClients->push(socket);
		};


// Hello from realtime server
// Client connected and got id = 0
// 1
// Authorized id = 0
// _authorizedPlayers.size = 1
// Client connected and got id = 1
// 1
// Authorized id = 1
// _authorizedPlayers.size = 2
// Client connected and got id = 2
// 1
// Client connected and got id = 3
// 1
// Authorized id = 2
// _authorizedPlayers.size = 3
// Authorized id = 0
// _authorizedPlayers.size = 3
// Writed _authorizedPlayers->size()3


		// 
		// _connectActions->push([this, newClient]() {
		// 	size_t newId = _clients.add(newClient);
		// 	newClient->authorize(newId);
		// 	return newClient;
		// 	});

		newClient->connectedEvent = [this](PlayerAction<Authorized> event) {
			std::cout << "connectEvent Authorized id = " << (int)event.id << std::endl;
			_eventHandler.onClientAuthorized(event);
			_game.onPlayerAuthorized(event);
			};
		newClient->spawnedEvent = [this](PlayerAction<Spawned> event) {
			_eventHandler.onClientSpawned(event);
			_game.onPlayerSpawned(event);
			};
		newClient->movedEvent = [this](PlayerAction<Moved> event) {
			_eventHandler.onClientMoved(event);
			_game.onPlayerMoved(event);
			};
		newClient->rotatedEvent = [this](PlayerAction<Rotated> event) {
			_eventHandler.onClientRotated(event);
			_game.onPlayerRotated(event);
			};
		newClient->shotEvent = [this](PlayerAction<Shot> event) {
			_eventHandler.onClientShot(event); };
		newClient->enemyKilledEvent = [this](PlayerAction<EnemyKilled> event) {
			_eventHandler.onClientEnemyKilled(event);
			_game.onPlayerKilled(event);
			};
		newClient->bulletSpawnedEvent = [this](PlayerAction<BulletSpawned>  event) {
			//std::cout << "onBulletSpawnedEvent\tid = " << (int)event.id << "\tbulletId = " << (int)event.package.bulletId << std::endl;
			_eventHandler.onClientBulletSpawned(event);
			_game.onBulletSpawned(event);
			};
		newClient->bulletMovedEvent = [this](PlayerAction<BulletMoved> event) {
			//std::cout << "onBulletMovedEvent\tid = " << (int)event.id << "\tbulletId = " << (int)event.package.bulletId << std::endl;
			_eventHandler.onClientBulletMoved(event);
			_game.onBulletMoved(event);
			};
		newClient->bulletCollidedEvent = [this](PlayerAction<BulletCollided> event) {
			//std::cout << "onBulletCollidedEvent\tid = " << (int)event.id << "\tbulletId = " << (int)event.package.bulletId << std::endl;
			_eventHandler.onClientBulletCollided(event);
			_game.onBulletCollided(event);
			};
		newClient->bulletDissapearedEvent = [this](PlayerAction<BulletDissapeared> event) {\
			//std::cout << "onBulletDissapearEvent\tid = " << (int)event.id << "\tbulletId = " << (int)event.package.bulletId << std::endl;
			_eventHandler.onClientBulletDissapeared(event);
			_game.onBulletDissapeared(event);
			};

		newClient->disconnected = [this](PlayerDisconnected event) {
			onClientDisconnected(event);
			_eventHandler.onClientDisconnected(event);
			_game.onPlayerDisconnected(event);
			};

		// Authorize client only after all events are set
		newClient->authorize(newId);

	}
}

void RealtimeServer::_mainLoop()
{
	while (true)
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		if (_justConnectedClients->size() > 0){
			_gameStateStream.clear();
			_gameStateStream << byte(0);
			_game.writeState(_gameStateStream);
			std::cout << "_gameStateStream length = " << _gameStateStream.getLength() << std::endl;
			unsigned char* message = new unsigned char[_gameStateStream.getLength()];
			size_t size = _gameStateStream.getLength();
			_gameStateStream.getBuf(message);
			if (size >= 18)
				std::cout << "18th byte = " << (int)message[18] << std::endl;
			if (size >= 34)
				std::cout << "34th byte = " << (int)message[34] << std::endl;
			for (int i = 0; i < _justConnectedClients->size(); i++) {
				ClientSocket* client = _justConnectedClients->front();
				_justConnectedClients->pop();
#ifndef DISABLE_STATE_SENDING
				client->sendState((char*)message, size);
#endif
			}
		}

// 		if (_connectActions->size() > 0)
// 		{
// 			_gameStateStream.clear();
// 			_game.writeState(_gameStateStream);
// 			unsigned char* message = new unsigned char[_gameStateStream.getLength()];
// 			size_t size = _gameStateStream.getLength();
// 			_gameStateStream.getBuf(message);
// 				for (int i = 0; i < _connectActions->size(); i++) {
// 					ClientSocket* client = _connectActions->front()();
// 					_connectActions->pop();
// #ifdef DEBUG
// 					std::cout
// 						<< "mailLoop; "
// 						<< "_allConnectedPlayers.size() = "
// 						<< (int)message[0]
// 						<< "; message.size() = "
// 						<< (int)size
// 						<< std::endl;
// 					//char size;
// 					//if (message[0] == 3)
// 					//	size = message[0];
// 					//size = message[0];
// 					//for (int j = 0; j < size; j++) {
// 					//	int index = 16 * j + 1;
// 					//	std::cout << "sended id " << (int)message[index] << std::endl;
// 					//}
// #endif // DEBUG
// #ifndef DISABLE_STATE_SENDING
// 					client->sendState((char*)message, size);
// #endif
// 				}
// 		}
		//std::cout << "Server loop\n";
		_eventsStream.clear();
		_eventsStream << byte(0);
		_eventHandler.writeEvents(_eventsStream);
		
		unsigned int length = _eventsStream.getLength();
		unsigned char* buffer = new unsigned char[length];
		_eventsStream.getBuf(buffer);
		auto& clients = _clients.getMap();
		//ClientSocket::printAligned((char*)buffer, length, 4);
		for (auto& client : clients) {
			if (client.second->isConnected()) {
				client.second->send((char*)buffer, length);
			}
		}

		auto endTime = std::chrono::high_resolution_clock::now();
		double delta = std::chrono::duration<double, std::micro>(endTime - startTime).count();
		const float tickRate = 0.5;
		const int sec_microsec = 1000000;
		int period = std::micro::den / tickRate;
		if (delta < period)
			std::this_thread::sleep_for(std::chrono::microseconds(period - (long long)delta));
	}
}