#include <iostream>
#include "Station.h"
#include "Line.h"
#include <fstream>
#include "Network.h"
#include <sstream>
#include <string>
#include <queue>
#include <algorithm>
#include <iomanip>


void setup(std::string &fileGraph, std::string &start, std::string &end) {
    do {
        std::cout << "Network: ";
        std::cin >> fileGraph;
    } while (fileGraph.length() <= 0);

    do {
        std::cout << "Start: ";
        std::cin >> start;
    } while (start.length() <= 0);

    do {
        std::cout << "End: ";
        std::cin >> end;
    } while (end.length() <= 0);
}

void load_data(std::string filename, Network *network) {
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) return;

    Line *networkLine;
    bool newLine = true;
    std::string line;
    while (std::getline(file, line)) {
        std::string lineToRead, station, weight;
        std::stringstream ss(line);

        //Line
        std::getline(ss, lineToRead, ':');

        networkLine = new Line(lineToRead);

        bool toConnect = false;
        bool lineDone = false;

        Station *newStation = nullptr;
        int weightToConnect = 0;
        while (!lineDone) {
            if (newLine) // Lese bis zum ersten Anführungszeichen
                std::getline(ss, station, '"');
            if (!std::getline(ss, station, '"')) lineDone = true; // Lese bis zum zweiten Anführungszeichen
            if (!std::getline(ss, weight, '"')) lineDone = true;
            if (toConnect) {
                Station *stationToConnect = network->get_station(station);
                if (!stationToConnect) {
                    stationToConnect = new Station(station);
                }
                newStation->add_connection(stationToConnect, weightToConnect, networkLine->get_name());
                stationToConnect->add_connection(newStation, weightToConnect, networkLine->get_name());
                networkLine->add_station(newStation);
                if (!lineDone) {
                    newStation = stationToConnect;
                    weightToConnect = std::stoi(weight);
                } else networkLine->add_station(stationToConnect);
            } else {
                newStation = network->get_station(station);
                if (!newStation) {
                    newStation = new Station(station);
                }
                weightToConnect = std::stoi(weight);
                toConnect = true;
            }
            newLine = false;
        }
        newLine = true;
        network->add_line(networkLine);
    }

    file.close();
}

using StationPair = std::pair<int, Station *>;

struct pathStruct {
    Station *station;
    int weight;
    std::string line;
};

std::vector<pathStruct> buildPath(std::unordered_map<Station *, int> dist,
                                  std::unordered_map<Station *, std::pair<Station *, std::string> > prev,
                                  Station *startStation, Station *endStation) {
    std::vector<pathStruct> path;

    for (auto at = endStation; at != nullptr; at = prev[at].first) {
        pathStruct ps;
        ps.station = at;
        ps.weight = dist[at];
        ps.line = prev[at].second;
        path.push_back(ps);
    }

    return path;
}

std::vector<pathStruct> find_path(std::string fileGraph, Network *network, std::string start, std::string end) {
    load_data(fileGraph, network);

    using StationPair = std::pair<int, Station *>;
    std::priority_queue<StationPair, std::vector<StationPair>, std::greater<StationPair> > minHeap;
    std::unordered_map<Station *, int> dist;
    std::unordered_map<Station *, std::pair<Station *, std::string> > prev;

    Station *startStation = network->get_station(start);
    Station *endStation = network->get_station(end);

    for (auto line: network->get_network()) {
        for (auto station: line->get_stations()) {
            dist[station] = INT_MAX;
        }
    }
    minHeap.push(std::make_pair(0, startStation));
    dist[startStation] = 0;

    while (!minHeap.empty()) {
        int distance = minHeap.top().first;
        Station *currStation = minHeap.top().second;
        minHeap.pop();

        if (currStation == endStation) {
            break;
        }
        for (auto &[adjStation, connection]: currStation->get_connections()) {
            int edgeWeight = connection.weight;

            if (distance + edgeWeight < dist[adjStation]) {
                dist[adjStation] = distance + edgeWeight;
                prev[adjStation] = std::make_pair(currStation, connection.line);
                minHeap.push(std::make_pair(dist[adjStation], adjStation));
            }
        }
    }

    auto path = buildPath(dist, prev, startStation, endStation);
    std::reverse(path.begin(), path.end());
    return path;
}

void printPath(std::vector<pathStruct> path) {
    std::string currLine;
    std::cout << "From: " << path.begin()->station->get_name() << std::endl;
    std::cout << "To: " << path.back().station->get_name() << std::endl;
    std::cout << std::endl;
    for (auto it = path.begin(); it != path.end(); ++it) {
        if (it->line != currLine) {
            std::cout << "Line: " << it->line << std::endl;
            currLine = it->line;
        }
        std::cout << std::setw(4) << it->station->get_name() << std::endl;
        if(std::next(it) != path.end())std::cout << std::setw(4) << "." << std::endl;
    }
}


int main() {
    Network *network = new Network("Wiener Netze");
    std::string fileGraph = "C:\\Users\\manso\\Downloads\\ADS_Programmieraufgabe3_WienerVerkehrsNetz.txt";
    auto path = find_path(fileGraph, network, "Vorgartenstrasse", "Stadion");
    printPath(path);


    // Station* station = network->get_station("Stephansplatz");
    // if(station) {
    //     for(auto [stationName, connection]: station->get_connections()) {
    //         std::cout << "To: " << stationName->get_name() << " Fahrzeit: " << connection.weight << " Line: " << connection.line << std::endl;
    //     }
    // }

    //std::string fileGraph, start, end;
    //setup(fileGraph, start, end);

    //find_path(fileGraph, start, end);
    return 0;
}
