
#ifndef GAME_HPP
#define GAME_HPP


#include <map>
#include <optional>


#include "rtserver/PackageStructs.hpp"
#include "rtserver/LambdaMap.hpp"
#include "safe_ptr.h"
#include "ByteStream.hpp"

using BulletId = Id;

class Bullet {
private:
	Transform _transform;
	BulletId _id;

	void spawn(Transform transform);
public:

	Bullet(BulletId id, Transform transform);
	void move(Vector2 position);

	Transform getTransform() const;
};

#define PLAYER_BULLET BulletId, Bullet*
//using PlayerBullet = std::pair<PLAYER_BULLET>;
using PlayerBullets = sf::safe_ptr<LambdaMap<PLAYER_BULLET>>;

using PlayerId = Id;

class Player {
private:
	
	StringName<NICKNAME_SIZE> _name;
	ByteNullable<Transform> _transform;
	PlayerBullets _bullets;
	PlayerId _id;

public:

	Player(PlayerId id, StringName<NICKNAME_SIZE> name);

	size_t writeBullets(ByteStream& stream);

	bool isSpawned();
	void spawn(Transform transform);
	void move(Vector2 position);
	void rotate(float rotation);
	void despawn();

	Bullet* spawnBullet(BulletSpawned event);
	void moveBullet(BulletMoved event);
	void despawnBullet(BulletId id);

	StringName<NICKNAME_SIZE> getName() const;

	// Can be optimized
	Transform getTransform() const;
};

using Players = LambdaMap<PlayerId, Player*>;

struct GlobalBulletId {
	PlayerId playerId;
	BulletId bulletId;

	GlobalBulletId();
	GlobalBulletId(PlayerId playerId, BulletId bulletId);
	GlobalBulletId(unsigned short value);
	operator uint16_t();
};

using Bullets = LambdaMap<uint16_t, Bullet*>;

class Game
{
	sf::safe_ptr<Players> _authorizedPlayers;
	sf::safe_ptr<Players> _spawnedPlayers;
	sf::safe_ptr<Bullets> _bullets;

	void despawnBullet(PlayerId playerId, BulletId bulletId);

public:

	size_t writeState(ByteStream& stream);

	void onPlayerAuthorized(PlayerAction<Authorized> authrized);
	void onPlayerSpawned(PlayerAction<Spawned> spawned);
	void onPlayerMoved(PlayerAction<Moved> moved);
	void onPlayerRotated(PlayerAction<Rotated> rotated);
	void onPlayerKilled(PlayerAction<EnemyKilled> killed);
	void onPlayerDisconnected(PlayerDisconnected disconnected);

	void onBulletSpawned(PlayerAction<BulletSpawned> event);
	void onBulletMoved(PlayerAction<BulletMoved> event);
	void onBulletCollided(PlayerAction<BulletCollided> event);
	void onBulletDissapeared(PlayerAction<BulletDissapeared> event);
};

#endif