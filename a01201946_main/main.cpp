#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using std::vector, std::string, std::endl, std::cout;
namespace fs = std::filesystem;

// O(N) both in space and time
void manacher_mirror(const string &str) {
  // pre-processing to avoid odd/even str problems
  string processed_string = "#";
  for (char c : str) {
    processed_string += c;
    processed_string += "#";
  }
  vector<int> palyndrome_len(processed_string.length(), 0);
  int center = 0, right = 0;
  int start = 0, end = 0;

  for (int i = 1; i < processed_string.length() - 1; i++) {
    int mirror = 2 * center - i;
    if (i < right)
      palyndrome_len[i] = std::min(right - i, palyndrome_len[mirror]);

    while (processed_string[i + 1 + palyndrome_len[i]] ==
           processed_string[i - 1 - palyndrome_len[i]])
      palyndrome_len[i]++;

    if (i + palyndrome_len[i] > right) {
      center = i;
      right = i + palyndrome_len[i];
    }

    if (palyndrome_len[i] > end - start) {
      start = (i - palyndrome_len[i]) / 2;
      end = start + palyndrome_len[i];
    }
  }

  cout << "Longest Palindromic Substring: " << str.substr(start, end - start)
       << endl;
  cout << "Initial Position: " << start << endl;
  cout << "Final Position: " << end - 1 << endl;
}

vector<int> LPSVec(const string &pat, int M);

