//
// Created by MansourMina on 31.05.2024.
//

#include "Station.h"

Station::Station(std::string stationName) {
    _name = stationName;
}

std::string Station::get_name() const {
    return this->_name;
}

std::unordered_map<Station *, connection> Station::get_connections() const {
    return this->_connections;
}

int Station::get_weight(Station *station) const {
    const auto connection = _connections.find(station);
    return connection->second.weight;
}

void Station::add_connection(Station *station, int weight, std::string line) {
    auto find_connection = _connections.find(station);
    if (find_connection == _connections.end()) {
        _connections[station] = {weight, line};
    }

}
