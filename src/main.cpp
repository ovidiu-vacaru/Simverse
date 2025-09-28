#include <iostream>
#include "world.hpp"
#include "types.hpp"


int main() {
    World world;

    Entity player = world.create_entity();
    Entity rock = world.create_entity();

    world.add_component(player, Position{2.5f, 3.5f});
    world.add_component(player, Health{});
    world.add_component(rock, Position{1.1f, 2.2f});


    auto& player_location = world.get_component<Position>(player);

    std::cout << player_location.x << player_location.y;



}