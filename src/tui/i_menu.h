#pragma once

#include "menu_result.h"

class IMenu{
    public:
    virtual MenuResult run()=0;
    virtual ~IMenu()=default;
};