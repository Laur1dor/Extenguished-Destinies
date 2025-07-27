#pragma once
#include "utils.h"

class Item {
public:
    std::string name;
    bool is_secret;
    Item(const std::string& name, bool is_secret = false);
};
