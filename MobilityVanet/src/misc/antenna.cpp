/**
 * \file	Antenna.cpp
 * \class	Antenna
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	Descritpion of an antenna (one mesh + a coverage)
 * An antenna is a mesh on the road network, do not mix up with gridmesh which is just a toolbox class
 * The meshing of the antenna is fixed by the config file
 */

#include "misc/antenna.h"

using namespace std;


/**
 * \brief 	Default constructor of class Antenna
 *
 * \param 	None
 * \return 	None
 */
Antenna::Antenna() {
    this->pdm = 25;
}


/**
 * \brief 	Default destructor of class Antenna
 *
 * \param 	None
 * \return 	None
 */
Antenna::~Antenna() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	t   a time_t
 * \return 	None
 * Display an antenna by drawing a square (mesh) for each reachable mesh from this position (antenna position)
 */
void Antenna::draw(unsigned long long t) {
    //OGRSpatialReference* lambe = CoordinateSystemManager::getInstance()->getCSbyName("Lambert II etendu");
    //OGRSpatialReference* pm = CoordinateSystemManager::getInstance()->getCSbyName("WGS 84 Pseudo Mercator");
    //OGRSpatialReference* wgs84 = CoordinateSystemManager::getInstance()->getCSbyName("WGS 84");
    //OGRSpatialReference* display = lambe;
    for (unsigned int i=0; i<meshes.size(); i++) {
        OGRPoint* meshPos = meshes[i].first.get();
        //meshPos->assignSpatialReference(display);
        double rxlev = meshes[i].second; //-30 <-> -100
        double dbMin = -100.0;
        double dbMax = -50.0;
        double alpha = (rxlev-dbMin)/(dbMax-dbMin);

        glColor4d(color.red, color.green, color.blue, alpha);
        //Color c = Color(colors[it->first].red, colors[it->first].green, colors[it->first].blue, 0.4);

        glBegin(GL_QUADS);
            glVertex3d(meshPos->getX(), meshPos->getY(), 0.0);
            glVertex3d(meshPos->getX(), meshPos->getY()+pdm, 0.0);
            glVertex3d(meshPos->getX()+pdm, meshPos->getY()+pdm, 0.0);
            glVertex3d(meshPos->getX()+pdm, meshPos->getY(), 0.0);
        glEnd();
    }
}
