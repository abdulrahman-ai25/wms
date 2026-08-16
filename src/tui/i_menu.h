#pragma once

#include "menu_result.h"
#include <string>
#include <iostream>

class IMenu{
    public:
    virtual MenuResult run() = 0;
    virtual std::string get_title() const = 0;
    virtual ~IMenu() = default;

    static inline std::string read_str(std::string& var, bool nullable = false)
    {
        while (true)
        {
            if (std::cin.peek() == '\n')
                std::cin.get();

            std::getline(std::cin, var);

            if (std::cin.fail())
            {
                std::cin.clear();
                var = "";
            }

            if (!var.empty() || nullable)
                return var;

            std::cout << "Input cannot be empty. Please try again: "<<std::endl;
        }
    }

};
