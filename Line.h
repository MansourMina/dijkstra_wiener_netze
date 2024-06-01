//
// Created by MansourMina on 31.05.2024.
//

#ifndef LINE_H
#define LINE_H
#include <vector>
#include "Station.h"
#include <algorithm>


class Line {
    std::string _name;
    std::vector<Station*> _stations;
public:
    Line(std::string lineName);
    ~Line();
    std::string get_name() const;
    std::vector<Station*> get_stations() const;
    void add_station(Station* station);
    Station* get_station(std::string name) const;
};



#endif //LINE_H
