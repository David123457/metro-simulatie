#include "../lib/DesignByContract.h"
#include "MetroImporter.h"
#include "../lib/tinyxml.h"

using namespace std;


//Auxiliary function for internal use only

//const std::string fetch_text(TiXmlNode *pElement, std::ostream& errStream) {
//	if (pElement == NULL) return "";
//
//	TiXmlNode *elemNode = pElement->FirstChild();
//	if (elemNode == NULL) return "";
//	TiXmlText* text = elemNode->ToText();
//	if(text == NULL) return "";
//	return text->Value();
//}

//Below is a static member function but one cannot repeat the static keyword in the implementation

SuccessEnum MetroImporter::importMetro(
        const char * inputfilename, std::ostream& errStream, Metro& simulatie) {

    Tram::sim = & simulatie;
    Station::sim = & simulatie;

	TiXmlDocument doc;
	SuccessEnum endResult = Success;

	REQUIRE(simulatie.properlyInitialized(), "Metro simulatie wasn't initialized");
    bool fileLoaded = doc.LoadFile (inputfilename);
    REQUIRE(fileLoaded,"00:file failed to load");

    TiXmlElement * root = doc.RootElement();
    REQUIRE(root->NextSiblingElement() == NULL,"01:more root elements");

    TiXmlElement * child = root->FirstChildElement();

    while (child != NULL) {                                                   // hier leggen we uit hoe de XLM-file eruit ziet.
        string Elementnaam = child->ValueStr();                                  // Wat er allemaal op de XLM-geschreven staat leggen we stap per stap uit.
        REQUIRE(Elementnaam == "TRAM" or Elementnaam == "STATION","02:Element not Tram of Station");
        if (Elementnaam == "STATION") {                                          // Als er van boven Metro staat noemen we dat de root van de file.
                                                                                 // Daarna komt het volgende element.
            string naam = child->FirstChildElement("naam")->GetText();     // Dat element noemt men de Child.
            const char *spoor = child->FirstChildElement("spoorNr")->GetText(); // Onder de Child komen dan de variabelen die we moeten benoemen.
            string type = child->FirstChildElement("type")->GetText();
            int spoornummer = atoi(spoor);                                   // Hiervoor gaan we ze eerst alles benoemen tot wat ze behoren.
            Station * s = simulatie.findStation(naam);                           // We zeggen dan bijvoorbeeld dat na de Naam gedeelte zijn argumeng komt.
            if (s == NULL) {                                                  // en zo voor al de variabelen.

                if (type == "Halte") {
                    s = new Halte(naam,spoornummer);
                }
                else if (type == "Metrostation") {
                    s = new Metrostation(naam,spoornummer);
                }
                else {
                    s = new Station(naam,spoornummer);
                }
                simulatie.addStation(s);
            }
            string volgende = child->FirstChildElement("volgende")->GetText();
            string vorige = child->FirstChildElement("vorige")->GetText();
            Station * vol = simulatie.findStation(volgende);
            if (vol == NULL) {
                vol = new Station(volgende, spoornummer);
                simulatie.addStation(vol);
            }
            Station * vor = simulatie.findStation(vorige);
            if (vor == NULL) {
                vor = new Station(vorige,spoornummer);
                simulatie.addStation(vor);
            }
            s->setVolgende(vol);
            s->setVorige(vor);
        }
        if (Elementnaam == "TRAM") {
            const char *lijnNr = child->FirstChildElement("lijnNr")->GetText();                  // Hetzelfde doen we voor de child element Tram
            string type = child->FirstChildElement("type")->GetText();              // Dan benoemen we al de variabelen voor de Child Tram.
            int lijn = atoi(lijnNr);                                                          // we benoemen ze eerst
                                                                                            // dan voegen we de bijbehoorende argument toe
            string beginStation = child->FirstChildElement("beginStation")->GetText();
            Station * t = simulatie.findStation(beginStation);
            if (t == NULL) {
                t = new Station(beginStation, lijn);
                simulatie.addStation(t);
            }

            TiXmlElement * defectElement = child->FirstChildElement("aantalDefecten");
            TiXmlElement * reparatieElement = child->FirstChildElement("reparatieTijd");

            Tram * tram;

            if (type == "PCC") {
                PCC * pcc = new PCC(lijn);
                int defecten = atoi(defectElement->GetText());
                int reparatie = atoi(reparatieElement->GetText());
                pcc->setMaxDefecten(defecten);
                pcc->setMaxReparatieDuur(reparatie);
                tram = pcc;
            }
            else if (type == "Albatros") {
                tram = new Albatros(lijn);
            }
            else if (type == "Stadslijner") {
                tram = new Stadlijner(lijn);
            }
            else {
                tram = new Tram(lijn);
            }

            tram->setBeginstation(t);
            tram->setHuidigeStation(t);
            simulatie.addTram(tram);
        }

        child = child->NextSiblingElement();
    }

    doc.Clear();

	return endResult;
}