#include "rtserver/ClientSocket.hpp"

//#define DEBUG
using std::optional;

void ClientSocket::getSpawned(ByteStream& stream)
{
	optional<Spawned> event;
	stream >> event;
	if (!event)
		return;

	if (spawnedEvent != nullptr)
		spawnedEvent({ _id, event.value()});
}

void ClientSocket::getMoved(ByteStream& stream)
{
	optional<Moved> event;
	stream >> event;
	if (!event)
		return;

	if (movedEvent != nullptr)
		movedEvent({ _id, event.value()});
}

void ClientSocket::getRotated(ByteStream& stream)
{
	optional<Rotated> event;
	stream >> event;
	if (!event)
		return;

	if (rotatedEvent != nullptr)
		rotatedEvent({_id, event.value()});
}
void ClientSocket::getShot(ByteStream& stream)
{
	optional<Shot> event;
	stream >> event;
	if (!event)
		return;

		if (shotEvent != nullptr)
			shotEvent({ _id, event.value() });
}
void ClientSocket::getEnemyKilled(ByteStream& stream)
{
	optional<EnemyKilled> event;
	stream >> event;
	if (!event)
		return;
	if (enemyKilledEvent != nullptr)
		enemyKilledEvent({ _id, event.value()});
}
void ClientSocket::getBulletSpawned(ByteStream& stream)
{
	optional<BulletSpawned> event;
	stream >> event;
	if (!event)
		return;


	if (bulletSpawnedEvent != nullptr)
		bulletSpawnedEvent({ _id, event.value() });
}
void ClientSocket::getBulletMoved(ByteStream& stream)
{
	optional<BulletMoved> event;
	stream >> event;
	if (!event)
		return;


	if (bulletMovedEvent != nullptr)
		bulletMovedEvent({ _id, event.value() });
}
void ClientSocket::getBulletCollided(ByteStream& stream)
{
	optional<BulletCollided> event;
	stream >> event;
	if (!event)
		return;


	if (bulletCollidedEvent != nullptr)
		bulletCollidedEvent({ _id, event.value() });
}
void ClientSocket::getBulletDissapeared(ByteStream& stream)
{
	optional<BulletDissapeared> event;
	stream >> event;
	if (!event)
		return;


	if (bulletDissapearedEvent != nullptr)
		bulletDissapearedEvent({ _id, event.value() });
}
void ClientSocket::emitSignals(PackageFromPlayer& package) const
{
	if (package.fixed.spawned.hasValue && spawnedEvent) {
		spawnedEvent({ _id, package.fixed.spawned.value });
	}

	if (package.fixed.moved.hasValue && movedEvent) {
		movedEvent({ _id, package.fixed.moved.value });
	}

	if (package.fixed.rotated.hasValue && rotatedEvent) {
		rotatedEvent({ _id, package.fixed.rotated.value });
	}

	if (package.fixed.shot.hasValue && shotEvent) {
		shotEvent({ _id, package.fixed.shot.value });
	}

	if (package.fixed.enemyKilled.hasValue && enemyKilledEvent) {
		enemyKilledEvent({ _id, package.fixed.enemyKilled.value });
	}

	if (bulletSpawnedEvent)
		for (auto& bullet : package.spawnedBullets) {
			//std::cout << "bulletSpawnedEvent\t" << "\tbulletId = " << (int)bullet.bulletId << std::endl;
			bulletSpawnedEvent({ _id, bullet });

		}

	if (bulletMovedEvent)
		for (auto& bullet : package.movedBullets) {
			//std::cout << "bulletMovedEvent\t" << "\tbulletId = " << (int)bullet.bulletId << std::endl;
			bulletMovedEvent({ _id, bullet });

		}

	if (bulletCollidedEvent)
		for (auto& bullet : package.collidedBullets) {
			//std::cout << "bulletCollidedEvent\t" << "\tbulletId = " << (int)bullet.bulletId << std::endl;
			bulletCollidedEvent({ _id, bullet });

		}

	if (bulletDissapearedEvent)
		for (auto& bullet : package.dissapearedBullets) {
			//std::cout << "bulletDissapearedEvent\t" << "\tbulletId = " << (int)bullet.bulletId << std::endl;
			bulletDissapearedEvent({ _id, bullet });

		}

}

void ClientSocket::printAligned(char* buffer, size_t size, unsigned char alignment)
{
	size_t rowCount = size / alignment;
	for (size_t i = 0; i < rowCount; i++) {
		std::cout << (unsigned int)(i * alignment) << ":\t";
		for (char j = 0; j < alignment; j++) {
			std::cout << (unsigned int)(unsigned char)buffer[i * alignment + j] << '\t';
		}
		std::cout << std::endl;
	}
}

ClientSocket::ClientSocket(sockpp::tcp_socket socket)
{
	_socket = std::move(socket);
	_stop = true;

	_receiveBuffer = new char[_receiveBufferSize];
}

void ClientSocket::authorize(byte id)
{
	_id = id;
	byte bufferB[1] = { _id };
	char* buffer = (char*)bufferB;
	send(buffer, sizeof bufferB);
	char nameBuffer[15];
	auto result = _socket.recv(nameBuffer, sizeof(nameBuffer));
	std::array<char, 15> name{};
	for (char i = 0; i < result.value(); i++)
		_name[i] = nameBuffer[i];
	PlayerAction<Authorized> event{ _id, name };
	if (connected != nullptr)
		connected(event);

	_connected = true;

	std::thread([this] {
		Receiving();
		}).detach();
}

void ClientSocket::Receiving()
{
	while (true)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		char stopBuffer[1] = { 1 };
		send((char*)stopBuffer, 1);
		auto receivedCount = _socket.recv(_receiveBuffer, _receiveBufferSize, 0);
		//ByteStream stream((unsigned char*)_receiveBuffer, receivedCount);
		ByteStream stream(_receiveBufferSize);
		stream << stream(_receiveBuffer, _receiveBufferSize);
		//getSpawned(stream);
		//getMoved(stream);
		//getRotated(stream);
		//getShot(stream);
		//getEnemyKilled(stream);
		//getBulletSpawned(stream);
		//getBulletMoved(stream);
		//getBulletCollided(stream);
		//getBulletDissapeared(stream);

#ifdef DEBUG
		std::cout << "Received " << receivedCount << " bytes\n";
		char xarr[] = { buffer[16], buffer[17], buffer[18], buffer[19] };
		float x = *(float*)xarr;
		std::cout << "24 byte = " << (bool)buffer[24] << std::endl;
		std::cout << "moved.position.x (from buffer) = " << x << std::endl;
		
#endif // !DEBUG

		auto package = PackageFromPlayer::readFromStream(stream);
		//std::cout << package.toString() << std::endl;
		emitSignals(package);
		//std::cout << package.toString();

		auto endTime = std::chrono::high_resolution_clock::now();
		double delta = std::chrono::duration<double, std::micro>(endTime - startTime).count();
		const float tickRate = 0.5;
		const int sec_microsec = 1000000;
		int period = std::micro::den / tickRate;
		if (delta < period)
			std::this_thread::sleep_for(std::chrono::microseconds(period - (long long)delta));
	}
}

void ClientSocket::stop()
{
	_stop = true;
}

void ClientSocket::send(const char* buffer, int size)
{
	_mutex.lock();
	try
	{
		std::cout << (_socket.send((void*)buffer, size)).value() << std::endl;
	}
	catch (const std::exception&)
	{
		std::cout << "Player " << _id << " disconnected\n";
	}
	_mutex.unlock();
}

bool ClientSocket::isConnected() const
{
	return _connected;
}
