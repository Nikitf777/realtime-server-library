#include "rtserver/Game.hpp"

void Bullet::spawn(Transform transform)
{
	_transform = transform;
}

Bullet::Bullet(BulletId id, Transform transform)
{
	spawn(transform);
	_id = id;
}

void Bullet::move(Vector2 position)
{
	_transform.position = position;
}

Transform Bullet::getTransform() const
{
	return _transform;
}

Player::Player(PlayerId id, StringName<NICKNAME_SIZE> name)
{
	_transform.hasValue = false;
	_name = name;
	_id = id;
}

size_t Player::writeBullets(ByteStream& stream)
{
	size_t initialSize = stream.getLength();

	for (auto i = _bullets->begin(); i != _bullets->end(); ++i)
		stream << PlayerAction<BulletSpawned>{_id, { (*i).first, (*i).second->getTransform() }};

	return stream.getLength() - initialSize;
}

bool Player::isSpawned()
{
	return _transform.hasValue;
}

void Player::spawn(Transform transform)
{
	_transform.hasValue = true;
	_transform.value = transform;
}

void Player::move(Vector2 position)
{
	_transform.value.position = position;
}

void Player::rotate(float rotation)
{
	_transform.value.rotation = rotation;
}

void Player::despawn()
{
	_transform.hasValue = false;
}

Bullet* Player::spawnBullet(BulletSpawned event)
{
	Bullet* bullet = new Bullet(event.bulletId, event.transform);
	_bullets->operator[](event.bulletId) = bullet;
	return bullet;
}

void Player::moveBullet(BulletMoved event)
{
	_bullets->operator[](event.bulletId)->move(event.position);
}

void Player::despawnBullet(BulletId id)
{
	auto bulletIter = _bullets->find(id);
	if (bulletIter != _bullets->end()) {
		delete (*bulletIter).second;
		_bullets->erase(bulletIter);
	}
}

StringName<NICKNAME_SIZE> Player::getName() const
{
	return _name;
}

Transform Player::getTransform() const
{
	if (_transform.hasValue)
		return _transform.value;
	throw std::exception();
}

void Game::despawnBullet(PlayerId playerId, BulletId bulletId)
{
	_spawnedPlayers->operator[](playerId)->despawnBullet(bulletId);
	_bullets->erase(GlobalBulletId{ playerId, bulletId });
}

size_t Game::writeState(ByteStream& stream)
{
	size_t initialLength = stream.getLength();

	_authorizedPlayers->runFunction([&stream] (Players* thisMap) {
		stream << (byte)thisMap->size();
		std::cout << "Writed _authorizedPlayers->size() = " << (int)thisMap->size() << std::endl;
		for (auto i = thisMap->begin(); i != thisMap->end(); ++i) {
			std::cout << "Writed id = " << (int)(*i).first << std::endl;
			stream << PlayerAction<Authorized>{PlayerId{ (*i).first }, Authorized{ (*i).second->getName() }};
		}
		});

	_spawnedPlayers->runFunction([&stream](Players* thisMap) {
		stream << byte(thisMap->size());
		for (auto i = thisMap->begin(); i != thisMap->end(); ++i) {
			stream << PlayerAction<Spawned>{ PlayerId{ (*i).first }, Spawned{ (*i).second->getTransform() } };
		}
		});

	_bullets->runFunction([&stream](Bullets* thisMap) {
		stream << byte(thisMap->size());
		for (auto i = thisMap->begin(); i != thisMap->end(); ++i) {
			stream << PlayerAction<BulletSpawned>{ GlobalBulletId((*i).first).playerId, BulletSpawned{ GlobalBulletId((*i).first).bulletId, (*i).second->getTransform() }};
		}
		});

	
	//stream << byte(_authorizedPlayers->size());
	//std::cout << "Writed _authorizedPlayers->size()" << (int)_authorizedPlayers->size() << std::endl;
	//for (auto i = _authorizedPlayers->begin(); i != _authorizedPlayers->end(); ++i) {
	//	stream << PlayerAction<Authorized>{ PlayerId{ (*i).first }, Authorized{ (*i).second->getName() } };
	//}
	//
	//stream << byte(_spawnedPlayers->size());
	//for (auto i = _spawnedPlayers->begin(); i != _spawnedPlayers->end(); ++i) {
	//	stream << PlayerAction<Spawned>{ PlayerId{ (*i).first }, Spawned{ (*i).second->getTransform() } };
	//}
	//
	//stream << byte(_bullets->size());
	//for (auto i = _bullets->begin(); i != _bullets->end(); ++i) {
	//	stream << PlayerAction<BulletSpawned>{ GlobalBulletId((*i).first).playerId, BulletSpawned{ GlobalBulletId((*i).first).bulletId, (*i).second->getTransform() }};
	//}

	return stream.getLength() - initialLength;
}

