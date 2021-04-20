//
// Created by Thefefo3 on 20/04/2021.
//

#ifndef MAIN_CPP_PATHSFORT_H
#define MAIN_CPP_PATHSFORT_H


#include "Path.h"

class PathsForT {
public:
    int id_pair;
    std::vector<Path> paths;

    PathsForT(int idPair);

    void add_path(Path p);
};


#endif //MAIN_CPP_PATHSFORT_H
