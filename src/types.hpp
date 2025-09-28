#ifndef TYPES_H
#define TYPES_H


#include <cstdint>

// An entity is just a unique ID.

using Entity = std::uint32_t;

struct Position {float x = 0.0f; float y = 0.0f;};
struct Health {int current = 100; int max = 100;};


#endif