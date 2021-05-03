//
// Created by andre on 19.04.2021.
//

#include "Data.h"

#define EQUAL ":="
#define STRING '"'
#define CLOSE ";"

using namespace std;

Data::Data(string pathFilename, string demandsFilename) : path_filename(std::move(
        pathFilename)), demands_filename(std::move(demandsFilename)), q(0), g(12.5), b(37.5), s(4000) {

}

Data::Data(const Data &d){

}

void Data::init(){
    paths = vector<Path>();
    k1k2 = vector<std::pair<int, int>>();
    demands = vector<Demand>();
    kts = vector<PathsForT>();
    reach = vector<vector<int>>();
    modulations = {"BPSK", "QPSK", "8 QAM", "16 QAM", "32 QAM", "64 QAM"};


    read_paths();

    share_link = vector<vector<int>>(paths.size(), vector<int>(paths.size()));
    compute_k1k2();

    read_demands();
    compute_Kt();
    read_reach();

    vector<int> d2(paths.size(), 0);
    lambda = vector<vector<int>> (modulations.size(), d2);
    compute_lambda();

    compute_q();
    compute_r();
}

//Split string using space as delimiter
vector<string> Data::split(string s)
{
    stringstream ss(s);
    string word;
    vector<string> res;
    while (ss >> word) {
        res.push_back(word);
    }
    return res;
}

void Data::read_paths() {
    string content;
    ifstream file(path_filename);
    while(getline(file, content)) {
        vector<string> line = split(content);
        if(!line.empty()) {
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

void Data::compute_k1k2(){

    for(auto &p1 : paths){
        for(auto &p2 : paths){
             if(p2.path_id < p1.path_id){
                 continue;
             }
             if((p1 != p2) and (p1.id_from != p2.id_from || p1.id_to != p2.id_to) and p1.share_link(p2)){
                share_link.at(p1.path_id).at(p2.path_id) = 1;
                share_link.at(p2.path_id).at(p1.path_id) = 1;
                k1k2.emplace_back(p1.path_id, p2.path_id);
             }
        }
    }
}

void Data::read_demands(){
    string content;
    ifstream file(demands_filename);

    while(getline(file, content)) {
        vector<string> line = split(content);
        if(!line.empty()) {

            Demand d(stoi(line[0]), stoi(line[1]), stoi(line[2]));
            demands.push_back(d);
        }
    }
    file.close();
}

void Data::compute_Kt(){
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

void Data::read_reach(){
    string content;
    ifstream file("..\\files\\reach");

    while(getline(file, content)) {
        vector<string> line = split(content);
        if(!line.empty()){
            vector<int> col;
            for(auto &data : line){
                col.push_back(stoi(data));
            }
            reach.push_back(col);
        }
    }
    file.close();
}

vector<int> Data::reach_col(int demand){
    vector<int> col;
    for(auto &r_ele : reach){
        for(int i = 0; i <= r_ele.size(); i++){
            if((demand / 50 - 1) == i)
                col.push_back(r_ele[i]);
        }
    }
    return col;
}

void Data::compute_lambda(){
    for(auto &p : paths){
        for(auto &d : demands){
            if(p.id_from == d.id_from && p.id_to == d.id_to){
                vector<int> r_col = reach_col(d.dem);
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

void Data::compute_q(){
    int max;
    int min;
    for (int i = 0; i < demands.size(); ++i) {
        if (i == 0) {
            min = demands[i].dem;
            max = demands[i].dem;
        } else if (demands[i].dem > max) {
            max = demands[i].dem;
        } else if (demands[i].dem < min) {
            min = demands[i].dem;
        }
    }
    q = max/min;
}

void Data::compute_r(){
    for(int i = 1; i<= 6; i++)
    {
        r.insert(pair<string, int>(modulations[i-1],i*50));
    }
}

template <typename T>
std::string NumberToString (T Number)
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

void Data::write_first_dat(string filename){
    ofstream outfile;
    outfile.open("../" + filename + ".dat");
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
    outfile << "param Q " EQUAL <<" " + data << CLOSE << endl;
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


void Data::write_second_dat(string filename, map<string, vector<int>> &b_star, double smax_star, map<string, vector<int>> &beta_star){
    ofstream outfile;
    outfile.open("../" + filename + ".dat");
    cout << "Writing to the file" << endl;
    string data;

    // Write K
    for(auto &p : paths)
    {
        data.append(" ");
        data.append(NumberToString(p.path_id));
    }
    data.append(";");
    outfile << "set K " EQUAL <<data << endl;
    data.clear();

    //Write M
    for(auto & i : modulations)
    {
        data.append(" ");
        data.append(STRING+i+STRING);
    }
    outfile << "set M " EQUAL << data << CLOSE << endl;
    data.clear();

    //Write K1K2
    for(auto &k12 : k1k2){
        data.append(" ");
        data.append(NumberToString(k12.first) + " " + NumberToString(k12.second) + "\n");
    }
    data.append(";");
    outfile << "set K1K2 " EQUAL << data << endl;
    data.clear();

    //Write B
    data.append(NumberToString(b));
    outfile << "param B " EQUAL << " " + data << CLOSE << endl;
    data.clear();

    //Write S
    data.append(NumberToString(s));
    outfile << "param S " EQUAL << " " + data << CLOSE << endl;
    data.clear();

    //Write G
    data.append(NumberToString(g));
    outfile << "param G " EQUAL <<" " + data << CLOSE << endl;
    data.clear();

    //b_star
    outfile << "param b_star :";
    for(auto &p : paths){
        outfile << " " << p.path_id;
    }
    outfile << " :=" << endl;
    map<string, vector<int>>::iterator it;
    for(it = b_star.begin(); it!=b_star.end(); ++it){
        outfile  << it->first;
        for(int i = 0; i < it->second.size(); i++){
            outfile <<" " << it->second[i];
        }
        outfile << endl;
    }
    outfile << ";" << endl;

    //Smax_star
    outfile << "param Smax_star := ";
    outfile << smax_star << ";" << endl;

    //beta_star
    outfile << "param beta_star :";
    for(auto &p : paths){
        outfile << " " << p.path_id;
    }
    outfile << " :=" << endl;
    for(it = beta_star.begin(); it!=beta_star.end(); ++it){
        outfile  << it->first;
        for(int i = 0; i < it->second.size(); i++){
            outfile <<" " << it->second[i];
        }
        outfile << endl;
    }
    outfile << ";" << endl;
}