//
// Created by andre on 19.04.2021.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <map>

#include "NDArray.cpp"
#include "Demand.h"
#include "PathsForT.h"
#include "Path.h"
#include "Link.h"

#define EQUAL ":="
#define STRING '"'
#define CLOSE ";"

using namespace std;

//Split string using space as delimiter
vector<string> split(string s)
{
    stringstream ss(s);
    string word;
    vector<string> res;
    while (ss >> word) {
        res.push_back(word);
    }
    return res;
}

void read_paths(vector<Path> &paths, string filename) {
    string content;
    ifstream file(filename);
    while(getline(file, content)) {
        vector<string> line = split(content);
        if(line.size() > 0) {
            Path pfw(stoi(line[0])*2, stoi(line[1]), stoi(line[2]), stoi(line[4]));
            Path pbw(stoi(line[0])*2+1, stoi(line[2]), stoi(line[1]), stoi(line[4]));
            for(int i = 1; i < stoi(line[5]); i++) {
                Link lfw(stoi(line[5 + i]), stoi(line[5 + i + 1]));
                pfw.append_link(lfw);
                lfw.add_path(pfw.path_id);
                Link lbw(stoi(line[5 + i + 1]), stoi(line[5 + i]));
                pbw.append_link(lbw);
                lbw.add_path(pbw.path_id);
            }
            reverse(pbw.links.begin(), pbw.links.end());
            paths.push_back(pfw);
            paths.push_back(pbw);
        }
    }
}

void compute_k1k2(vector<Path> &paths, NDArray &share_link, vector<pair<int, int>> &interferring_paths){
    int npaths = paths.size();

    for(auto &p1 : paths){
        for(auto &p2 : paths){
             if(p2.path_id < p1.path_id){
                 continue;
             }
             if((p1 != p2) and (p1.id_from != p2.id_from || p1.id_to != p2.id_to) and p1.share_link(p2)){
                share_link[{static_cast<unsigned long long>(p1.path_id), static_cast<unsigned long long>(p2.path_id)}] = 1;
                share_link[{static_cast<unsigned long long>(p2.path_id), static_cast<unsigned long long>(p1.path_id)}] = 1;
                interferring_paths.emplace_back(p1.path_id, p2.path_id);
             }
        }
    }
}

void read_demands(vector<Demand> &demands, string filename){
    string content;
    ifstream file("..\\files\\" + filename);

    while(getline(file, content)) {
        vector<string> line = split(content);
        if(line.size() > 0) {
            Demand d(stoi(line[0]), stoi(line[1]), stoi(line[2]));
            demands.push_back(d);
        }
    }
    file.close();
}

void compute_Kt(vector<PathsForT> &kts, vector<Path> &paths, vector<Demand> &demands){
    int index = 0;
    for(auto &d : demands){
        PathsForT kt(index);
        for(auto &p : paths){
            if(d.id_from == p.id_from && d.id_to == p.id_to){
                kt.add_path(p);
            }
        }
        index++;
        kts.push_back(kt);
    }
}

vector<vector<int>> read_reach(){
    vector<vector<int>> reach;
    string content;
    ifstream file("..\\files\\reach");

    while(getline(file, content)) {
        vector<string> line = split(content);
        if(line.size() > 0){
            vector<int> col;
            for(auto &data : line){
                col.push_back(stoi(data));
            }
            reach.push_back(col);
        }
    }
    file.close();

    return reach;
}
vector<int> reach_col(int demand, vector<vector<int>> &reach){
    vector<int> col;
    for(auto &r : reach){
        for(int i = 0; i <= r.size(); i++){
            if((demand / 50 - 1) == i)
                col.push_back(r[i]);
        }
    }
    return col;
}

void compute_lambda(vector<Path> &paths, vector<Demand> &demands, vector<vector<int>> &reach, vector<vector<int>> &lambda){
    for(auto &p : paths){
        for(auto &d : demands){
            if(p.id_from == d.id_from && p.id_to == d.id_to){
                vector<int> r_col = reach_col(d.dem, reach);
                for(int i = 0; i < r_col.size(); i++){
                    if(r_col[i] > p.length)
                        lambda.at(i).at(p.path_id) = 1;
                    else
                        lambda.at(i).at(p.path_id) = 0;
                }
            }
        }
    }
}

template <typename T>
std::string NumberToString (T Number)
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

