//
// Created by Thefefo3 on 26/04/2021.
//

#include "Data.h"
using namespace std;

void greedy_heuristic(Data mod){
    for(auto &p : mod.paths){
        cout << p.to_string() << endl;
    }
}
int main(){
    Data mod1("..\\files\\1paths", "..\\files\\demand_50_200_safe");
    mod1.init();
    mod1.write_first_dat("test-finale");

    greedy_heuristic(mod1);
}

