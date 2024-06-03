//
// Created by MansourMina on 31.05.2024.
//

#include "Line.h"

Line::Line(std::string lineName) {
    _name = lineName;
}

std::vector<Station*> Line::get_stations() const {
    return  _stations;
}

std::string toLower(std::string letter) {
    for (auto& x : letter) {
        x = tolower(x);
    }
    return letter;
}

Station* Line::get_station(std::string name) const {
    for(auto station: _stations) {
        if(toLower(station->get_name()) == toLower(name)) return station;
    }
    return nullptr;
}

std::string Line::get_name() const {
    return _name;
}

void Line::add_station(Station *station) {
    int cnt = std::count(_stations.begin(), _stations.end(), station);

    if (cnt <= 0) {
        _stations.push_back(station);
    }
}
