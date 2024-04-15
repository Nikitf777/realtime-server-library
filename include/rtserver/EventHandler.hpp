
#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP
#include <array>
// #include "Include.h"
// #include "SafeQueue.h"
// #include "SafeList.h"

#include "rtserver/PackageStructs.hpp"
#include "ByteStream.hpp"
#include "rtserver/StreamList.hpp"
#include "rtserver/AutoMap.hpp"
#include <unordered_map>
#include "safe_ptr.h"

class EventHandler
{
private:

	sf::safe_ptr<StreamList<PlayerAction<Authorized>>> _authorizedClients;
	sf::safe_ptr<StreamList<PlayerAction<Spawned>>> _spawnedClients;
	sf::safe_ptr<StreamList<PlayerAction<Moved>>> _movedClients;
	sf::safe_ptr<StreamList<PlayerAction<Rotated>>> _rotatedClients;
	sf::safe_ptr<StreamList<PlayerAction<Shot>>> _shotClients;
	sf::safe_ptr<StreamList<PlayerAction<EnemyKilled>>> _enemyKilledClients;
	sf::safe_ptr<StreamList<PlayerDisconnected>> _disconnectedClients;
	sf::safe_ptr<StreamList<PlayerAction<BulletSpawned>>> _bulletSpawnedClients;
	sf::safe_ptr<StreamList<PlayerAction<BulletMoved>>> _bulletMovedClients;
	sf::safe_ptr<StreamList<PlayerAction<BulletCollided>>> _bulletCollidedClients;
	sf::safe_ptr<StreamList<PlayerAction<BulletDissapeared>>> _bulletDissapearedClients;

public:

	void onClientAuthorized(PlayerAction<Authorized> event);
	void onClientSpawned(PlayerAction<Spawned> event);
	void onClientMoved(PlayerAction<Moved> event);
	void onClientRotated(PlayerAction<Rotated> event);
	void onClientShot(PlayerAction<Shot> event);
	void onClientEnemyKilled(PlayerAction<EnemyKilled> event);
	void onClientDisconnected(PlayerDisconnected event);
	void onClientBulletSpawned(PlayerAction<BulletSpawned> event);
	void onClientBulletMoved(PlayerAction<BulletMoved> event);
	void onClientBulletCollided(PlayerAction<BulletCollided> event);
	void onClientBulletDissapeared(PlayerAction<BulletDissapeared> event);

	void write(ByteStream& stream);

	std::array<std::pair<std::string, byte>, 11> getSizes();
};

#endif // EVENTHANDLER_HPP