//
// Created by Thefefo3 on 26/04/2021.
//

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "NDArray.cpp"
#include "Demand.h"
#include "PathsForT.h"
#include "Path.h"
#include "Link.h"

#ifndef MAIN_CPP_DATA_H
#define MAIN_CPP_DATA_H

#endif //MAIN_CPP_DATA_H

class Data{
public:
    std::string path_filename;
    std::string demands_filename;
    std::vector<Path> paths;
    std::vector<std::pair<int, int>> k1k2;
    std::vector<Demand> demands;
    std::vector<PathsForT> kts;
    std::vector<std::vector<int>> reach;
    std::vector<std::string> modulations;
    std::vector<std::vector<int>> share_link;
    float g;
    float b;
    int s;
    int q;
    std::map<std::string, int> r;
    std::vector<std::vector<int>> lambda;

    Data(std::string pathFilename, std::string demandsFilename);

    Data(const Data &d);

    static std::vector<std::string> split(std::string s);

    void write_first_dat(std::string filename);

    void init();


private:
    void read_paths();

    void compute_k1k2();

    void read_demands();

    void compute_Kt();

    void read_reach();

    std::vector<int> reach_col(int demand);

    void compute_lambda();

    void compute_q();

    void compute_r();
};