#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

void readfile(string name)
{
    string content;
    ifstream file("..\\files\\" + name);
    while(getline(file, content)) {
        cout<<content<<endl;
    }
    file.close();

}

vector<int> getNodes(string name)
{
    vector<int> nodes;
    int i = 0;
    string content;
    ifstream file("..\\files\\" + name);
    while(getline(file, content)) {
        nodes.push_back(i);
        i++;
    }
    file.close();
    return nodes;
}

template <class T>
void printData(vector<T> vec) {
    for(int i = 0; i<vec.size(); i++)
    {
        cout<<vec[i]<<endl;
    }
}


int main() {

    //get nodes (T)
    vector<int> nodes = getNodes("demand_50_100_safe");
    printData(nodes);

    //set modulations (M)
    vector<string> mod = {"BPSK", "QPSK", "8 QAM", "16 QAM", "32 QAM", "64 QAM"};
    printData(mod);

    

    return 0;
}
