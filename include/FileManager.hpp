#pragma once

#include <filesystem>

class FileManager final {
public:
    FileManager();

    ~FileManager();

    static const std::filesystem::path& getLocalDir();

private:
    static std::filesystem::path localDir;
};
