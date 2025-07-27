#include "npc.h"

NPC::NPC(const std::string& name, int x, int y) : name(name), x(x), y(y) {}

void NPC::interact(Player& player) {
    std::cout << "NPC " << name << " говорит с вами." << std::endl;
}

Trader::Trader(int x, int y) : NPC("Торговец", x, y) {}

void Trader::interact(Player& player) {
    if (player.has_bought_sword && player.has_bought_stone) {
        std::cout << "Все предметы распроданы. Заходите позже!" << std::endl;
    } else {
        std::cout << "Приветствую! У вас есть " << player.coins << " монет. Что вы хотите приобрести?" << std::endl;
        if (!player.has_bought_sword) {
            std::cout << "1. Обычный меч - 50 монет" << std::endl;
        }
        if (!player.has_bought_stone) {
            std::cout << "2. Титанический камень ковки(+1) - 100 монет" << std::endl;
        }

        char choice;
        std::cout << "Введите номер предмета для покупки (или 'q' для выхода): ";
        std::cin >> choice;

        switch (choice) {
        case '1':
            if (!player.has_bought_sword) {
                if (player.coins >= 50) {
                    player.add_to_inventory("Обычный меч", 1);
                    player.coins -= 50;
                    player.has_bought_sword = true;
                    std::cout << "Вы приобрели обычный меч за 50 монет." << std::endl;
                } else {
                    std::cout << "У вас недостаточно монет для покупки обычного меча." << std::endl;
                }
            } else {
                std::cout << "Неверный выбор." << std::endl;
            }
            break;
        case '2':
            if (!player.has_bought_stone) {
                if (player.coins >= 100) {
                    player.add_to_inventory("Титанический камень ковки(+1)", 1);
                    player.coins -= 100;
                    player.has_bought_stone = true;
                    std::cout << "Вы приобрели Титанический камень ковки(+1) за 100 монет." << std::endl;
                } else {
                    std::cout << "У вас недостаточно монет для покупки Титанического камня ковки(+1)." << std::endl;
                }
            } else {
                std::cout << "Неверный выбор." << std::endl;
            }
            break;
        case 'q':
            std::cout << "До свидания!" << std::endl;
            break;
        default:
            std::cout << "Неверный выбор." << std::endl;
        }
    }
    player.in_conversation = false;
}

Blacksmith::Blacksmith(int x, int y) : NPC("Кузнец", x, y) {}

