#include "levenshtein.h"
#include <bits/stdc++.h>

int main() {
    int n; std::cin >> n;
    std::vector<std::string> v;
    for(int i = 0; i < n; i++){
        std::string tmp;
        std::cin >> tmp;
        v.push_back(tmp);
    }    
    int T; std::cin >> T;
    while(T--){
        std::string s;
        std::cin >> s;
        Levenshtein L;
        printf("%s\n", L.levenshtein(v, s).c_str());
    }
}