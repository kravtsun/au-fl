#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <set>

#define forn(i, n) for (int i = 0; i < (int)(n); ++i)
#define forit(it, x) for (auto it = (x).begin(); it != (x).end(); ++it)
#define all(x) (x).begin(), (x).end()
using vint = std::vector<int>;
using vbool = std::vector<bool>;
using vvbool = std::vector<vbool>;
using vvvbool = std::vector<vvbool>;
using token_t = int;
using ptt = std::pair<token_t, token_t>;
using vptt = std::vector<ptt>;
using st = std::set<token_t>;
using vst = std::vector<st>;
using vvst = std::vector<vst>;
#endif // COMMON_H
