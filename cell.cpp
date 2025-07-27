#include "cell.h"

void Cell::describe() {
    for (const auto& item : items) {
        if (!item.is_secret) {
            std::cout << "Вы видите " << item.name << std::endl;
        }
    }
    for (const auto& npc : npcs) {
        std::cout << "Здесь есть NPC по имени " << npc->name << std::endl;
    }
}

void Cell::interact(Player& player) {
    for (auto it = items.begin(); it != items.end();) {
        if (!it->is_secret) {
            player.add_to_inventory(it->name, 1);
            it = items.erase(it);
        } else {
            ++it;
        }
    }
    for (auto& npc : npcs) {
        npc->interact(player);
    }
}

bool Cell::has_nearby_npc(const Player& player) const {
    for (const auto& npc : npcs) {
        if (std::abs(player.x - npc->x) <= 1 && std::abs(player.y - npc->y) <= 1) {
            return true;
        }
    }
    return false;
}
