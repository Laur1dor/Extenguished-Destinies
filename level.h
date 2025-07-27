#pragma once
#include "utils.h"
#include "cell.h"
#include "character.h"
#include "holyfire.h"

class Level {
public:
    std::vector<std::vector<Cell>> grid;
    Player& player;
    bool welcome_message_displayed;
    std::string name;
    std::vector<std::pair<int, int>> blocked_cells;

    Level(Player& player, int width, int height, const std::string& name, const std::vector<std::pair<int, int>>& blocked_cells);
    void enter();
    void interact();
    void battle_with_dwarf(bool& dwarf_resurrected);
    void display_player_position() const;
    bool has_item(const std::string& item_name) const;
    void remove_item(const std::string& item_name);
};
