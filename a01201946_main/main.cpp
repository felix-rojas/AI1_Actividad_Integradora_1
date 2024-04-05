#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<int> LPSVec(std::string pat, int M);

// Prints occurrences of pat[] in txt[]
int KMPSearch(std::string pat, std::string txt) {
  int M = pat.size();
  int N = txt.size();

  // Preprocess the pattern (calculate lps[] array)
  auto lps = LPSVec(pat, M);
  int i = 0;
  int j = 0;
  while ((N - i) >= (M - j)) {
    if (pat[j] == txt[i]) {
      j++;
      i++;
    }
    if (j == M) {
      return i - j;
      j = lps[j - 1];
    } else if (i < N && pat[j] != txt[i]) {
      if (j != 0)
        j = lps[j - 1];
      else
        i++;
    }
  }
  return -1;
}

// Fills lps[] for given pattern pat[0..M-1]
std::vector<int> LPSVec(std::string pat, int M) {
  std::vector<int> lps;
  lps.reserve(M);
  int len = 0;
  lps[0] = 0;
  int i = 1;
  while (i < M) {
    if (pat[i] == pat[len]) {
      len++;
      lps[i] = len;
      i++;
    } else {
      if (len != 0) {
        len = lps[len - 1];
      } else {
        lps[i++] = 0;
      }
    }
  }
  return lps;
}

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

  for (unsigned int i = 0; i < 3; i++) {
    std::string malicious_code = "";
    std::ifstream mcode(mcodes[i]);
    std::getline(mcode, malicious_code);
    std::cout << "Malicious code file: " << mcodes[i] << std::endl;
    std::cout << "Malicious code: " << malicious_code << std::endl;

    for (unsigned int i = 0; i < 2; i++) {
      std::string msg = "";
      std::ifstream transmission_file(transmissions[i]);
      std::cout << "Transmission file: " << transmissions[i] << std::endl;
      if (transmission_file.is_open()) {
        while (std::getline(transmission_file, msg)) {
          std::cout << "Transmission: " << msg << std::endl;
          int ans = KMPSearch(malicious_code, msg);
          if (ans >= 0)
            std::cout << "True at index: " << ans << std::endl;
          else
            std::cout << "False" << std::endl;
        }
      }
      std::cout << std::endl;
      transmission_file.close();
    }
    mcode.close();
  }

  /*  parte2 palndromo
       posiciónInicial posiciónFinal (para archivo de transmisión1)
       posiciónInicial posiciónFinal (para archivo de transmisión2)
    */
  /*parte 3 
      posiciónInicial posiciónFinal (de substring común más largo entre dos
     archivos de transmisión )*/
}