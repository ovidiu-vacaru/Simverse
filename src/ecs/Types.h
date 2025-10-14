#pragma once

#include <cstdint>
#include <bitset>
#include <limits> // Required for std::numeric_limits

// --- Entity ---
// A unique ID for any "thing" in our world.
// We use a 32-bit unsigned integer, allowing for ~4.2 billion unique entities.
using Entity = std::uint32_t;


// --- Component ---
// A unique ID for a type of component (e.g., Position, Velocity).
// We use an 8-bit unsigned integer.
using ComponentID = std::uint8_t;

// The maximum number of component types is derived directly from the ComponentID type.
// This is a robust way to determine the size of our Signature bitset.
// A uint8_t can represent 256 distinct values (0-255).
constexpr std::size_t MAX_COMPONENTS_TYPE_LIMIT = std::numeric_limits<ComponentID>::max() + 1;


// --- Signature ---
// A "checklist" for an entity, represented as a bitset.
// Each bit corresponds to a ComponentID. A '1' means the entity has that component.
// The size of the bitset is configured to the maximum possible for the ComponentID type.
using Signature = std::bitset<MAX_COMPONENTS_TYPE_LIMIT>;

