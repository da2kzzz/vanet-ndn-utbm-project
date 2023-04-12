/**
 * \file	Antenna.h
 */

#ifndef ANTENNA_H
#define ANTENNA_H

#include <geologic-core/objects/geo/untemporal/GeoZone.h>
#include <data/data.h>
#include <vector>
#include "Common/color.h"
#include <geologic-core/core/geologic-launcher/CoordinateSystemManager.h>
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


class Antenna : public GeoZone {

    public:
        //*/ -------------------------------------------------
        Antenna();
        virtual ~Antenna();

        //*/ -------------------------------------------------
        void draw(unsigned long long t);

        //*/ -------------------------------------------------
        int centroidMesh; // Id of the mesh in the current referential
        int pdm; // the size of one mesh
        Color color;

        std::vector<std::pair<std::unique_ptr<OGRPoint>, double>> meshes; // The reachable meshes from this position, with the RxLev value
};

#endif // ANTENNA_H
