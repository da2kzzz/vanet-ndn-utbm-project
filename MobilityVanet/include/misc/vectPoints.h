/**
 * \file	VectPoints.h
 */

#ifndef VectPoints_h
#define VectPoints_h

#include <unordered_map>
#include <vector>
#include <string>
#include "Common/color.h"
#include "geologic-core/objects/geo/untemporal/GeographicalObject.h"
#include "data/data.h"
#include <geologic-core/core/geologic-launcher/CoordinateSystemManager.h>
#include <geologic-core/algorithms/geoMetrics/geoMetrics.h>
#include "stdio.h"
#include <iostream>
#include "ogr_geometry.h"
#include <ogr_spatialref.h>
#include <OpenGL/discdisplayer.h>
#include <OpenGL/multipointsdisplayer.h>
#include <OpenGL/linedisplayer.h>
#include <OpenGL/zonedisplayer.h>
#include <OpenGL/discdisplayer.h>
#include <OpenGL/histogramdisplayer.h>
#include <glrastertile.h>
#include <glvectortile.h>


class VectPoints : public Data {

    public:
        //*/ -------------------------------------------------
        VectPoints();
        virtual ~VectPoints();

        //*/ -------------------------------------------------
        void draw();
        void draw(unsigned long long t);

        std::unordered_map<std::string, Color> colors;
};

#endif // ANTENNAMAP_H
