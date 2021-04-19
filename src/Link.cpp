//
// Created by andre on 19.04.2021.
//

#include "Link.h"



Link::~Link() {

}

void Link::add_path(int path_id) {
    Link::traversed_paths.push_back(path_id);
}

Link::Link(int idFrom, int idTo) : id_from(idFrom), id_to(idTo) {}
