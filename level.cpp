#include "level.h"

Level::Level(Player& player, int width, int height, const std::string& name, const std::vector<std::pair<int, int>>& blocked_cells)
    : player(player), grid(height, std::vector<Cell>(width)), welcome_message_displayed(false), name(name), blocked_cells(blocked_cells) {}

void Level::enter() {
    if (!welcome_message_displayed) {
        std::cout << "Вы вошли в " << name << "!" << std::endl;
        std::cout << "У вас есть " << player.coins << " монет." << std::endl;
        welcome_message_displayed = true;
    }
    grid[player.y][player.x].describe();
    player.display_available_moves(grid[0].size() - 1, grid.size() - 1, blocked_cells);
    if (grid[player.y][player.x].has_nearby_npc(player)) {
        std::cout << "e - Заговорить" << std::endl;
    }
    if (player.x == 0 && player.y == 2) {
        std::cout << "f - Воспользоваться Благодатным Огнем" << std::endl;
    }
    if (name == "Главный уровень" && player.x == 3 && player.y == 3) {
        if (player.has_key()) {
            std::cout << "g - Зайти на Перевал" << std::endl;
        } else {
            std::cout << "У вас нет ключа от Перевала." << std::endl;
        }
    }
    else if (name == "Перевал" && player.x == 0 && player.y == 0) {
        std::cout << "r - Вернуться на Главный уровень" << std::endl;
    }
    else if (name == "Святилище" && player.x == 0 && player.y == 0) {
        std::cout << "r - Вернуться на Перевал" << std::endl;
    }
    if (name == "Главный уровень" && player.x == 2 && player.y == 5 && player.z == 0 && !player.has_pilgrimage_sword) {
        std::cout << "e - Подобрать меч Паломничества" << std::endl;
    }
    if (name == "Перевал" && player.x == 4 && player.y == 0 && player.z == 1 && has_item("Титанический камень ковки(+2)")) {
        std::cout << "e - Подобрать Титанический камень ковки(+2)" << std::endl;
    }
    if (name == "Главный уровень" && player.x == 0 && player.y == 7 && player.z == 0 && (!player.defeated_dwarf || !grid[0][7].npcs.empty())) {
        std::cout << "h - Вступить в бой с Дворфом" << std::endl;
    }
    if (name == "Перевал" && player.x == 3 && player.y == 2 && player.z == 1 && has_item("Ключ от Святилища") && !player.has_shrine_key) {
        std::cout << "t - Подобрать ключ от Святилища" << std::endl;
    }
    if (name == "Перевал" && player.x == 3 && player.y == 2 && player.z == 1 && player.talked_to_storyteller2 && !has_item("Ключ от Святилища")) {
        std::cout << "e - Заговорить" << std::endl;
    }
    if (name == "Перевал" && player.x == 4 && player.y == 4 && player.z == 1) {
        if (player.has_shrine_key) {
            std::cout << "r - Перейти в Святилище" << std::endl;
        } else {
            std::cout << "У вас нет ключа от Святилища." << std::endl;
        }
    }
    display_player_position();
}

void Level::interact() {
    if (name == "Главный уровень" && player.x == 2 && player.y == 5 && player.z == 0 && !player.has_pilgrimage_sword) {
        player.add_to_inventory("Меч Паломничества", 1);
        player.has_pilgrimage_sword = true;
    }
    else if (name == "Перевал" && player.x == 4 && player.y == 0 && player.z == 1 && has_item("Титанический камень ковки(+2)")) {
        player.add_to_inventory("Титанический камень ковки(+2)", 1);
        remove_item("Титанический камень ковки(+2)");
    }
    else if (name == "Перевал" && player.x == 3 && player.y == 2 && player.z == 1 && has_item("Ключ от Святилища")) {
        player.add_to_inventory("Ключ от Святилища", 1);
        player.has_shrine_key = true;
        remove_item("Ключ от Святилища");
    }
    else {
        grid[player.y][player.x].interact(player);
    }
}

