#include "rtserver/PackageFromPlayer.hpp"

std::string PackageFromPlayer::toString() const
{
    std::stringstream stream;
    if (fixed.spawned.hasValue) {
        stream << "spawned {\n\t";
        stream << "rotation: " << fixed.spawned.value.transform.rotation << std::endl;
        stream << "position: " << fixed.spawned.value.transform.position.x << ',' << fixed.spawned.value.transform.position.y << std::endl;
        stream << "}\n";
    }

    if (fixed.moved.hasValue) {
        stream << "moved {\n\t";
        stream << "position: " << fixed.moved.value.position.x << ',' << fixed.moved.value.position.y << std::endl;
        stream << "}\n";
    }

    if (fixed.rotated.hasValue) {
        stream << "rotated {\n\t";
        stream << "rotation: " << fixed.rotated.value.rotation << std::endl;
        stream << "}\n";
    }

    if (fixed.shot.hasValue) {
        stream << "shot {\n\t";
        stream << "shot: " << (int)fixed.shot.value.shotType << std::endl;
        stream << "}\n";
    }

    if (fixed.enemyKilled.hasValue) {
        stream << "enemyKilled {\n\t";
        stream << "enemyId: " << (int)fixed.enemyKilled.value.enemyId << std::endl;
        stream << "}\n";
    }
    
    if (!spawnedBullets.empty()) {
        stream << "spawnedBullets {\n";
        for (auto bullet : spawnedBullets) {
            stream << "bulletId: " << (int)bullet.bulletId << std::endl;
            stream << "rotation: " << bullet.transform.rotation << std::endl;
            stream << "position: " << bullet.transform.position.x << ',' << bullet.transform.position.y << std::endl;
        }
        stream << "}\n";
    }

    if (!movedBullets.empty()) {
        stream << "movedBullets {\n";
        for (auto bullet : movedBullets) {
            stream << "bulletId: " << (int)bullet.bulletId << std::endl;
            stream << "position: " << bullet.position.x << ',' << bullet.position.y << std::endl;
        }
        stream << "}\n";
    }

    if (!collidedBullets.empty()) {
        stream << "collidedBullets {\n";
        for (auto bullet : collidedBullets) {
            stream << "bulletId: " << (int)bullet.bulletId << std::endl;
        }
        stream << "}\n";
    }

    if (!dissapearedBullets.empty()) {
        stream << "dissapearedBullets {\n";
        for (auto bullet : dissapearedBullets) {
            stream << "bulletId: " << (int)bullet.bulletId << std::endl;
        }
        stream << "}\n";
    }

    return stream.str();
}

PackageFromPlayer PackageFromPlayer::readFromStream(ByteStream stream)
{
    PackageFromPlayer pack;
    pack.fixed = stream.read<FixedPackage>();

    byte spawnedCount = stream.readByte();
    //std::cout << "spawnedCount = " << (int)spawnedCount << std::endl;
    pack.spawnedBullets.reserve(spawnedCount);
    for (byte i = 0; i < spawnedCount; i++)
        pack.spawnedBullets.push_back(stream.read<BulletSpawned>());

    byte movedCount = stream.readByte();
    //std::cout << "movedCount = " << (int)movedCount << std::endl;
    pack.movedBullets.reserve(movedCount);
    for (byte i = 0; i < movedCount; i++)
        pack.movedBullets.push_back(stream.read<BulletMoved>());

    byte collidedCount = stream.readByte();
    //std::cout << "collidedCount = " << (int)collidedCount << std::endl;
    pack.collidedBullets.reserve(collidedCount);
    for (byte i = 0; i < collidedCount; i++) {
        pack.collidedBullets.push_back(stream.read<BulletCollided>());
    }

    byte dissapearedCount = stream.readByte();
    //std::cout << "dissapearedCount = " << (int)dissapearedCount << std::endl;
    pack.dissapearedBullets.reserve(dissapearedCount);
    for (byte i = 0; i < dissapearedCount; i++)
        pack.dissapearedBullets.push_back(stream.read<BulletDissapeared>());

    return pack;
}
