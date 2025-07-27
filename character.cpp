#include "character.h"

Character::Character(int health, int strength, int luck)
    : health(health), strength(strength), luck(luck), level(1) {}

void Character::display_stats() {
    std::cout << "Здоровье: " << health << ", Сила: " << strength << ", Удача: " << luck << ", Уровень: " << level << std::endl;
}

bool Character::is_critical_hit() const {
    return (std::rand() % 100) < luck;
}

bool Character::is_dodge() const {
    return (std::rand() % 100) < luck;
}

Player::Player(const std::string& nickname, int health, int strength, int luck, int coins)
    : Character(health, strength, luck), nickname(nickname), x(0), y(0), z(0), coins(coins), flask_count(2),
      in_conversation(false), interacted_with_holy_fire(false), has_pilgrimage_sword(false), defeated_dwarf(false),
      has_bought_sword(false), has_bought_stone(false), talked_to_storyteller(false), talked_to_storyteller2(false),
      has_shrine_key(false), rested_after_talking_to_storyteller2(false), last_fire_x(0), last_fire_y(2), last_fire_z(0) {}

void Player::add_to_inventory(const std::string& item, int quantity) {
    inventory[item] += quantity;
    if (item == "Обычный меч" || item == "Меч Паломничества") {
        weapons[item] = true;
    }
    std::cout << item << " добавлено в ваш инвентарь." << std::endl;
}

void Player::display_inventory() const {
    std::cout << "Инвентарь:" << std::endl;
    for (const auto& item : inventory) {
        std::cout << item.first << ": " << item.second << std::endl;
    }
    std::cout << "Фласки: " << flask_count << std::endl;
    std::cout << "Монеты: " << coins << std::endl;
    std::cout << "Оружие:" << std::endl;
    for (const auto& weapon : weapons) {
        std::string weapon_name = weapon.first;
        if (weapon_upgrades.count(weapon_name)) {
            weapon_name += " (+" + std::to_string(weapon_upgrades.at(weapon_name)) + ")";
        }
        std::cout << weapon_name << std::endl;
    }
}

bool Player::has_weapon() const {
    return !weapons.empty();
}

bool Player::has_key() const {
    return inventory.find("Ключ от Перевала") != inventory.end();
}

bool Player::can_move_up(const std::vector<std::pair<int, int>>& blocked_cells) const {
    return y > 0 && !is_blocked(x, y - 1, blocked_cells);
}

bool Player::can_move_down(int max_y, const std::vector<std::pair<int, int>>& blocked_cells) const {
    return y < max_y && !is_blocked(x, y + 1, blocked_cells);
}

bool Player::can_move_left(const std::vector<std::pair<int, int>>& blocked_cells) const {
    return x > 0 && !is_blocked(x - 1, y, blocked_cells);
}

bool Player::can_move_right(int max_x, const std::vector<std::pair<int, int>>& blocked_cells) const {
    return x < max_x && !is_blocked(x + 1, y, blocked_cells);
}

void Player::move_down(int max_y, const std::vector<std::pair<int, int>>& blocked_cells) {
    if (can_move_down(max_y, blocked_cells)) {
        y++;
    }
}

void Player::move_up(const std::vector<std::pair<int, int>>& blocked_cells) {
    if (can_move_up(blocked_cells)) {
        y--;
    }
}

void Player::move_left(const std::vector<std::pair<int, int>>& blocked_cells) {
    if (can_move_left(blocked_cells)) {
        x--;
    }
}

void Player::move_right(int max_x, const std::vector<std::pair<int, int>>& blocked_cells) {
    if (can_move_right(max_x, blocked_cells)) {
        x++;
    }
}

void Player::display_available_moves(int max_x, int max_y, const std::vector<std::pair<int, int>>& blocked_cells) const {
    std::cout << "Доступные команды:" << std::endl;
    if (can_move_down(max_y, blocked_cells)) {
        std::cout << "s - Вниз" << std::endl;
    }
    if (can_move_up(blocked_cells)) {
        std::cout << "w - Вверх" << std::endl;
    }
    if (can_move_left(blocked_cells)) {
        std::cout << "a - Влево" << std::endl;
    }
    if (can_move_right(max_x, blocked_cells)) {
        std::cout << "d - Вправо" << std::endl;
    }
    if (!in_conversation) {
        std::cout << "i - Инвентарь" << std::endl;
    }
}

bool Player::is_blocked(int nx, int ny, const std::vector<std::pair<int, int>>& blocked_cells) const {
    for (const auto& block : blocked_cells) {
        if (block.first == nx && block.second == ny) {
            return true;
        }
    }
    return false;
}

void Player::reset() {
    health = 104;
    x = 0;
    y = 0;
    z = 0;
    coins = 100;
    flask_count = 2;
    inventory.clear();
    weapons.clear();
    weapon_upgrades.clear();
    current_weapon = "";
    in_conversation = false;
    interacted_with_holy_fire = false;
    has_pilgrimage_sword = false;
    defeated_dwarf = false;
    has_bought_sword = false;
    has_bought_stone = false;
    talked_to_storyteller = false;
    talked_to_storyteller2 = false;
    has_shrine_key = false;
    rested_after_talking_to_storyteller2 = false;
    last_fire_x = 0;
    last_fire_y = 2;
    last_fire_z = 0;
}