void Level::battle_with_dwarf(bool& dwarf_resurrected) {
    if (!player.has_weapon()) {
        std::cout << "У вас нет оружия! Вы не можете вступить в бой с Дворфом." << std::endl;
        return;
    }

    player.choose_weapon();
    std::cout << "Вы вступили в бой с Дворфом!" << std::endl;

    int dwarf_health = 120;
    Character dwarf(dwarf_health, 15, 15);

    while (dwarf.health > 0 && player.health > 0) {
        std::cout << "Дворф: " << dwarf.health << " HP" << std::endl;
        std::cout << "Игрок: " << player.health << " HP" << std::endl;
        std::cout << "Выберите действие:" << std::endl;
        std::cout << "1. Обычная атака" << std::endl;
        std::cout << "2. Заряженная атака" << std::endl;
        std::cout << "3. Использовать фласку" << std::endl;

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Вы используете обычную атаку." << std::endl;
            if (dwarf.is_dodge()) {
                std::cout << "Дворф уклонился!" << std::endl;
            } else {
                dwarf.health -= player.get_normal_attack_damage();
            }
            break;
        case 2:
            std::cout << "Вы используете заряженную атаку." << std::endl;
            if (dwarf.is_dodge()) {
                std::cout << "Дворф уклонился!" << std::endl;
            } else {
                dwarf.health -= player.get_charged_attack_damage();
            }
            break;
        case 3:
            if (player.flask_count > 0) {
                std::cout << "Вы используете фласку." << std::endl;
                player.health = 104;
                player.flask_count--;
            } else {
                std::cout << "У вас нет фласок!" << std::endl;
            }
            continue;
        default:
            std::cout << "Неверный выбор." << std::endl;
            continue;
        }

        if (dwarf.health > 0) {
            if (player.is_dodge()) {
                std::cout << "Вы уклонились от атаки Дворфа!" << std::endl;
            } else {
                int dwarf_attack_damage = 15;
                if (dwarf.is_critical_hit()) {
                    dwarf_attack_damage += static_cast<int>(std::floor(dwarf_attack_damage * 0.2));
                    std::cout << "Дворф нанес критический удар!" << std::endl;
                }
                std::cout << "Дворф атакует вас." << std::endl;
                player.health -= dwarf_attack_damage;
            }
        }
    }

    if (player.health > 0) {
        std::cout << "Вы победили Дворфа и получили 100 монет!" << std::endl;
        if (!player.defeated_dwarf) {
            player.add_to_inventory("Ключ от Перевала", 1);
            player.defeated_dwarf = true;
        }
        player.coins += 100;
        grid[0][7].npcs.clear();
    } else {
        std::cout << "Вы были побеждены Дворфом." << std::endl;
        if (player.interacted_with_holy_fire) {
            std::cout << "Вы возрождаетесь у Благодатного Огня." << std::endl;
            player.x = player.last_fire_x;
            player.y = player.last_fire_y;
            player.z = player.last_fire_z;
            player.health = 104;
        } else {
            std::cout << "Вы не взаимодействовали с Благодатным Огнем. Игра начинается сначала." << std::endl;
            player.reset();
            enter();
        }
    }

    dwarf_resurrected = false;
}

void Level::display_player_position() const {
    std::cout << " x  y  z" << std::endl;
    std::cout << "(" << player.x << ", " << player.y << ", " << player.z << ")" << std::endl;
}

bool Level::has_item(const std::string& item_name) const {
    for (const auto& item : grid[player.y][player.x].items) {
        if (item.name == item_name) {
            return true;
        }
    }
    return false;
}

void Level::remove_item(const std::string& item_name) {
    auto& items = grid[player.y][player.x].items;
    items.erase(std::remove_if(items.begin(), items.end(), [&item_name](const Item& item) {
        return item.name == item_name;
    }), items.end());
}