void write_first_dat(string filename, vector<Path> paths, vector<pair<int, int>> k1k2, vector<Demand> demands, vector<PathsForT> kts, vector<string> modulations, int q,float g, float b, int s, map<string, int> r, vector<vector<int>> lambda){
    ofstream outfile;
    outfile.open(filename + ".dat");
    cout << "Writing to the file" << endl;
    string data;

    //Write M
    for(auto & i : modulations)
    {
        data.append(" ");
        data.append(STRING+i+STRING);
    }
    outfile << "set M " EQUAL << data << CLOSE << endl;
    data.clear();

    //Write T
    for(int i = 0; i < demands.size(); i++)
    {
        data.append(" ");
        data.append(NumberToString(i));
    }
    outfile << "set T " EQUAL << data << CLOSE << endl;
    data.clear();

    // Write K
    for(auto &p : paths)
    {
        data.append(" ");
        data.append(NumberToString(p.path_id));
    }
    data.append(";");
    outfile << "set K " EQUAL <<data << endl;
    data.clear();

    //Write K1K2
    for(auto &k12 : k1k2){
        data.append(" ");
        data.append(NumberToString(k12.first) + " " + NumberToString(k12.second) + "\n");
    }
    data.append(";");
    outfile << "set K1K2 " EQUAL << data << endl;
    data.clear();

    //Write Kts
    outfile << endl;
    for(auto &kt : kts){
        outfile << "set Kt[" << kt.id_pair << "] :=";
        for(auto &p : kt.paths){
            outfile << " " << p.path_id;
        }
        outfile << ";" << endl;
    }

    //Write Q
    data.append(NumberToString(q));
    outfile << "param Q " EQUAL << data << CLOSE << endl;
    data.clear();

    //Write G
    data.append(NumberToString(g));
    outfile << "param G " EQUAL <<" " + data << CLOSE << endl;
    data.clear();

    //Write B
    data.append(NumberToString(b));
    outfile << "param B " EQUAL << " " + data << CLOSE << endl;
    data.clear();

    //Write S
    data.append(NumberToString(s));
    outfile << "param S " EQUAL << " " + data << CLOSE << endl;
    data.clear();

    //Write R
    for (auto& x: r) {
        data.append(" ");
        data.append(STRING+x.first+STRING);
        data.append(" ");
        data.append(NumberToString(x.second));
    }
    outfile << "param r " EQUAL << data << CLOSE << endl;
    data.clear();

    //Write D
    for(int i=0; i<demands.size(); i++)
    {
        data.append(" ");
        data.append(NumberToString(i));
        data.append(" ");
        data.append(NumberToString(demands[i].dem));
    }
    outfile << "param d " EQUAL << data << CLOSE << endl;
    data.clear();

    //Write Lambda
    outfile << "param Lam :";
    for(auto &p : paths){
        outfile << " " << p.path_id;
    }
    outfile << " :=" << endl;
    for(int i = 0; i < lambda.size(); i++){
        outfile << STRING << modulations[i] << STRING;
        for(auto &val : lambda[i]){
            outfile << " " << val;
        }
        outfile << endl;
    }
    outfile << ";" << endl;
}



int main() {
    vector<Path> paths;
    vector<pair<int, int>> k1k2;
    vector<Demand> demands;
    vector<PathsForT> kts;
    vector<vector<int>> reach = read_reach();
    vector<string> modulations = {"BPSK", "QPSK", "8 QAM", "16 QAM", "32 QAM", "64 QAM"};
    int q = 4;
    float g = 12.5;
    float b = 37.5;
    int s = 4000;
    map<string, int> r;
    for(int i = 1; i<= 6; i++)
    {
        r.insert(pair<string, int>(modulations[i-1],i*50));
    }

    //Read paths
    read_paths(paths, "..\\files\\1paths");
    for(auto &path : paths) {
        cout << path.to_string() << endl;
    }

    //K1K2
    NDArray share_link{{paths.size(), paths.size()}};
    compute_k1k2(paths, share_link, k1k2);
    for(auto &pair : k1k2){
        cout << pair.first << " - " << pair.second << endl;
    }

    //Demands
    read_demands(demands, "..\\files\\demand_50_200_safe");
    for(auto &d : demands){
        cout << d.id_from << " " << d.id_to << " " << d.dem << endl;
    }

    //Kts
    compute_Kt(kts, paths, demands);
    for(auto &kt : kts){
        cout << "Kt[" << kt.id_pair << "] = ";
        for(auto &p : kt.paths){
            cout << p.path_id << " ";
        }
        cout << endl;
    }


    //Lambda
    //int lambda[modulations.size()][paths.size()];
    vector<int> v(paths.size(), 0);
    vector<vector<int>> lambda(modulations.size(), v);
    compute_lambda(paths, demands, reach, lambda);
    for (auto &r : lambda) {
        for (int i = 0; i < r.size(); i++) {
            cout<< r[i] << " ";
        }
        cout << "endline" << endl;
    }

    write_first_dat("test-finale", paths, k1k2, demands, kts, modulations, q, g, b, s, r, lambda);

    return 0;
}