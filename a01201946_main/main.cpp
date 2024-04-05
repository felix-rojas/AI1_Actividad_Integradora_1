#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace fs = std::filesystem;

const std::string test_paths = "../pruebas";
const std::regex transmission_pattern(".*transmission\\d*\\.txt");
const std::regex mc_pattern(".*mcode\\d*\\.txt");

int main() {
  std::vector<fs::path> files;
  std::vector<std::string> mcodes;
  std::vector<std::string> transmissions;
  for (const auto &entry : fs::directory_iterator(test_paths))
    files.push_back(entry.path());

  std::sort(files.begin(), files.end());

  for (auto &file : files) {
    if (std::regex_match(file.generic_string(), transmission_pattern)) {
      transmissions.push_back(file.generic_string());
    }
    if (std::regex_match(file.generic_string(), mc_pattern)) {
      mcodes.push_back(file.generic_string());
    }
  }
  
}