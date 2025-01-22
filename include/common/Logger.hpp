#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <bits/chrono.h>

class Logger
{
private:
    std::string className;
public:
    Logger(std::string name);
    ~Logger();

    void console_log(const std::string &msg) const;
};