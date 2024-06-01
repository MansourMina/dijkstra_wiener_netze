#include <iostream>
#include "Station.h"
#include "Line.h"
#include <fstream>
#include "Network.h"
#include <sstream>
#include <string>
#include <algorithm>


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
        int weightToConnect= 0;
        while (!lineDone) {
            if (newLine) // Lese bis zum ersten Anführungszeichen
                std::getline(ss, station, '"');
            if (!std::getline(ss, station, '"')) lineDone = true; // Lese bis zum zweiten Anführungszeichen
            if (!std::getline(ss, weight, '"')) lineDone = true;
            if (toConnect) {
                Station* stationToConnect = network->get_station(station);
                if (!stationToConnect) {
                    stationToConnect = new Station(station);
                }
                newStation->add_connection(stationToConnect, weightToConnect, networkLine->get_name());
                stationToConnect->add_connection(newStation, weightToConnect,networkLine->get_name());
                networkLine->add_station(newStation);
                if(!lineDone) {
                    newStation = stationToConnect;
                    weightToConnect = std::stoi(weight);
                } else networkLine->add_station(stationToConnect);
            } else {
                newStation = new Station(station);
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

void find_path(std::string fileGraph, std::string start, std::string end) {
}

int main() {
    Network *network = new Network("Wiener Netze");
    std::string fileGraph = "C:\\Users\\manso\\Downloads\\ADS_Programmieraufgabe3_WienerVerkehrsNetz.txt";
    load_data(fileGraph,network);
    Station* station = network->get_station("Lainz, Wolkersbergenstrasse");
    if(station) {
        for(auto [stationName, connection]: station->get_connections()) {
            std::cout << "To: " << stationName->get_name() << " Fahrzeit: " << connection.weight << " Line: " << connection.line << std::endl;
        }
    }

    //std::string fileGraph, start, end;
    //setup(fileGraph, start, end);

    //find_path(fileGraph, start, end);
    return 0;
}
