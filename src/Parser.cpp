//
// Created by Thefefo3 on 26/04/2021.
//

#include "Data.h"
#include "ampl/ampl.h"

using namespace std;

void greedy_heuristic(Data mod){
    for(auto &p : mod.paths){
        cout << p.to_string() << endl;
    }
}
int main(){
    Data mod1("..\\files\\1paths", "..\\files\\demand_50_100_safe");
    mod1.init();
    mod1.write_first_dat("test-finale");

    ampl::Environment env("ampl");
    cout << "test 1 " << endl;
    ampl::AMPL ampl(env);
    cout << "test 2 " << endl;
    ampl.setOption("solver", "gurobi");
    ampl.setOption("gurobi_options", "outlev=1 mipgap=0.03");
    cout << "test 3 " << endl;
    ampl.read("../progetto.mod");
    cout << "test 4 " << endl;
    ampl.readData("../test-finale.dat");
    cout << "test 5 " << endl;

    ampl.solve();

    ampl::Variable smax = ampl.getVariable("Smax");
    cout << "Smax: " << smax.get().value() << endl;

    ampl::Variable f = ampl.getVariable("f");
    ampl::DataFrame df = f.getValues();
    cout << "f" << f[0].value() << endl;

    //Get z
    ampl::Objective z = ampl.getObjective("z");
    cout << z.get().value() << endl;

    cout << "test 6 " << endl;
}

