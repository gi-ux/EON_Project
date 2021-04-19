#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <string>
#include <sstream>
#include <set>
#include <algorithm>

#define EQUAL ":="
#define STRING '"'
#define CLOSE ";"
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

template<typename K, typename V>
void print_map(std::map<K,V> const &m)
{
    for (auto const& pair: m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

vector<int> getPaths(string name)
{
    int dimPaths = getNodes(name).size() * 2;
    vector<int> paths;
    for(int i = 0; i < dimPaths; i++)
    {
        paths.push_back(i);
    }
    return paths;
}

vector<int> getDemands(string demand)
{
    string delimiter = " ";
    vector<int> vector;
    int i = 0;
    string content;
    ifstream file("..\\files\\" + demand);

    while(getline(file, content)) {
        size_t pos = 0;
        string token;
        while ((pos = content.find(delimiter)) != std::string::npos) {
            token = content.substr(0, pos);
            content.erase(0, pos + delimiter.length());
        }
        vector.push_back(stoi(content));
    }
    file.close();
    return vector;
}

template <typename T>
std::string NumberToString (T Number)
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

void menuDemand()
{
    cout << "Select demand:" << endl;
    cout << "a -> demand_50_100_safe" << endl;
    cout << "b -> demand_50_200_safe" << endl;
    cout << "c -> demand_50_400_safe" << endl;
    cout << "d -> demand_100_200_safe" << endl;
    cout << "e -> demand_100_300_safe" << endl;
    cout << "f -> demand_100_400_safe" << endl;
    cout << "g -> demand_200_300_safe" << endl;
    cout << "h -> demand_200_400_safe" << endl;
}

void menuPaths()
{
    cout << "Select path:" << endl;
    cout << "1 -> 1paths" << endl;
    cout << "2 -> 2paths" << endl;
    cout << "3 -> 3paths" << endl;
}

vector<vector<string>> populate(string path)
{
    vector<vector<string>> container;
    vector<string> vector;
    string content;
    ifstream file("..\\files\\" + path);

    while(getline(file, content)) {
        vector.clear();
        std::istringstream iss(content);
//        cout<<content<<endl;
        for(std::string s; iss >> s; ) {
            vector.push_back(s);

        }
        container.push_back(vector);
    }
    file.close();
    return container;
}
/*
int main() {
    string demand;
    string path;
    char userInput='x';
    while (userInput != 'a' && userInput != 'b' && userInput != 'c' && userInput != 'd' && userInput != 'e' && userInput != 'f' && userInput != 'g' && userInput != 'h')
    {
        menuDemand();
        cin >> userInput;

        if (userInput == 'a')
        {
            demand = "demand_50_100_safe";
        }else if (userInput == 'b')
        {
            demand = "demand_50_200_safe";

        }else if(userInput == 'c')
        {
            demand = "demand_50_400_safe";

        }else if(userInput == 'd')
        {
            demand = "demand_100_200_safe";

        } else if(userInput == 'e')
        {
            demand = "demand_100_300_safe";

        } else if(userInput == 'f')
        {
            demand = "demand_100_400_safe";

        } else if(userInput == 'g')
        {
            demand = "demand_200_300_safe";

        } else if(userInput == 'h')
        {
            demand = "demand_200_400_safe";
        }else
        {
            cout << "Invalid entry for demand" << endl;
        }
    }

    while (userInput != '1' && userInput != '2' && userInput != '3')
    {
        menuPaths();
        cin >> userInput;

        if (userInput == '1')
        {
            path = "1paths";
        }else if (userInput == '2')
        {
            path = "2paths";

        }else if(userInput == '3')
        {
            path = "3paths";

        }else
        {
            cout << "Invalid entry for path" << endl;
        }
    }

    //const values (params)
    int q = 4;
    float g = 12.5;
    float b = 37.5;
    int s = 4000;

    //set modulations (M)
    vector<string> m = {"BPSK", "QPSK", "8 QAM", "16 QAM", "32 QAM", "64 QAM"};
    //printData(m);

    //get all nodes (T)
    vector<int> t = getNodes("demand_50_100_safe");
    //printData(t);

    //get all paths (K)
    vector<int> k = getPaths(path);
    //printData(k);

    //capacità di un transceiver che opera a una modulazione m (0, 50gb/s; 1, 100; ...)
    map<string, int> r;
    for(int i = 1; i<= 6; i++)
    {
        r.insert(pair<string, int>(m[i-1],i*50));
    }
    //print_map(r);

    //get all demands (D)
    vector<int> d = getDemands(demand);
    //printData(d);

    vector<pair<int, int>> k1k2;
    vector<vector<string>> fileParsed = populate("..\\test-path.txt");
    int init_index;
    for(auto & row : fileParsed)
    {
        row.erase(row.begin() + 1, row.begin() + 6);
    }

    for(auto & row : fileParsed)
    {
        for(auto & i : row)
            cout<<i<<" ";
        cout<<endl;
    }


    //WRITING ON FILE
    ofstream outfile;
    outfile.open("test-write.dat");
    cout << "Writing to the file" << endl;
    string data;

    //Writing M
    for(auto & i : m)
    {
        data.append(" ");
        data.append(STRING+i+STRING);
    }
    outfile << "set M " EQUAL << data << CLOSE << endl;
    data.clear();

    //Writing T
    for(int i : t)
    {
        data.append(" ");
        data.append(NumberToString(i));
    }
    outfile << "set T " EQUAL << data << CLOSE << endl;
    data.clear();

    // Writing K
    for(int i : k)
    {
        data.append(" ");
        data.append(NumberToString(i));
    }
    data.append(";");
    outfile << "set K " EQUAL <<data << endl;
    data.clear();

    //Writing Q
    data.append(NumberToString(q));
    outfile << "param Q " EQUAL << data << CLOSE << endl;
    data.clear();

    //Writing G
    data.append(NumberToString(g));
    outfile << "param G " EQUAL <<" " + data << CLOSE << endl;
    data.clear();

    //Writing B
    data.append(NumberToString(b));
    outfile << "param B " EQUAL << " " + data << CLOSE << endl;
    data.clear();

    //Writing S
    data.append(NumberToString(s));
    outfile << "param S " EQUAL << " " + data << CLOSE << endl;
    data.clear();

    //Writing R
    for (auto& x: r) {
        data.append(" ");
        data.append(STRING+x.first+STRING);
        data.append(" ");
        data.append(NumberToString(x.second));
    }
    outfile << "param r " EQUAL << data << CLOSE << endl;
    data.clear();

    //Writing D
    for(int i=0; i<d.size(); i++)
    {
        data.append(" ");
        data.append(NumberToString(i));
        data.append(" ");
        data.append(NumberToString(d[i]));
    }
    outfile << "param d " EQUAL << data << CLOSE << endl;
    data.clear();


    //Missing Lam, Kt, K1K2

    outfile.close();
    return 0;
}
*/