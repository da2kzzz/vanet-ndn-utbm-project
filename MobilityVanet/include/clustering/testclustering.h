

#ifndef TESTCLUSTERING_H
#define TESTCLUSTERING_H


#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <unordered_set>
#include <unordered_map>
#include "geologic-core/algorithms/clustering/ClusteringEnvironment.h"
#include "geologic-core/algorithms/clustering/GeologicCluster.h"
#include "geologic-core/objects/geo/untemporal/Grid.h"
#include <geologic-core/utils/customToolbox.h>
#include "Common/color.h"
#include <unordered_set>
#include "misc/fleet.h"
#include "misc/antennamap.h"




using namespace customToolbox;


class testClustering: public ClusteringEnvironment {

public:
    //*/ -------------------------------------------------
    testClustering();
    virtual ~testClustering();

    //*/ -------------------------------------------------
    void startDBscan(Fleet* fleet, AntennaMap*  amap, time_t t);
    void setAntennagridParams(double xmin, double ymin, int nbmailles, int pdm);
    void clear(bool hard=false);

    //sstd::unordered_map<Vehicule*, std::unordered_set<Vehicule*>> connections;

private:
    //*/ -------------------------------------------------



    void calculateParameter(Vehicule* currentObject, time_t t, std::unordered_set<Vehicule*> neighbors,  Fleet* fleet);

    std::unordered_set<Vehicule*> getNeighbors(Fleet* fleet, AntennaMap* amap, time_t t, Vehicule* currentVehicule);

    Vehicule* getHead(Vehicule* currentObject, std::unordered_set<Vehicule*> neighbors, Fleet* fleet);


    void findRealConnection();

    int OGR2AntennaMesh(OGRPoint *ob);


    double XminA = 933225;
    double YminA = 2289875;
    int nbmaillesXA = 582;
    int pdmA = 25;
};




#endif // TESTCLUSTERING_H
