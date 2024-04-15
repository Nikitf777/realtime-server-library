
#ifndef PACKAGE_FROM_PLAYER_HPP
#define PACKAGE_FROM_PLAYER_HPP
#include <optional>
#include <sstream>
#include <vector>


#include "rtserver/PackageStructs.hpp"
#include "ByteStream.hpp"


struct FixedPackage {
	ByteNullable<Spawned> spawned;
	ByteNullable<Moved> moved;
	ByteNullable<Rotated> rotated;
	ByteNullable<Shot> shot;
	ByteNullable<EnemyKilled> enemyKilled;
};


struct PackageFromPlayer {
	FixedPackage fixed;

	std::vector<BulletSpawned> spawnedBullets;
	std::vector<BulletMoved> movedBullets;
	std::vector<BulletCollided> collidedBullets;
	std::vector<BulletDissapeared> dissapearedBullets;

	std::string toString() const;

	static PackageFromPlayer readFromStream(ByteStream stream);
};

//struct PackageFromPlayer {
//	std::optional<Spawned> spawned;
//	std::optional<Moved> moved;
//	std::optional<Rotated> rotated;
//	std::optional<Shot> shot;
//	std::optional<EnemyKilled> enemyKilled;
//	std::optional<BulletSpawned> bulletSpawned;
//	std::optional<BulletMoved> bulletMoved;
//	std::optional<BulletCollided> bulletCollided;
//	std::optional<BulletDissapeared> bulletDissapeared;
//
//	std::string toString() const;
//};

#endif