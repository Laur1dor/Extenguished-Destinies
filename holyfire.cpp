#include "holyfire.h"

void HolyFire::interact(Player& player, bool& dwarf_resurrected, Cell& dwarf_cell) {
    bool interacting = true;
    while (interacting) {
        std::cout << "1. Отдохнуть, восстановить здоровье." << std::endl;
        std::cout << "2. Повысить уровень персонажа." << std::endl;
        std::cout << "3. Переместиться." << std::endl;
        std::cout << "4. Уйти." << std::endl;

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Вы восстанавливаете здоровье и фласки у Благодатного Огня." << std::endl;
            player.health = 104;
            player.flask_count = 2;
            player.interacted_with_holy_fire = true;
            player.last_fire_x = player.x;
            player.last_fire_y = player.y;
            player.last_fire_z = player.z;
            dwarf_resurrected = true;
            if (player.defeated_dwarf && dwarf_cell.npcs.empty()) {
                dwarf_cell.npcs.push_back(new NPC("Дворф", 0, 7));
            }
            std::cout << "Ваше здоровье и фласки восстановлены." << std::endl;
            player.display_stats();
            interacting = false;
            break;
        case 2:
            player.level_up();
            break;
        case 3:
            teleport(player);
            interacting = false;
            break;
        case 4:
            interacting = false;
            break;
        default:
            std::cout << "Неверный выбор. Попробуйте еще раз." << std::endl;
        }
    }
}

void HolyFire::teleport(Player& player) {
    std::cout << "Выберите пункт назначения:" << std::endl;
    if (!(player.x == 0 && player.y == 2 && player.z == 0)) {
        std::cout << "1. Главный уровень (0, 2, 0)" << std::endl;
    }
    if (!(player.x == 0 && player.y == 2 && player.z == 1)) {
        std::cout << "2. Перевал (0, 2, 1)" << std::endl;
    }
    std::cout << "3. Отмена" << std::endl;

    int choice;
    std::cin >> choice;

    switch (choice) {
    case 1:
        if (!(player.x == 0 && player.y == 2 && player.z == 0)) {
            player.x = 0;
            player.y = 2;
            player.z = 0;
        } else {
            std::cout << "Вы уже находитесь на этом Благодатном Огне." << std::endl;
        }
        break;
    case 2:
        if (!(player.x == 0 && player.y == 2 && player.z == 1)) {
            player.x = 0;
            player.y = 2;
            player.z = 1;
        } else {
            std::cout << "Вы уже находитесь на этом Благодатном Огне." << std::endl;
        }
        break;
    case 3:
        std::cout << "Перемещение отменено." << std::endl;
        break;
    default:
        std::cout << "Неверный выбор. Попробуйте еще раз." << std::endl;
    }
}
