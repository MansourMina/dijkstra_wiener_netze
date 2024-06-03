//
// Created by MansourMina on 31.05.2024.
//

#include "Network.h"

Network::Network(std::string name) {
    _name = name;
}

void Network::add_line(Line *newLine) {
    if (!get_line(newLine->get_name()))
        _network.push_back(newLine);
}

Line *Network::get_line(std::string line) {
    for (auto cline: _network) {
        if (cline->get_name() == line) return cline;
    }
    return nullptr;
}

Station *Network::get_station(std::string stationName) {
    for (auto cline: _network) {
        Station *station = cline->get_station(stationName);
        if (station != nullptr) return station;
    }
    return nullptr;
}

std::vector<Line *> Network::get_network() const {
    return _network;
}
