//
// Created by MansourMina on 31.05.2024.
//

#ifndef STATION_H
#define STATION_H
#include <unordered_map>
#include <string>
#include <iostream>

struct connection {
    int weight;
    std::string line;
};
class Station {
    std::string _name;
    std::unordered_map<Station*, connection> _connections;

public:
    Station(std::string stationName);
    ~Station();
    void add_connection(Station *station, int weight, std::string line);
    std::string get_name() const;
    int get_weight(Station* station) const;
    std::unordered_map<Station *, connection> get_connections() const;
};


#endif //STATION_H
