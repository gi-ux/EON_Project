//
// Created by andre on 19.04.2021.
//
#include <vector>


#ifndef EON_PROJECT_LINK_H
#define EON_PROJECT_LINK_H


class Link {
public:
    int id_from;
    int id_to;
    std::vector<int> traversed_paths;

    Link(int idFrom, int idTo);

    ~Link();

    void add_path(int path_id);
};


#endif //EON_PROJECT_LINK_H
