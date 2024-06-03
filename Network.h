//
// Created by MansourMina on 31.05.2024.
//

#ifndef NETWORK_H
#define NETWORK_H
#include <unordered_map>
#include "Station.h"
#include "Line.h"
#include <algorithm>

class Network {
    std::vector<Line*> _network;
    std::string _name;
public:
    Network(std::string name);
    std::string get_name() const;
    void add_line(Line* line);
    Line* get_line(std::string line);
    Station* get_station(std::string stationName);
    std::vector<Line*>get_network() const;
};



#endif //NETWORK_H
