#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <string>
#include <sstream>

#define EQUAL ":="
#define STRING '"'


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

    //const values
    int q = 4;
    int g = 12.5;
    int b = 37.5;
    int s = 4000;
    //set modulations (M)
    vector<string> m = {"BPSK", "QPSK", "8 QAM", "16 QAM", "32 QAM", "64 QAM"};
//    printData(m);

    //get all nodes (T)
    vector<int> t = getNodes("demand_50_100_safe");
//    printData(t);

    //get all paths (K)
    vector<int> k = getPaths(path);
//    printData(k);

    //capacità di un transceiver che opera a una modulazione m (0, 50gb/s; 1, 100; ...)
    map<string, int> r;
    for(int i = 1; i<= 6; i++)
    {
        r.insert(pair<string, int>(m[i-1],i*50));
    }
//    print_map(r);

    //get all demands (D) todo avanti e indietro
    vector<int> d = getDemands(demand);
//    printData(d);

    ofstream outfile;
    outfile.open("test-write.dat");
    cout << "Writing to the file" << endl;
    string data;
    for(int i = 0; i<m.size(); i++)
    {
        data.append(" ");
        data.append(STRING+m[i]+STRING);
    }
    data.append(";");
    outfile << "set M " EQUAL <<data << endl;
    data.clear();
    for(int i=0; i<t.size(); i++)
    {
        data.append(" ");
        data.append(NumberToString(t[i]));
    }
    data.append(";");
    outfile << "set T " EQUAL <<data << endl;
    outfile.close();

    return 0;
}
