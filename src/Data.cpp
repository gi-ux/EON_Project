//
// Created by andre on 19.04.2021.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Path.h"
#include "Link.h"
#include <algorithm>
#include "NDArray.cpp"
#include "Demand.h"

using namespace std;

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

int main() {
    vector<Path> paths;
    vector<pair<int, int>> k1k2;
    vector<Demand> demands;

    read_paths(paths, "..\\files\\1paths");
    for(auto &path : paths) {
        cout << path.to_string() << endl;
    }

    NDArray share_link{{paths.size(), paths.size()}};

    compute_k1k2(paths, share_link, k1k2);

    for(auto &pair : k1k2){
        cout << pair.first << " - " << pair.second << endl;
    }

    read_demands(demands, "..\\files\\demand_50_200_safe");

    for(auto &d : demands){
        cout << d.id_from << " " << d.id_to << " " << d.dem << endl;
    }

    return 0;
}