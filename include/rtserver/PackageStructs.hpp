#ifndef PACKAGESTRUCTS_HPP
#define PACKAGESTRUCTS_HPP
//#include "Include.h"
#include <array>
#include <stdint.h>

//using size_t = long long int;
constexpr std::size_t NICKNAME_SIZE = 15;

// Basic types
struct Vector2 { float x, y; };
struct Transform {
	float rotation;
	Vector2 position;
};


using byte = unsigned char;
using Id = byte;

template<std::size_t Size>
// using StringName = std::array<char, Size>;
struct StringName : public std::array<char, Size>{};


// Event types

struct Authorized { StringName<NICKNAME_SIZE> name; };
struct Spawned { Transform transform; };
struct Moved { Vector2 position; };
struct Rotated { float rotation; };
struct Shot { byte shotType; };
struct EnemyKilled { Id enemyId; };
struct BulletSpawned { Id bulletId; Transform transform; };
struct BulletMoved { Id bulletId; Vector2 position; };
struct BulletCollided { Id bulletId; };
struct BulletDissapeared { Id bulletId; };
struct Disconnected {};

template<typename TPackage>
struct PlayerAction {
	Id id;
	TPackage package;
};

template<typename T>
struct ByteNullable {
	T value;
	bool hasValue;
};

struct PlayerShot {
	Id id;
};

struct PlayerDisconnected {
	Id id;
};

//struct Bullet {
//	Transform transform;
//};
//
//struct Player {
//	Transform transform;
//};

struct Struct1 {
	uint8_t a; // 1 byte
	int32_t b; // 4 bytes
	uint8_t d; // 1 byte
};

struct Struct2 {
	uint8_t a; // 1 byte
	uint8_t b; // 1 byte
	uint8_t c; // 1 byte
	int32_t d; // 4 bytes
};

struct Struct3{
	int8_t a; // 1 byte
	int8_t str[10]; // 10 bytes
	int8_t b; // 1 byte
	int64_t c; // 8 byte
};

#endif // PACKAGESTRUCTS_HPP