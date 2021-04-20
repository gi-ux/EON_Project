//
// Created by andre on 19.04.2021.
//

#include <vector>
#include <string>
#include "Link.h"

#ifndef EON_PROJECT_PATH_H
#define EON_PROJECT_PATH_H


class Path {
public:
    int path_id;
    int id_from;
    int id_to;
    int length;
    std::vector<Link> links;

    Path(int pathId, int idFrom, int idTo, int length);

    virtual ~Path();

    void append_link(Link link);

    int n_links();

    bool share_link(Path path);

    std::string to_string();

    friend bool operator== (const Path &p1, const Path &p2);

    friend bool operator!= (const Path &p1, const Path &p2);
};


#endif //EON_PROJECT_PATH_H
