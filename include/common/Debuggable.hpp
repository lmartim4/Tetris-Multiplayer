#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <bits/chrono.h>

class Debuggable
{
private:
    std::string className;
public:
    Debuggable(std::string name);
    ~Debuggable();

    void console_log(const std::string &msg) const;
};