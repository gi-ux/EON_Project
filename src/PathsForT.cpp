//
// Created by Thefefo3 on 20/04/2021.
//

#include "PathsForT.h"

PathsForT::PathsForT(int idPair) : id_pair(idPair) {}

void PathsForT::add_path(Path p) {
    paths.push_back(p);
}
