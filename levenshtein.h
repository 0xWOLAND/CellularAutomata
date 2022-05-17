#ifndef LEV_DIST
#define LEV_DIST
#include <bits/stdc++.h>
const int mx = 1e5;

class Levenshtein {
  public:
    Levenshtein() {}
    int ffind(std::vector<std::string> v, std::string s1, std::string s2){
    const std::size_t len1 = s1.size(), len2 = s2.size();
        std::vector<std::vector<int>> d(len1 + 1, std::vector<int>(len2 + 1));

        d[0][0] = 0;
        for(int i = 1; i <= len1; ++i) d[i][0] = i;
        for(int i = 1; i <= len2; ++i) d[0][i] = i;

        for(int i = 1; i <= len1; ++i)
            for(int j = 1; j <= len2; ++j)
                        d[i][j] = std::min({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) });
        return d[len1][len2];
    }
    int levenshtein(std::vector<std::string> v, std::string s){
        int low = mx;
        int ans;
        for(int i = 0; i < v.size(); i++){
            int k = ffind(v, v[i], s);
            if(low > k){
                low = k;
                ans = i;
            }
        }
        return ans + 1;
    }
};
#endif