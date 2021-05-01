//
// Created by Thefefo3 on 26/04/2021.
//

#include "Data.h"
#include "ampl/ampl.h"

using namespace std;

void greedy_heuristic(Data &mod){
    for(auto &p : mod.paths){
        cout << p.to_string() << endl;
    }
}

int countTransceiver(Data &mod){
    int count = 0;
    for(auto &p : mod.paths){
        for(auto &l : p.links){
            count++;
        }
    }
    return count;
}

string controlModulation(int dem) {
    if(dem == 50) {
        return "BPSK";
    }else if(dem == 100) {
        return "QPSK";
    }else if(dem == 150) {
        return "8 QAM";
    }else if(dem == 200) {
        return "16 QAM";
    }else if(dem == 250) {
        return "32 QAM";
    }else if(dem == 300) {
        return "64 QAM";
    }
}

void setPathModulation(Data &mod) {
    for(auto &d : mod.demands){
        for(auto &p : mod.paths){
            if(d.id_from == p.id_from && d.id_to == p.id_to){
                p.setModulation(controlModulation(d.dem));
                break;
            }
        }
    }
}

int getIntModulation(string modulation) {
    if(modulation.compare("BPSK") == 0) {
        return 0;
    }else if(modulation.compare("QPSK") == 0) {
        return 1;
    }else if(modulation.compare("8 QAM") == 0) {
        return 2;
    }else if(modulation.compare("16 QAM") == 0) {
        return 3;
    }else if(modulation.compare("32 QAM") == 0) {
        return 4;
    }else if(modulation.compare("64 QAM") == 0) {
        return 5;
    }
}

void controlModulation(Data &mod) {
    for(auto &p : mod.paths){
        if(mod.lambda[getIntModulation(p.modulation)][p.path_id] == 0){
            p.setModulation("No modulation possible!");
        }
    }
}

int main(){
    Data mod1("..\\files\\1paths", "..\\files\\demand_50_100_safe");
    mod1.init();
    cout << countTransceiver(mod1) << endl;
    setPathModulation(mod1);
    controlModulation(mod1);
    greedy_heuristic(mod1);
    /*
    mod1.write_first_dat("test-finale");

    ampl::Environment env("ampl");
    ampl::AMPL ampl(env);
    ampl.setOption("solver", "gurobi");
    ampl.setOption("gurobi_options", "outlev=1 mipgap=0.03");
    ampl.read("../progetto.mod");
    ampl.readData("../test-finale.dat");
    ampl.solve();

    ampl::Variable smax = ampl.getVariable("Smax");
    cout << "Smax: " << smax.get().value() << endl;

    ampl::Variable f = ampl.getVariable("f");
    ampl::DataFrame df = f.getValues();
    cout << "f" << f[0].value() << endl;

    //Get z
    ampl::Objective z = ampl.getObjective("z");
    cout << z.get().value() << endl;
    */


}

