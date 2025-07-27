#pragma once
#include "utils.h"

class Character {
public:
    int health, strength, luck;
    int level;
    Character(int health, int strength, int luck);
    virtual void display_stats();
    bool is_critical_hit() const;
    bool is_dodge() const;
};

class Player : public Character {
public:
    std::string nickname;
    int x, y, z;
    int coins;
    int flask_count;
    std::map<std::string, int> inventory;
    std::map<std::string, bool> weapons;
    std::map<std::string, int> weapon_upgrades;
    std::string current_weapon;
    bool in_conversation;
    bool interacted_with_holy_fire;
    bool has_pilgrimage_sword;
    bool defeated_dwarf;
    bool has_bought_sword;
    bool has_bought_stone;
    bool talked_to_storyteller;
    bool talked_to_storyteller2;
    bool has_shrine_key;
    bool rested_after_talking_to_storyteller2;
    int last_fire_x, last_fire_y, last_fire_z;

    Player(const std::string& nickname, int health, int strength, int luck, int coins);

    void add_to_inventory(const std::string& item, int quantity);
    void display_inventory() const;
    bool has_weapon() const;
    bool has_key() const;

    bool can_move_up(const std::vector<std::pair<int, int>>& blocked_cells) const;
    bool can_move_down(int max_y, const std::vector<std::pair<int, int>>& blocked_cells) const;
    bool can_move_left(const std::vector<std::pair<int, int>>& blocked_cells) const;
    bool can_move_right(int max_x, const std::vector<std::pair<int, int>>& blocked_cells) const;

    void move_down(int max_y, const std::vector<std::pair<int, int>>& blocked_cells);
    void move_up(const std::vector<std::pair<int, int>>& blocked_cells);
    void move_left(const std::vector<std::pair<int, int>>& blocked_cells);
    void move_right(int max_x, const std::vector<std::pair<int, int>>& blocked_cells);

    void display_available_moves(int max_x, int max_y, const std::vector<std::pair<int, int>>& blocked_cells) const;
    bool is_blocked(int nx, int ny, const std::vector<std::pair<int, int>>& blocked_cells) const;

    void reset();
    void choose_weapon();
    int get_normal_attack_damage() const;
    int get_charged_attack_damage() const;
    void level_up();
    bool upgrade_weapon(const std::string& weapon, int stone_level);
};
