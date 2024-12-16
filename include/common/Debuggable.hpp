#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <bits/chrono.h>

class Debuggable
{
private:
    std::string className;

protected:
    void console_log(const std::string &msg) const;
    Debuggable(std::string name);
    ~Debuggable();
};