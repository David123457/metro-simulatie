//
// Created by tigok on 20/03/2023.
//

#ifndef METRO_SIMULATIE_TRAM_H
#define METRO_SIMULATIE_TRAM_H
#include <iostream>
#include <vector>
#include <string>

class Station;
class Metro;

using namespace std;


class Tram {                                                                // hier geef ik alle variabelen weer voor Tram
private:
    int lijnNr;
    Station * beginStation;
    Station * huidigeStation;
    string type;
    int maxDefecten = -1;
    int aantalDefecten = 0;
    int maxReparatieDuur = 0;
    int reparatieDuur = 0;
    Station * getNextValidStation();
    bool padIsVrij(Station *k);
public:

    static Metro * sim;

    Tram(int lijnNr, int aantalDefecten = -1, int reparatieTijd = 0);                                         // ectra functies die ik ga gebruiken voor de variabelen.
    Tram();
    void setLijnNr(int lijn);
    int getLijnNr();
    int getSnelheid();
    void setBeginstation(Station * beginpunt);
    Station * getBeginstation();
    void setHuidigeStation(Station * startpunt);
    Station * getHuidigeStation();
    void setType(string type);
    string getType();
    bool checkCompatible(Station * a);
    bool move();
    void reset();
    void setMaxDefecten(int d);
    void setMaxReparatieDuur(int r);
};


#endif //METRO_SIMULATIE_TRAM_H