void Blacksmith::interact(Player& player) {
    bool interacting = true;
    while (interacting) {
        std::cout << "1. Повысить уровень оружия" << std::endl;
        std::cout << "2. Что можно найти на просторах подземелья?" << std::endl;
        std::cout << "3. Уйти" << std::endl;

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            int plus1_count = 0;
            int plus2_count = 0;

            if (player.inventory.find("Титанический камень ковки(+1)") != player.inventory.end()) {
                plus1_count = player.inventory.at("Титанический камень ковки(+1)");
            }
            if (player.inventory.find("Титанический камень ковки(+2)") != player.inventory.end()) {
                plus2_count = player.inventory.at("Титанический камень ковки(+2)");
            }

            if (plus1_count == 0 && plus2_count == 0) {
                std::cout << "У вас нет Титанических камней ковки." << std::endl;
            } else {
                std::cout << "У вас есть:" << std::endl;
                if (plus1_count > 0) {
                    std::cout << plus1_count << " Титанических камней ковки(+1)" << std::endl;
                }
                if (plus2_count > 0) {
                    std::cout << plus2_count << " Титанических камней ковки(+2)" << std::endl;
                }

                std::cout << "Какое оружие вы хотите улучшить?" << std::endl;
                int index = 1;
                std::vector<std::string> upgradeable_weapons;
                for (const auto& weapon : player.weapons) {
                    int upgrade_level = player.weapon_upgrades.count(weapon.first) ? player.weapon_upgrades.at(weapon.first) : 0;
                    std::cout << index << ". " << weapon.first << " (Уровень: +" << upgrade_level << ")" << std::endl;
                    upgradeable_weapons.push_back(weapon.first);
                    index++;
                }

                int weapon_choice;
                std::cin >> weapon_choice;
                if (weapon_choice > 0 && weapon_choice <= static_cast<int>(upgradeable_weapons.size())) {
                    std::string chosen_weapon = upgradeable_weapons[weapon_choice - 1];

                    if (plus1_count > 0 && plus2_count > 0) {
                        std::cout << "Какой камень вы хотите использовать для улучшения?" << std::endl;
                        std::cout << "1. Титанический камень ковки(+1)" << std::endl;
                        std::cout << "2. Титанический камень ковки(+2)" << std::endl;
                        int stone_choice;
                        std::cin >> stone_choice;
                        int stone_level = (stone_choice == 1) ? 1 : 2;

                        player.upgrade_weapon(chosen_weapon, stone_level);
                    } else {
                        int stone_level = (plus1_count > 0) ? 1 : 2;
                        player.upgrade_weapon(chosen_weapon, stone_level);
                    }
                } else {
                    std::cout << "Неверный выбор." << std::endl;
                }
            }
            interacting = false;
            break;
        }
        case 2:
            std::cout << "На просторах подземелья разбросано множество ценных предметов, имей терпение и бдительность чтобы найти их" << std::endl;
            break;
        case 3:
            std::cout << "До свидания!" << std::endl;
            interacting = false;
            break;
        default:
            std::cout << "Неверный выбор. Попробуйте еще раз." << std::endl;
        }

        std::cout << std::endl;
        std::cout << "----------------------------" << std::endl;
        std::cout << std::endl;
    }
}

Storyteller::Storyteller(int x, int y) : NPC("Рассказчик", x, y) {}

void Storyteller::interact(Player& player) {
    bool continue_conversation = true;

    while (continue_conversation) {
        if (!player.talked_to_storyteller) {
            std::cout << "Очередную погасшую душу призвали на нескончаемое поле брани. "
                         "Зачем же они нужны вам? О великий, неужели вы еще не насытились кровью бессмертных?" << std::endl;
        }
        std::cout << "1. О чём ты говоришь?" << std::endl;
        std::cout << "2. Уйти промолчав." << std::endl;

        if (player.talked_to_storyteller) {
            std::cout << "3. Что насчет торговца?" << std::endl;
        }

        char choice;
        std::cout << "Выберите вариант ответа: ";
        std::cin >> choice;

        std::cout << std::endl;
        std::cout << "----------------------------" << std::endl;
        std::cout << std::endl;

        switch (choice) {
        case '1':
            std::cout << "Глупая погасшая душа, ты еще не знаешь куда попал, да поможет тебе наш Повелитель" << std::endl;
            player.talked_to_storyteller = true;
            break;
        case '2':
            std::cout << "Вы ушли промолчав." << std::endl;
            continue_conversation = false;
            break;
        case '3':
            if (player.talked_to_storyteller) {
                std::cout << "Местный пройдоха и богохульник, купить у него меч - пойти на верную смерть! О Великий, сохрани нас... О Великий..." << std::endl;
            } else {
                std::cout << "Неверный выбор." << std::endl;
            }
            break;
        default:
            std::cout << "Неверный выбор." << std::endl;
        }
    }
}

Storyteller2::Storyteller2(int x, int y) : NPC("Рассказчик2", x, y) {}

void Storyteller2::interact(Player& player) {
    std::cout << "О великий... Совсем скоро... Совсем скоро я воочию смогу лицезреть вас... О Великий, мне осталось лишь в последний раз использовать Благодатный огонь..." << std::endl;
    std::cout << "1. Уйти" << std::endl;
    char choice;
    std::cin >> choice;
    if (choice == '1') {
        std::cout << "Вы ушли." << std::endl;
        player.talked_to_storyteller2 = true;
    } else {
        std::cout << "Неверный выбор. Вы ушли." << std::endl;
        player.talked_to_storyteller2 = true;
    }
}
