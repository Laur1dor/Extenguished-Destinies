#pragma once
#include "utils.h"
#include "character.h"
#include "cell.h"

class HolyFire {
public:
    void interact(Player& player, bool& dwarf_resurrected, Cell& dwarf_cell);
    void teleport(Player& player);
};
