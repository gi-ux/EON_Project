//
// Created by andre on 19.04.2021.
//

#include "Path.h"

Path::Path(int pathId, int idFrom, int idTo, int length) : path_id(pathId), id_from(idFrom), id_to(idTo),
                                                           length(length) {}

Path::~Path() {

}

void Path::append_link(Link link) {
    Path::links.push_back(link);
}

int Path::n_links() {
    return Path::links.size();
}

bool Path::share_link(Path path) {
    for(auto &link1 : Path::links) {
        for (auto &link2 : path.links) {
            if (link1.id_from == link2.id_from && link1.id_to == link2.id_to) {
                return true;
            }
        }
    }
    return false;
}

std::string Path::to_string() {
    std::string links;
    for(auto &link : Path::links) {
        links.append("link " + std::to_string(link.id_from) + " , " + std::to_string(link.id_to));
    }

    return std::string("path id: " + std::to_string(path_id) + " id from: " + std::to_string(id_from) + " id to: " + std::to_string(id_to) + " length: " + std::to_string(length) + " links: ").append(links);
}