void Game::onPlayerAuthorized(PlayerAction<Authorized> authrized)
{
	// _authorizedPlayers->operator[](authrized.id) = new Player(authrized.id, authrized.package.name);
	std::cout << "Authorized id = " << (int)authrized.id << std::endl << std::endl;
	_authorizedPlayers->insert({authrized.id, new Player(authrized.id, authrized.package.name)});
	std::cout << "_authorizedPlayers.size = " << _authorizedPlayers->size() << std::endl;
}

void Game::onPlayerSpawned(PlayerAction<Spawned> spawned)
{
	Player* player = _authorizedPlayers->operator[](spawned.id);
	player->spawn(spawned.package.transform);
	_spawnedPlayers->operator[](spawned.id) = player;
}

void Game::onPlayerMoved(PlayerAction<Moved> moved)
{
	_spawnedPlayers->operator[](moved.id)->move(moved.package.position);
}

void Game::onPlayerRotated(PlayerAction<Rotated> rotated)
{
	_spawnedPlayers->operator[](rotated.id)->rotate(rotated.package.rotation);
}

void Game::onPlayerKilled(PlayerAction<EnemyKilled> killed)
{
	auto playerIter = _spawnedPlayers->find(killed.id);
	(*playerIter).second->despawn();
	_spawnedPlayers->erase(playerIter);
}

void Game::onPlayerDisconnected(PlayerDisconnected disconnected)
{
	auto playerIter = _authorizedPlayers->find(disconnected.id);
	delete (*playerIter).second;
	_spawnedPlayers->erase(playerIter);
	_authorizedPlayers->erase(playerIter);
}

void Game::onBulletSpawned(PlayerAction<BulletSpawned> event)
{
	auto bullet = _bullets->operator[](GlobalBulletId{ event.id, event.package.bulletId }) = _spawnedPlayers->operator[](event.id)->spawnBullet(event.package);
	//std::cout << "onBulletSpawned\tbulletId = " << int(event.package.bulletId) << "\t_bullets.size() = " << _bullets->size() << std::endl;
	if (bullet == nullptr)
		std::cerr << "bullet was null\n";
}

void Game::onBulletMoved(PlayerAction<BulletMoved> event)
{
	_spawnedPlayers->operator[](event.id)->moveBullet(event.package);
}

void Game::onBulletCollided(PlayerAction<BulletCollided> event)
{
	//std::cout << "onBulletCollided\n";
	despawnBullet(event.id, event.package.bulletId);
}

void Game::onBulletDissapeared(PlayerAction<BulletDissapeared> event)
{
	//std::cout << "onBulletDissapeared\n";
	despawnBullet(event.id, event.package.bulletId);
}

GlobalBulletId::GlobalBulletId()
{
	playerId = 0;
	bulletId = 0;
}

GlobalBulletId::GlobalBulletId(PlayerId playerId, BulletId bulletId)
{
	this->playerId = playerId;
	this->bulletId = bulletId;
}

GlobalBulletId::GlobalBulletId(unsigned short value)
{
	byte* bytes = (byte*)&value;
	playerId = bytes[0];
	bulletId = bytes[1];
}

GlobalBulletId::operator uint16_t()
{
	byte bytes[2] = { playerId, bulletId };
	return *(uint16_t*)bytes;
}
