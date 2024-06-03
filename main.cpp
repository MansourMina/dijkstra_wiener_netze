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
#include <limits>
#include <chrono>

using StationPair = std::pair<int, Station *>;

struct pathStruct {
    Station *station;
    int weight;
    std::string line;
};





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

bool valid_file(std::string &file) {
    std::fstream fileStream;
    fileStream.open(file);
    return !fileStream.fail();
}

void setup(std::string &fileGraph, Network *network, std::string &start, std::string &end) {
    do {
        std::cout << "Network:";
        if (!(std::cin >> fileGraph)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (fileGraph.length() <= 0 || !valid_file(fileGraph));

    load_data(fileGraph, network);

    while (true) {
        std::cout << "Start (" << network->get_name() << "):";
        if (!(std::cin >> start)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (start.length() > 0 && network->get_station(start)) break;
        std::cout << "Station not found!" << std::endl;
    }

    while (true) {
        std::cout << "Destination (" << network->get_name() << "):";
        if (!(std::cin >> end)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (end.length() > 0 && network->get_station(end)) break;
        std::cout << "Station not found!" << std::endl;
    }

}


std::vector<pathStruct> buildPath(std::unordered_map<Station *, int> dist,
                                  std::unordered_map<Station *, std::pair<Station *, std::string> > prev,
                                  Station *endStation) {
    std::vector<pathStruct> path;

    for (auto at = endStation; at != nullptr; at = prev[at].first) {
        pathStruct ps;
        ps.station = at;
        ps.weight = dist[at];
        ps.line = prev[at].second;
        path.push_back(ps);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<pathStruct> find_path(Network *network, const std::string &start, const std::string &end) {
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
        for (auto &[adjStation, connection]: currStation->get_connections()) {
            if (distance + connection.weight < dist[adjStation]) {
                dist[adjStation] = distance + connection.weight;
                prev[adjStation] = std::make_pair(currStation, connection.line);
                minHeap.push(std::make_pair(dist[adjStation], adjStation));
            }
        }
    }
    return buildPath(dist, prev, endStation);
}

std::string getCurrentTime(const int addedTime = 0) {
    std::stringstream ss;
    auto now = std::chrono::system_clock::now();
    auto calculateTime = now + std::chrono::minutes(addedTime);
    std::time_t future_time_t = std::chrono::system_clock::to_time_t(calculateTime);
    std::tm *time = std::localtime(&future_time_t);
    ss << std::put_time(time, "%H:%M");
    return ss.str();
}

void printPath(std::vector<pathStruct> path) {
    std::string currLine = path[1].line;
    std::cout << "\nFrom: " << path.begin()->station->get_name() << std::endl;
    std::cout << "| " << std::endl;
    std::cout << "To: " << path.back().station->get_name() << std::endl;
    std::cout << std::endl;

    std::cout << "|" << path[1].line << std::endl;
    for (auto it = path.begin(); it != path.end(); ++it) {
        if (it->line != currLine && it != path.begin())
            std::cout << "|" << it->line << std::endl;

        std::cout << "  " << std::left << std::setw(30) << it->station->get_name();
        if (it == path.begin())
            std::cout << std::right << std::setw(10) << getCurrentTime() << " (Now)";
        else if (std::next(it) == path.end() || (it->line != currLine && it != path.begin())) {
            std::cout << std::right << std::setw(10) << getCurrentTime(it->weight) << " (in " << it->weight << " min)";
            currLine = it->line;
        }
        std::cout << std::endl;
    }
}


int main() {
    Network *network = new Network("Stadt Wien");
    std::string fileGraph, start, end;
    setup(fileGraph, network, start, end);

    auto ExStart = std::chrono::high_resolution_clock::now();
    auto path = find_path(network, start, end);
    auto ExStop = std::chrono::high_resolution_clock::now();
    printPath(path);

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(ExStop - ExStart).count();
    std::cout << "\n\nDijsktra Execution time: " << duration << " ms" << std::endl;

    return 0;
}