// Prints occurrences of pat[] in txt[]
int KMPSearch(const string &pat, const string &txt) {
  int M = pat.size();
  int N = txt.size();

  // Preprocess the pattern (calculate lps[] array)
  vector<int> lps = LPSVec(pat, M);
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
vector<int> LPSVec(const string &pat, int M) {
  vector<int> lps;
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

// suffix automaton
//  $O(n)$  memory and  $O(n log k)$  time for processing the entire string.
// https://cp-algorithms.com/string/suffix-automaton.html

struct state {
  int len, link;
  // this could be unordered map IF we have a bigger dataset
  std::map<char, int> next;
};

// Preallocate state array
const int MAXLEN = 100000;
state st[MAXLEN * 2];
int sz, last;

void sa_init() {
  st[0].len = 0;
  st[0].link = -1;
  sz++;
  last = 0;
}

void sa_extend(char c) {
  int cur = sz++;
  st[cur].len = st[last].len + 1;
  int p = last;
  // linear traversal
  while (p != -1 && !st[p].next.count(c)) {
    st[p].next[c] = cur;
    p = st[p].link;
  }
  if (p == -1) {
    st[cur].link = 0;
  } else {
    int q = st[p].next[c];
    // continuous transition
    if (st[p].len + 1 == st[q].len) {
      st[cur].link = q;
    } else {
      // split states
      int clone = sz++;
      st[clone].len = st[p].len + 1;
      st[clone].next = st[q].next;
      st[clone].link = st[q].link;
      // copy transitions
      while (p != -1 && st[p].next[c] == q) {
        st[p].next[c] = clone;
        p = st[p].link;
      }
      st[q].link = st[cur].link = clone;
    }
  }
  last = cur;
}

void print_tree() {
  std::cout << std::left << std::setw(6) << "State" << std::setw(6) << "Len" << std::setw(6) << "Link" << "Transitions\n";
  for (int i = 0; i < sz; ++i) {
    std::cout << std::left << std::setw(6) << i << std::setw(6) << st[i].len << std::setw(6) << st[i].link;
    for (const auto& [ch, next_state] : st[i].next) {
      std::cout << ch << "->" << next_state << " ";
    }
    std::cout << "\n";
  }
}

// O(N)
string lcs(const string &S, const string &T) {
  sa_init();
  for (int i = 0; i < S.size(); i++)
    sa_extend(S[i]);

  print_tree();
  // the current state  "v",
  // and the current length " l"
  // describe the current matching part
  int v = 0, l = 0;

  // store longest match length and position
  int longest_match = 0, longest_match_pos = 0;

  for (int i = 0; i < T.size(); i++) {
    // If there is no transition using the required character,
    // go through the suffix link and decrease  l 
    // until we find a transition
    while (v && !st[v].next.count(T[i])) {
      v = st[v].link;
      l = st[v].len;
    }
    // If there is a transition from v  char  T[i] 
    if (st[v].next.count(T[i])) {
      // follow the transition
      v = st[v].next[T[i]];
      // increase  l  by one.
      l++;
    }
    // store longest match and position
    if (l > longest_match) {
      longest_match = l;
      longest_match_pos = i;
    }
  }
  cout << longest_match_pos - longest_match + 1 << ", " << longest_match_pos 
       << endl;
  return T.substr(longest_match_pos - longest_match + 1, longest_match);
}

const string test_paths = "../pruebas";
const std::regex transmission_pattern(".*transmission\\d*\\.txt");
const std::regex mc_pattern(".*mcode\\d*\\.txt");

int main() {
  vector<fs::path> files;
  vector<string> mcodes;
  vector<string> transmissions;
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

  cout << "------------------------------------------------------------------"
       << endl;
  cout << "--------------------- PARTE 1: String Search ---------------------"
       << endl;
  cout << "------------------------------------------------------------------"
       << endl;
  cout << endl;
  for (unsigned int i = 0; i < 3; i++) {
    string malicious_code = "";
    std::ifstream mcode(mcodes[i]);
    std::getline(mcode, malicious_code);
    cout << "Malicious code file: " << mcodes[i] << endl;
    cout << "Malicious code: " << malicious_code << endl;

    for (unsigned int i = 0; i < 2; i++) {
      string msg = "";
      std::ifstream transmission_file(transmissions[i]);
      cout << "Transmission file: " << transmissions[i] << endl;
      if (transmission_file.is_open()) {
        while (std::getline(transmission_file, msg)) {
          cout << "Transmission: " << msg << endl;
          int ans = KMPSearch(malicious_code, msg);
          if (ans >= 0)
            cout << "True at index: " << ans << endl;
          else
            cout << "False" << endl;
        }
      }
      cout << endl;
      transmission_file.close();
    }
    mcode.close();
  }

  /*  parte2 palndromo
       posiciónInicial posiciónFinal (para archivo de transmisión1)
       posiciónInicial posiciónFinal (para archivo de transmisión2)
    */
  cout << "---------------------------------------------------------------"
       << endl;
  cout << "--------------------- PARTE 2: Palindromo ---------------------"
       << endl;
  cout << "---------------------------------------------------------------"
       << endl;
  cout << endl;
  string trnsmn1 = "";
  string trnsmn2 = "";
  std::ifstream transmission_file_1(transmissions[2]);
  cout << "file: " << transmissions[2] << endl;
  std::getline(transmission_file_1, trnsmn1);
  cout << "file: " << transmissions[3] << endl;
  std::ifstream transmission_file_2(transmissions[3]);
  std::getline(transmission_file_2, trnsmn2);

  manacher_mirror(trnsmn1);
  manacher_mirror(trnsmn2);
  transmission_file_1.close();
  transmission_file_2.close();

  /* Test string
   * longest palyndrome = dabalearrozalazorraelabad
   */
  manacher_mirror("mnzbchzvcbdabalearrozalazorraelabadqwarrozaerarrozaty");

  /*parte 3
      posiciónInicial posiciónFinal (de substring común más largo entre dos
     archivos de transmisión )*/

  cout << "------------------------------------------------------" << endl;
  cout << "--------------------- PARTE 3: LCS -------------------" << endl;
  cout << "-------------------------------------------------------" << endl;
  cout << endl;
  trnsmn1 = "";
  trnsmn2 = "";
  std::ifstream part3_file1(transmissions[6]);
  std::ifstream part3_file2(transmissions[7]);
  cout << "file: " << transmissions[6] << "& " << transmissions[7] << endl;
  std::getline(part3_file1, trnsmn1);
  std::getline(part3_file2, trnsmn2);
  cout << lcs(trnsmn1, trnsmn2) << endl;
  cout << trnsmn2.substr(87, 120-87) << endl;
  // lcs("abcbc", "ababcbccabcbcbcaabcbc");
  part3_file1.close();
  part3_file2.close();
}