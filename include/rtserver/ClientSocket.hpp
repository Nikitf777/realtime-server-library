
#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

#include <sockpp/tcp_socket.h>
#include <optional>
#include <array>
#include <atomic>
#include <mutex>
#include <functional>
#include <thread>

#include "asyncpp/task.h"
#include "asyncpp/event.h"

#include "ByteStream.hpp"
#include "rtserver/PackageStructs.hpp"
#include "rtserver/PackageFromPlayer.hpp"

constexpr int _receiveBufferSize = 16384;

class ClientSocket {
private:
	sockpp::tcp_socket _socket;
	std::atomic<bool> _stop;
	std::mutex _mutex;
	byte _id;
	std::array<char, 15> _name;
	asyncpp::single_consumer_event event;


	char* _receiveBuffer;

	bool _isConnected = false;
	//std::optional<Spawned> _spawned;
	//std::optional<Moved> _moved;
	//std::optional<float> _rotated;
	//bool _shot;
	//std::optional<byte> _enemyKilled;
	//std::optional<byte> _bulletSpawned;
	//std::optional<BulletMoved> _bulletMoved;
	//std::optional<byte> _bulletCollided;
	//std::optional<byte> _bulletDissapeared;

	void getSpawned(ByteStream& stream);
	void getMoved(ByteStream& stream);
	void getRotated(ByteStream& stream);
	void getShot(ByteStream& stream);
	void getEnemyKilled(ByteStream& stream);
	void getBulletSpawned(ByteStream& stream);
	void getBulletMoved(ByteStream& stream);
	void getBulletCollided(ByteStream& stream);
	void getBulletDissapeared(ByteStream& stream);

	void emitSignals(PackageFromPlayer& package) const;

	void _authorize();


public:

	std::function<void(ClientSocket*)> readyEvent;

	std::function<void(PlayerAction<Authorized>)> connectedEvent;
	std::function<void(PlayerAction<Spawned>)> spawnedEvent;
	std::function<void(PlayerAction<Moved>)> movedEvent;
	std::function<void(PlayerAction<Rotated>)> rotatedEvent;
	std::function<void(PlayerAction<Shot>)> shotEvent;
	std::function<void(PlayerDisconnected)> disconnected;
	std::function<void(PlayerAction<EnemyKilled>)> enemyKilledEvent;
	std::function<void(PlayerAction<BulletSpawned>)> bulletSpawnedEvent;
	std::function<void(PlayerAction<BulletMoved>)> bulletMovedEvent;
	std::function<void(PlayerAction<BulletCollided>)> bulletCollidedEvent;
	std::function<void(PlayerAction<BulletDissapeared>)> bulletDissapearedEvent;

	ClientSocket(sockpp::tcp_socket socket);
	void authorize(byte id);
	void receiving();
	void stop();
	void send(const char* buffer, int size);
	void sendState(const char* buffer, int size);

	bool isConnected() const;

	static void printAligned(char* buffer, size_t size, unsigned char alignment);
};

#endif // CLIENTSOCKET_HPP