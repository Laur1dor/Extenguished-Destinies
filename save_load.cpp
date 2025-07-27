#include "save_load.h"

void save_game(const Player& player) {
    std::string filename = player.nickname + "_save.txt";
    std::ofstream save_file(filename);
    if (save_file.is_open()) {
        save_file << "health=" << player.health << "\n";
        save_file << "strength=" << player.strength << "\n";
        save_file << "luck=" << player.luck << "\n";
        save_file << "level=" << player.level << "\n";
        save_file << "x=" << player.x << "\n";
        save_file << "y=" << player.y << "\n";
        save_file << "z=" << player.z << "\n";
        save_file << "coins=" << player.coins << "\n";
        save_file << "flask_count=" << player.flask_count << "\n";
        save_file << "in_conversation=" << player.in_conversation << "\n";
        save_file << "interacted_with_holy_fire=" << player.interacted_with_holy_fire << "\n";
        save_file << "has_pilgrimage_sword=" << player.has_pilgrimage_sword << "\n";
        save_file << "defeated_dwarf=" << player.defeated_dwarf << "\n";
        save_file << "has_bought_sword=" << player.has_bought_sword << "\n";
        save_file << "has_bought_stone=" << player.has_bought_stone << "\n";
        save_file << "talked_to_storyteller=" << player.talked_to_storyteller << "\n";
        save_file << "talked_to_storyteller2=" << player.talked_to_storyteller2 << "\n";
        save_file << "has_shrine_key=" << player.has_shrine_key << "\n";
        save_file << "rested_after_talking_to_storyteller2=" << player.rested_after_talking_to_storyteller2 << "\n";
        save_file << "last_fire_x=" << player.last_fire_x << "\n";
        save_file << "last_fire_y=" << player.last_fire_y << "\n";
        save_file << "last_fire_z=" << player.last_fire_z << "\n";

        for (const auto& item : player.inventory) {
            save_file << "inventory_" << item.first << "=" << item.second << "\n";
        }

        save_file << "current_weapon=" << player.current_weapon << "\n";

        for (const auto& weapon : player.weapons) {
            save_file << "weapon_" << weapon.first << "=" << (weapon.second ? 1 : 0) << "\n";
        }

        for (const auto& upgrade : player.weapon_upgrades) {
            save_file << "upgrade_" << upgrade.first << "=" << upgrade.second << "\n";
        }

        save_file.close();
    } else {
        std::cerr << "Ошибка: Не удалось открыть файл для сохранения.\n";
    }
}

void load_game(Player& player) {
    std::string filename = player.nickname + "_save.txt";
    std::ifstream load_file(filename);
    if (load_file.is_open()) {
        std::string line;
        while (std::getline(load_file, line)) {
            std::istringstream line_stream(line);
            std::string key;
            if (std::getline(line_stream, key, '=')) {
                std::string value;
                if (std::getline(line_stream, value)) {
                    if (key == "health") player.health = std::stoi(value);
                    else if (key == "strength") player.strength = std::stoi(value);
                    else if (key == "luck") player.luck = std::stoi(value);
                    else if (key == "level") player.level = std::stoi(value);
                    else if (key == "x") player.x = std::stoi(value);
                    else if (key == "y") player.y = std::stoi(value);
                    else if (key == "z") player.z = std::stoi(value);
                    else if (key == "coins") player.coins = std::stoi(value);
                    else if (key == "flask_count") player.flask_count = std::stoi(value);
                    else if (key == "in_conversation") player.in_conversation = (value == "1");
                    else if (key == "interacted_with_holy_fire") player.interacted_with_holy_fire = (value == "1");
                    else if (key == "has_pilgrimage_sword") player.has_pilgrimage_sword = (value == "1");
                    else if (key == "defeated_dwarf") player.defeated_dwarf = (value == "1");
                    else if (key == "has_bought_sword") player.has_bought_sword = (value == "1");
                    else if (key == "has_bought_stone") player.has_bought_stone = (value == "1");
                    else if (key == "talked_to_storyteller") player.talked_to_storyteller = (value == "1");
                    else if (key == "talked_to_storyteller2") player.talked_to_storyteller2 = (value == "1");
                    else if (key == "has_shrine_key") player.has_shrine_key = (value == "1");
                    else if (key == "rested_after_talking_to_storyteller2") player.rested_after_talking_to_storyteller2 = (value == "1");
                    else if (key == "last_fire_x") player.last_fire_x = std::stoi(value);
                    else if (key == "last_fire_y") player.last_fire_y = std::stoi(value);
                    else if (key == "last_fire_z") player.last_fire_z = std::stoi(value);
                    else if (key.rfind("inventory_", 0) == 0) {
                        std::string item_name = key.substr(10);
                        player.inventory[item_name] = std::stoi(value);
                    }
                    else if (key == "current_weapon") {
                        player.current_weapon = value;
                    }
                    else if (key.rfind("weapon_", 0) == 0) {
                        std::string weapon_name = key.substr(7);
                        player.weapons[weapon_name] = (value == "1");
                    }
                    else if (key.rfind("upgrade_", 0) == 0) {
                        std::string weapon_name = key.substr(8);
                        player.weapon_upgrades[weapon_name] = std::stoi(value);
                    }
                }
            }
        }
        load_file.close();

        // Восстанавливаем позицию
        player.x = player.last_fire_x;
        player.y = player.last_fire_y;
        player.z = player.last_fire_z;
    } else {
        std::cerr << "Ошибка: Не удалось открыть файл для загрузки. Начата новая игра.\n";
    }
}
