#include "rtserver/EventHandler.hpp"

void EventHandler::onClientAuthorized(PlayerAction<Authorized> event){
    _authorizedClients->push_back(event);
}

void EventHandler::onClientSpawned(PlayerAction<Spawned> event){
    _spawnedClients->push_back(event);
}

void EventHandler::onClientMoved(PlayerAction<Moved> event){
    _movedClients->push_back(event);
}

void EventHandler::onClientRotated(PlayerAction<Rotated> event){
    _rotatedClients->push_back(event);
}

void EventHandler::onClientShot(PlayerAction<Shot> event){
    _shotClients->push_back(event);
}

void EventHandler::onClientEnemyKilled(PlayerAction<EnemyKilled> event){
    _enemyKilledClients->push_back(event);
}

void EventHandler::onClientDisconnected(PlayerDisconnected event){
    _disconnectedClients->push_back(event);
}

void EventHandler::onClientBulletSpawned(PlayerAction<BulletSpawned> event){
    _bulletSpawnedClients->push_back(event);
}

void EventHandler::onClientBulletMoved(PlayerAction<BulletMoved> event){
    _bulletMovedClients->push_back(event);
}

void EventHandler::onClientBulletCollided(PlayerAction<BulletCollided> event){
    _bulletCollidedClients->push_back(event);
}

void EventHandler::onClientBulletDissapeared(PlayerAction<BulletDissapeared> event){
    _bulletDissapearedClients->push_back(event);
}

void EventHandler::writeEvents(ByteStream &stream){
    _authorizedClients->writeAllToStream(stream);
    _spawnedClients->writeAllToStream(stream);
    _movedClients->writeAllToStream(stream);
    _rotatedClients->writeAllToStream(stream);
    _shotClients->writeAllToStream(stream);
    _enemyKilledClients->writeAllToStream(stream);
    _disconnectedClients->writeAllToStream(stream);
    _bulletSpawnedClients->writeAllToStream(stream);
    _bulletMovedClients->writeAllToStream(stream);
    _bulletCollidedClients->writeAllToStream(stream);
    _bulletDissapearedClients->writeAllToStream(stream);
}

std::array<std::pair<std::string, byte>, 11> EventHandler::getSizes()
{

    return {
        std::pair{"_authorizedClients: ", _authorizedClients->size()},
        std::pair{"_spawnedClients: ", _spawnedClients->size()},
        std::pair{"_movedClients: ", _movedClients->size()},
        std::pair{"_rotatedClients: ", _rotatedClients->size()},
        std::pair{"_shotClients: ", _shotClients->size()},
        std::pair{"_disconnectedClients: ", _disconnectedClients->size()},
        std::pair{"_bulletSpawnedClients: ", _bulletSpawnedClients->size()},
        std::pair{"_bulletMovedClients: ", _bulletMovedClients->size()},
        std::pair{"_bulletCollidedClients: ", _bulletCollidedClients->size()},
        std::pair{"_bulletDissapearedClients: ", _bulletDissapearedClients->size()},
        };
}
