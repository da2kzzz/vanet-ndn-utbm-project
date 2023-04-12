/**
 * \file	AntennaMap.h
 */

#ifndef ANTENNAMAP_H
#define ANTENNAMAP_H

#include <unordered_map>
#include <string>
#include "antenna.h"
#include "data/data.h"


class AntennaMap : public Data {

    public:
        //*/ -------------------------------------------------
        AntennaMap();
        virtual ~AntennaMap();

        //*/ -------------------------------------------------
        void draw();
        void draw(unsigned long long t);

        //*/ -------------------------------------------------
        std::unordered_map<int, Antenna*>* AMap;
};

#endif // ANTENNAMAP_H
