#pragma once
#include "utils.h"
#include "character.h"

class NPC {
public:
    virtual ~NPC() {}
    std::string name;
    int x, y;
    NPC(const std::string& name, int x, int y);
    virtual void interact(Player& player);
};

class Trader : public NPC {
public:
    Trader(int x, int y);
    void interact(Player& player) override;
};

class Blacksmith : public NPC {
public:
    Blacksmith(int x, int y);
    void interact(Player& player) override;
};

class Storyteller : public NPC {
public:
    Storyteller(int x, int y);
    void interact(Player& player) override;
};

class Storyteller2 : public NPC {
public:
    Storyteller2(int x, int y);
    void interact(Player& player) override;
};
