//
// Created by tigok on 20/03/2023.
//

#include "Tram.h"
#include "Station.h"
#include "Metro.h"
#include "../lib/DesignByContract.h"
#include "MetroExporter.h"

Metro * Tram::sim = NULL;

Tram::Tram(int lijnNr) {

    REQUIRE(lijnNr >= 0, "LijnNr moet een positieve getal zijn");

    this->lijnNr = lijnNr;
    this->beginStation = NULL;
    this->huidigeStation = NULL;

    ENSURE(getLijnNr() == lijnNr, "lijnNR is correct ingestelt");
    ENSURE(this->beginStation == NULL, "tram heeft geen beginstation");
    ENSURE(this->huidigeStation == NULL, "tram heeft geen huidigestation");

}

Tram::Tram() {

    this->lijnNr = -1;
    this->beginStation = NULL;
    this->huidigeStation = NULL;

    ENSURE(this->lijnNr == -1,"lijnNr heeft een standaard constante -1");
    ENSURE(this->beginStation == NULL, "tram heeft geen beginstation");
    ENSURE(this->huidigeStation == NULL, "tram heeft geen huidigestation");
}

void Tram::setLijnNr(int lijn) {

    REQUIRE(lijn >= 0,"lijn is de nummer van de tram");

    this->lijnNr = lijn;
}

int Tram::getLijnNr() {

    return this->lijnNr;

    ENSURE(getLijnNr() == lijnNr,"krijgt het nummer van de tram terug");
}

int Tram::getSnelheid() {

    if (this->type == "PCC") {
        return 40;
    }
    else {
        return 70;
    }

    ENSURE(this->type == "PCC","de snelheid van PCC is 40 km/h");
    ENSURE(this->type == "Stadslijner", "de snelheid van Stadslijner is 70 km/h");
    ENSURE(this->type == "Albatros", "de snelheid van Albatros is 70 km/h");
}

void Tram::setBeginstation(Station *beginpunt) {

    REQUIRE(beginpunt,"beginpunt is waar de vertrekpunt is van de tram");

    this->beginStation = beginpunt;

    ENSURE(this->beginStation = beginpunt,"dit zal ook de start zijn van de tram");
}

Station *Tram::getBeginstation() {

    return this->beginStation;

    ENSURE(getBeginstation() == beginStation,"krijgt de beginstation terug");
}

void Tram::setHuidigeStation(Station *startpunt) {

    REQUIRE(startpunt,"dit is de station waar de tram gelegen is");

    this->huidigeStation = startpunt;

    ENSURE(this->huidigeStation = startpunt,"de huidige station is de plek waar de tram nu gelegen is");
}

Station *Tram::getHuidigeStation() {

    return this->huidigeStation;

    ENSURE(this->huidigeStation = huidigeStation,"krijgt de huidige station terug");
}

string Tram::getType() {

    return this->type;

    ENSURE(this->type == type,"krijgt de type van de tram terug");
}

bool Tram::checkCompatible(Station *a) {

    REQUIRE(a,"a kan 2 dingen zijn ofwel een station ofwel een halte");


    if (this->getType() != "PCC" and a->getType() == "Halte") {
        return false;
    }
    else {
        return true;
    }

    ENSURE(a,"indien a een halte is dan kan alleen een PCC daar een stop maken");

}

Station *Tram::getNextValidStation() {

    Station * kandidaat = this->getHuidigeStation()->getVolgende();
    while (checkCompatible(kandidaat) == false) {
        kandidaat = kandidaat->getVolgende();
    }
    return kandidaat;

    ENSURE(this->getHuidigeStation() == kandidaat,"als de kandidaat een PCC is in een halte dan kan bv een albatros niet naar de volgende station gaan");
}

bool Tram::padIsVrij(Station *k) {

    REQUIRE(k,"k is een station dat moet vermeld worden of de station vrij is");

    Station * huidige = this->getHuidigeStation()->getVolgende();
    while (huidige != k->getVolgende()) {
        if (huidige->getHuidigeTram() != NULL) {
            return false;
        }
        huidige = huidige->getVolgende();
    }
    return true;

    ENSURE(k,"indien k niet vrij is omdat PCC voor een albatros staat omdat de PCC in een halte is moet die wachten");
}

bool Tram::move() {

    Station * kandidaat = getNextValidStation();
    bool padVrij = padIsVrij(kandidaat);
    if (padVrij) {
        MetroExporter::tramMoved(this,kandidaat);
        setHuidigeStation(kandidaat);
        return true;
    }
    else {
        return false;
    }

    ENSURE(padVrij == true,"de tram kan bewegen naar de volgende station");
    ENSURE(padVrij == false,"de tram kan niet bewegen naar de volgende station");
}

void Tram::reset() {

    huidigeStation = beginStation;

    ENSURE(huidigeStation == beginStation,"het zorgt ervoor dat de beginstation gereset wordt als de huidigestation");
}

void PCC::setMaxDefecten(int d) {

    REQUIRE(d >= 0,"dit moet een positieve getal zijn mag zelf ook 0 zijn");

    maxDefecten = d;

    ENSURE(maxDefecten == d,"het maximaal aantal keren dat de tram in panne mag vallen");
}

void PCC::setMaxReparatieDuur(int r) {

    REQUIRE(r >= 0,"dit moet een positieve getal zijn mag zelf ook 0 zijn");

    maxReparatieDuur = r;

    ENSURE(maxReparatieDuur == r,"het maximaal aantal  stappen van de simulatie de tram moet wachten");
}

bool PCC::move() {
    if (reparatieDuur > 0) {
        reparatieDuur--;
        return false;
    }

    Station * kandidaat = getNextValidStation();
    bool padVrij = padIsVrij(kandidaat);
    if (padVrij) {
        MetroExporter::tramMoved(this,kandidaat);
        setHuidigeStation(kandidaat);
        aantalDefecten++;

        if (aantalDefecten == maxDefecten) {
            aantalDefecten = 0;
            reparatieDuur = maxReparatieDuur;
        }

        return true;
    }
    else {
        return false;
    };

    ENSURE(PCC::move() == true, "de tram mag zich voortbewegen omdat de situatie onder controle is");
    ENSURE(PCC::move() == false,"de tram mag niet verder bewegen want aangezien de situatie staat er een tram voor zich dat moet gerepareed worden");
}

PCC::PCC(int lijnNr): Tram(lijnNr) {

    REQUIRE(lijnNr >= 0,"de lijnNr mag geen negatief getal zijn");

    aantalDefecten = 0;
    reparatieDuur = 0;
    type = "PCC";

    ENSURE(type == "PCC","door de gegeven lijnNR kunnen we al weten of wat de type van de tram is en vandaaruit de defecten en reparaties zien en de PCC tram is de enige die nood heeft daaraan.");

}