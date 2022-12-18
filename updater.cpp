#include "updater.h"



Updater::Updater() {
    update("#include <iostream>");
}

void Updater::update(const char* word) {
    std::ofstream file;
    file.open(mainFile);
    file << word;
    file.close();
}
