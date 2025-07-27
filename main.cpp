#include "utils.h"
#include "character.h"
#include "npc.h"
#include "item.h"
#include "cell.h"
#include "holyfire.h"
#include "level.h"
#include "save_load.h"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    setlocale(LC_ALL, "Russian");
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    std::string nickname;
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    std::cout << "----------------------------\n";
    std::cout << "Добро пожаловать в Extenguished Destinies\n";
    std::cout << "----------------------------\n";
    std::cout << "Введите свой никнейм: ";
    std::cin >> nickname;

    Player player(nickname, 104, 1, 20, 100);

    // Загружаем сохранение
    load_game(player);

    // Заблокированные клетки для уровней
    std::vector<std::pair<int, int>> main_level_blocked = {
        {1, 4}, {2, 4}, {3, 4}, {3, 5}, {3, 6}, {3, 7},
        {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7},
        {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7},
        {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 7},
        {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7}
    };

    std::vector<std::pair<int, int>> mountain_pass_blocked = {}; 

    // Создаем уровни
    Level main_level(player, 8, 8, "Главный уровень", main_level_blocked);
    Level mountain_pass(player, 5, 5, "Перевал", mountain_pass_blocked);
    Level shrine(player, 3, 3, "Святилище", mountain_pass_blocked);

    // NPC
    main_level.grid[0][1].npcs.push_back(new Trader(0, 1));
    main_level.grid[2][2].npcs.push_back(new Storyteller(2, 2));
    mountain_pass.grid[1][2].npcs.push_back(new Blacksmith(2, 1));
    mountain_pass.grid[2][3].npcs.push_back(new Storyteller2(2, 3));

    // Items
    HolyFire holy_fire;
    bool dwarf_resurrected = false;
    main_level.grid[0][2].items.push_back(Item("Благодатный Огонь", true));
    mountain_pass.grid[0][4].items.push_back(Item("Титанический камень ковки(+2)"));

    if (!player.defeated_dwarf || dwarf_resurrected) {
        main_level.grid[0][7].npcs.push_back(new NPC("Дворф", 0, 7));
    }

    if (player.talked_to_storyteller2 && player.rested_after_talking_to_storyteller2) {
        mountain_pass.grid[2][3].npcs.clear();
        mountain_pass.grid[2][3].items.push_back(Item("Ключ от Святилища", true));
    }

    char command;
    bool in_mountain_pass = (player.z == 1);
    bool in_shrine = (player.z == 2);
    if (in_mountain_pass) {
        mountain_pass.enter();
    } else if (in_shrine) {
        shrine.enter();
    } else {
        main_level.enter();
    }

    while (true) {
        std::cout << "Здоровье: " << player.health << std::endl;
        std::cout << "Введите команду: ";
        std::cin >> command;

        std::cout << std::endl;
        std::cout << "----------------------------" << std::endl;
        std::cout << std::endl;

        if (command == 'q') {
            save_game(player);
            break;
        }
        else if (command == 'e' && (in_mountain_pass ? mountain_pass.grid[player.y][player.x].has_nearby_npc(player) : main_level.grid[player.y][player.x].has_nearby_npc(player))) {
            if (in_mountain_pass) {
                mountain_pass.interact();
            } else {
                main_level.interact();
            }
        }
        else if (command == 'w') {
            if (in_shrine) {
                player.move_down(2, mountain_pass_blocked);
                shrine.enter();
            } else if (player.x == 0 && player.y == 3 && !in_mountain_pass) {
                bool valid_choice = false;
                while (!valid_choice) {
                    std::cout << "1 - Зайти в Тупиковую Комнату" << std::endl;
                    std::cout << "2 - Остаться" << std::endl;
                    int choice;
                    std::cin >> choice;
                    switch (choice) {
                    case 1:
                        player.y = 5;
                        valid_choice = true;
                        break;
                    case 2:
                        valid_choice = true;
                        break;
                    default:
                        std::cout << "Неверный выбор." << std::endl;
                    }
                }
                main_level.enter();
            } else {
                if (in_mountain_pass) {
                    player.move_down(4, mountain_pass_blocked);
                    mountain_pass.enter();
                } else {
                    player.move_down(7, main_level_blocked);
                    main_level.enter();
                }
            }
        }
        else if (command == 's') {
            if (in_shrine) {
                player.move_up(mountain_pass_blocked);
                shrine.enter();
            } else if (player.x == 0 && player.y == 5 && !in_mountain_pass) {
                bool valid_choice = false;
                while (!valid_choice) {
                    std::cout << "1 - Выйти из Тупиковой Комнаты" << std::endl;
                    std::cout << "2 - Остаться" << std::endl;
                    int choice;
                    std::cin >> choice;
                    switch (choice) {
                    case 1:
                        player.y = 3;
                        valid_choice = true;
                        break;
                    case 2:
                        valid_choice = true;
                        break;
                    default:
                        std::cout << "Неверный выбор." << std::endl;
                    }
                }
                main_level.enter();
            } else {
                if (in_mountain_pass) {
                    player.move_up(mountain_pass_blocked);
                    mountain_pass.enter();
                } else {
                    player.move_up(main_level_blocked);
                    main_level.enter();
                }
            }
        }
        else if (command == 'a') {
            if (in_shrine) {
                player.move_left(mountain_pass_blocked);
                shrine.enter();
            } else if (in_mountain_pass) {
                player.move_left(mountain_pass_blocked);
                mountain_pass.enter();
            } else {
                player.move_left(main_level_blocked);
                main_level.enter();
            }
        }
        else if (command == 'd') {
            if (in_shrine) {
                player.move_right(2, mountain_pass_blocked);
                shrine.enter();
            } else if (in_mountain_pass) {
                player.move_right(4, mountain_pass_blocked);
                mountain_pass.enter();
            } else {
                player.move_right(7, main_level_blocked);
                main_level.enter();
            }
        }
        else if (command == 'i') {
            player.display_inventory();
        }
        else if (command == 'f') {
            if (player.x == 0 && player.y == 2) {
                holy_fire.interact(player, dwarf_resurrected, main_level.grid[0][7]);
                if (player.talked_to_storyteller2) {
                    player.rested_after_talking_to_storyteller2 = true;
                    mountain_pass.grid[2][3].npcs.clear();
                    mountain_pass.grid[2][3].items.push_back(Item("Ключ от Святилища", true));
                }
                in_mountain_pass = (player.z == 1);
                if (in_mountain_pass) {
                    mountain_pass.enter();
                } else {
                    main_level.enter();
                }
            } else {
                std::cout << "Здесь нет Благодатного Огня." << std::endl;
            }
        }
        else if (command == 'g' && player.x == 3 && player.y == 3 && !in_mountain_pass) {
            if (player.has_key()) {
                in_mountain_pass = true;
                player.x = 0;
                player.y = 0;
                player.z = 1;
                mountain_pass.enter();
            } else {
                std::cout << "У вас нет ключа от Перевала." << std::endl;
            }
        }
        else if (command == 'r' && in_mountain_pass && player.x == 0 && player.y == 0) {
            in_mountain_pass = false;
            player.x = 3;
            player.y = 3;
            player.z = 0;
            main_level.enter();
        }
        else if (command == 'r' && in_shrine && player.x == 0 && player.y == 0) {
            in_shrine = false;
            player.x = 4;
            player.y = 4;
            player.z = 1;
            mountain_pass.enter();
        }
        else if (command == 'r' && player.x == 4 && player.y == 4 && player.z == 1 && player.has_shrine_key) {
            in_shrine = true;
            player.x = 0;
            player.y = 0;
            player.z = 2;
            shrine.enter();
        }
        else if (command == 'h' && player.x == 0 && player.y == 7 && player.z == 0 && (!player.defeated_dwarf || dwarf_resurrected)) {
            main_level.battle_with_dwarf(dwarf_resurrected);
        }
        else {
            std::cout << "Неверная команда. Попробуйте еще раз." << std::endl;
        }

        std::cout << std::endl;
        std::cout << "----------------------------" << std::endl;
        std::cout << std::endl;
    }

    // Чистка памяти
    for (auto& row : main_level.grid) {
        for (auto& cell : row) {
            for (auto npc_ptr : cell.npcs) {
                delete npc_ptr;
            }
        }
    }

    for (auto& row : mountain_pass.grid) {
        for (auto& cell : row) {
            for (auto npc_ptr : cell.npcs) {
                delete npc_ptr;
            }
        }
    }

    for (auto& row : shrine.grid) {
        for (auto& cell : row) {
            for (auto npc_ptr : cell.npcs) {
                delete npc_ptr;
            }
        }
    }

    return 0;
}
