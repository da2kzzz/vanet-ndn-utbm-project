/**
 * \file	Fleet.h
 */

#ifndef FLEET_H
#define FLEET_H

#include "geologic-core/objects/dynamic/MobileObject.h"
#include <geologic-core/algorithms/geoMetrics/geoMetrics.h>
#include "geologic-core/algorithms/trajectories/Bspline.h"
#include <geologic-core/core/geologic-launcher/CoordinateSystemManager.h>
#include "stdio.h"
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "antennamap.h"
#include "vehicule.h"
#include "geologic-core/objects/geo/untemporal/Grid.h"

class Vanet;

class Fleet : public Data {

    public:
        //*/ -------------------------------------------------
        Fleet(Vanet* parent);
        virtual ~Fleet();

        void draw();
        void draw(unsigned long long t);

        //*/ -------------------------------------------------
        void sendAlert(AlerteInfo alerte, time_t t, OGRPoint* location);
        void updateAlerts(time_t t);
        Color getColorsInfo(AlerteInfo alerte);

        //*/ -------------------------------------------------
        Vanet* parent;
        Bspline splines; // For special trajectory computations
        //AntennaMap* AMAP; // The list of all antennas of the environment
        std::unordered_map<std::string, Vehicule*> fleet;
        std::unique_ptr<Grid> vehicleGrid;

        time_t currentT;
        std::vector<std::pair<AlerteInfo, Color>> colorsInfo;

        std::unordered_map<GridMesh*, Color> colorsMesh;
        void setColors(GridMesh* mesh);
};

#endif // FLEET_H
