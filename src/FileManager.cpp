#include "FileManager.hpp"

using namespace std;
using namespace std::filesystem;

FileManager::FileManager() {
    if(!exists(localDir)) {
        create_directory(localDir);
    }
}

FileManager::~FileManager() {}

const path& FileManager::getLocalDir() {
    return localDir;
}

path FileManager::localDir = string(getenv("HOME")) + "/.local/share/ProductionAnalysis/";
