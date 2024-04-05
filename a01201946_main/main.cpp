#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main() {
  const std::string test_paths = "../pruebas";
  std::vector<std::filesystem::path> files;
  for (const auto &entry : fs::directory_iterator(test_paths))
    files.push_back(entry.path());

  std::sort(files.begin(), files.end());
  for (auto &file : files)
    std::cout << file << std::endl;
}