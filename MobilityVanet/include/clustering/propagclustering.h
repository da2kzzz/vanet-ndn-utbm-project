/**
 * \file	PropagClustering.h
 */

#ifndef PROPAGCLUSTERING_H
#define PROPAGCLUSTERING_H

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


class PropagClustering : public ClusteringEnvironment {

    public:
        //*/ -------------------------------------------------
        PropagClustering();
        virtual ~PropagClustering();

        //*/ -------------------------------------------------
        void startClustering(Fleet* fleet, AntennaMap*  amap, time_t t) ;
        void setAntennagridParams(double xmin, double ymin, int nbmailles, int pdm);
        void clear(bool hard=false);

        std::unordered_map<GeographicalObject*, std::unordered_set<GeographicalObject*>> connections;

    private:
        //*/ -------------------------------------------------
        std::unordered_set<GeographicalObject*> notVisited;
        std::unordered_set<GeographicalObject*> visited;
        int OGR2AntennaMesh(OGRPoint *ob);
        void expandCluster(Fleet* fleet, AntennaMap* amap, time_t t, GeologicCluster* currentCluster, std::unordered_set<GeographicalObject*>& neighbors);

        double XminA = 933225;
        double YminA = 2289875;
        int nbmaillesXA = 582;
        int pdmA = 25;



};

#endif // PROPAGCLUSTERING_H
