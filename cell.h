#pragma once
#include "utils.h"
#include "item.h"
#include "npc.h"
#include "character.h"

class Cell {
public:
    std::vector<Item> items;
    std::vector<NPC*> npcs;

    void describe();
    void interact(Player& player);
    bool has_nearby_npc(const Player& player) const;
};