void Player::choose_weapon() {
    if (weapons.size() == 1) {
        current_weapon = weapons.begin()->first;
    } else {
        std::cout << "Выберите оружие для боя:" << std::endl;
        int index = 1;
        for (const auto& weapon : weapons) {
            int normal_attack = 0, charged_attack = 0;
            int bonus_damage = strength + 2;

            if (weapon.first == "Обычный меч") {
                normal_attack = 4;
                charged_attack = 16;
            } else if (weapon.first == "Меч Паломничества") {
                normal_attack = 13;
                charged_attack = 26;
            }

            int upgrade_level = weapon_upgrades.count(weapon.first) ? weapon_upgrades.at(weapon.first) : 0;
            normal_attack = static_cast<int>(std::ceil(normal_attack * (1 + upgrade_level * 0.25)));
            std::cout << index << ". " << weapon.first << " (" << normal_attack << "/" << charged_attack << ") (+" << bonus_damage << ")" << std::endl;
            index++;
        }

        int choice;
        std::cin >> choice;
        if (choice > 0 && choice <= static_cast<int>(weapons.size())) {
            current_weapon = std::next(weapons.begin(), choice - 1)->first;
        } else {
            std::cout << "Неверный выбор. Выбрано первое доступное оружие." << std::endl;
            current_weapon = weapons.begin()->first;
        }
    }
}

int Player::get_normal_attack_damage() const {
    int base_damage = 0;
    if (current_weapon == "Обычный меч") {
        base_damage = 4;
    } else if (current_weapon == "Меч Паломничества") {
        base_damage = 13;
    }
    int upgrade_level = weapon_upgrades.count(current_weapon) ? weapon_upgrades.at(current_weapon) : 0;
    base_damage = static_cast<int>(std::ceil(base_damage * (1 + upgrade_level * 0.25)));
    base_damage += (strength + 2);
    if (is_critical_hit()) {
        std::cout << "Критический удар!" << std::endl;
        return base_damage + static_cast<int>(std::floor(base_damage * 0.2));
    }
    return base_damage;
}

int Player::get_charged_attack_damage() const {
    int base_damage = 0;
    if (current_weapon == "Обычный меч") {
        base_damage = 16;
    } else if (current_weapon == "Меч Паломничества") {
        base_damage = 26;
    }
    int upgrade_level = weapon_upgrades.count(current_weapon) ? weapon_upgrades.at(current_weapon) : 0;
    base_damage = static_cast<int>(std::ceil(base_damage * (1 + upgrade_level * 0.25)));
    base_damage += (strength + 2);
    if (is_critical_hit()) {
        std::cout << "Критический удар!" << std::endl;
        return base_damage + static_cast<int>(std::floor(base_damage * 0.2));
    }
    return base_damage;
}

void Player::level_up() {
    int required_coins = static_cast<int>(std::round(100 * std::pow(1.3, level - 1)));
    std::cout << "Ваш текущий уровень: " << level << std::endl;
    std::cout << "Для повышения уровня нужно монет: " << required_coins << std::endl;

    if (coins >= required_coins) {
        std::cout << "Какую характеристику вы хотите повысить?" << std::endl;
        std::cout << "1. Здоровье" << std::endl;
        std::cout << "2. Физическая мощь" << std::endl;
        std::cout << "3. Удача" << std::endl;
        std::cout << "4. Выйти" << std::endl;

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            health += 10;
            break;
        case 2:
            strength += 1;
            break;
        case 3:
            luck += 2;
            break;
        case 4:
            std::cout << "Выход из меню." << std::endl;
            return;
        default:
            std::cout << "Неверный выбор." << std::endl;
            return;
        }

        coins -= required_coins;
        level++;
        std::cout << "Уровень повышен!" << std::endl;
        display_stats();
    } else {
        std::cout << "Недостаточно монет." << std::endl;
    }
}

bool Player::upgrade_weapon(const std::string& weapon, int stone_level) {
    int upgrade_level = weapon_upgrades.count(weapon) ? weapon_upgrades.at(weapon) : 0;

    if (stone_level <= upgrade_level) {
        std::cout << "Вы не можете использовать этот камень." << std::endl;
        return false;
    }

    if (stone_level > upgrade_level + 1) {
        std::cout << "Оружие должно быть сначала улучшено до +" << upgrade_level + 1 << std::endl;
        return false;
    }

    int cost = static_cast<int>(std::round(70 * std::pow(1.3, upgrade_level)));
    if (coins < cost) {
        std::cout << "Недостаточно монет." << std::endl;
        return false;
    }

    std::string stone_name = "Титанический камень ковки(+" + std::to_string(stone_level) + ")";
    if (inventory[stone_name] < 1) {
        std::cout << "У вас нет " << stone_name << std::endl;
        return false;
    }

    coins -= cost;
    inventory[stone_name]--;
    weapon_upgrades[weapon]++;
    std::cout << "Оружие " << weapon << " улучшено до +" << weapon_upgrades[weapon] << std::endl;

    return true;
}
