#include "Util/parse.h"
#include "Util/assert.h"

#include <iostream>
#include <string>

using namespace cwerg;
using namespace std;

void EmitId(string_view s) { cout << "{" << s << "}"; }

void EmitString(string_view s) {
  if (s.size() == 2) {
    cout << s;
    return;
  }

  char buffer[4 * 1024];
  ASSERT(s.size() < sizeof(buffer), "string too large");

  uint32_t size = EscapedStringToBytes({s.data() + 1, s.size() - 2}, buffer);
  if (size == 0) {
    cout << "@FAILED@";
    return;
  }

  char buffer2[4 * 1024];
  size = BytesToEscapedString({buffer, size}, buffer2);
  buffer2[size] = 0;
  cout << "\"" << buffer2 << "\"";
}

int main() {
  std::vector<string_view> vec;
  for (string line; getline(std::cin, line);) {
    cout << "\n" << line << "\n";
    vec.clear();
    if (!ParseLineWithStrings(line.c_str(), false, &vec)) {
      cout << "@FAILED@\n";
      continue;
    }
    const char* sep = "";
    for (const string_view& s : vec) {
      cout << sep;
      sep = " ";
      ASSERT(s.size() > 0, "");
      if (s[0] == '"') {
        EmitString(s);
      } else {
        EmitId(s);
      }
    }
    cout << "\n";
    if (vec.size() == 2) {
      char buf[32];
      if (vec[0] == "int64") {
        cout << "[INT64] ";
        auto val = ParseInt64(vec[1]);
        if (!val.has_value()) {
          cout << "@BAD VALUE@\n";
          continue;
        }
        cout << ToDecSignedString(val.value(), buf) << " "
             << ToHexString(val.value(), buf) << "\n";
      } else if (vec[0] == "uint64") {
        cout << "[UINT64] ";
        auto val = ParseUint64(vec[1]);
        if (!val.has_value()) {
          cout << "@BAD VALUE@\n";
          continue;
        }
        cout << ToDecString(val.value(), buf) << " "
             << ToHexString(val.value(), buf) << "\n";
      } else if (vec[0] == "flt64") {
        cout << "[FLT64] ";
        auto val = ParseFlt64(vec[1]);
        if (!val.has_value()) {
          cout << "@BAD VALUE@\n";
          continue;
        }
        cout << ToFltString(val.value(), buf) << " "
             << ToFltHexString(val.value(), buf) << "\n";
      }
    }
  }
  return 0;
}
