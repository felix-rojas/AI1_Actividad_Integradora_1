#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main() {
  const std::string path = "../pruebas";
  for (const auto &entry : fs::directory_iterator(path))
    std::cout << entry.path() << std::endl;
}