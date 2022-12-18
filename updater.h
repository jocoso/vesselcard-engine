#ifndef UPDATER_H
#define UPDATER_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>

class Updater
{
public:
    Updater();
    void update(const char* word);
private:
    std::string fileName = "src/";
    std::string mainFile = fileName + "entry.cpp";
};

#endif // UPDATER_H
