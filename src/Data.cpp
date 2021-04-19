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

int main() {
    vector<Path> paths;
    read_paths(paths, "..\\files\\1paths");
    for(auto &path : paths) {
        cout << path.to_string() << endl;
    }
    return 0;
}