/**
 * \file	VehiclePosition.cpp
 * \class	VehiclePosition
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	VehiclePosition is a class representing a VehiclePosition
 */

#include <displayClasses/vehiclePosition.h>
#include "vanet.h"

using namespace std;


/**
 * \brief 	Default constructor of class Vehicule
 *
 * \param 	None
 * \return 	None
 */
VehiclePosition::VehiclePosition(Vanet* parent) : Data("Vehicle positions") {
    this->parent = parent;
}

/**
 * \brief 	Default destructor of class Vehicule
 *
 * \param 	None
 * \return 	None
 */
VehiclePosition::~VehiclePosition() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void VehiclePosition::draw() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	t   a time_t
 * \return 	None
 * Display a vehicle as a disc at the current position
 */
void VehiclePosition::draw(unsigned long long t) {

    unordered_map<string, Vehicule*>::iterator it = this->parent->fleet->fleet.begin();
    for (; it!=this->parent->fleet->fleet.end(); it++) {

        Vehicule* currentVehicle = it->second;
        time_t lastT = (currentVehicle->listTrajectories["0"])->trajectory->back()->timestamp;

        glColor4d(currentVehicle->color.red, currentVehicle->color.green, currentVehicle->color.blue, 0.9);
        if (t >= lastT) {
            //temp.reset(new OGRPoint(currentTraj->trajectory->back()->getX(), currentTraj->trajectory->back()->getY()));
            glColor4d(currentVehicle->color.red, currentVehicle->color.green, currentVehicle->color.blue, 0.5);
        }

        glPointSize(6.0);
        //if (!hasCluster) glColor4d(color.red, color.green, color.blue, 0.8);
        glBegin(GL_POINTS);
        glVertex3d(currentVehicle->getX(), currentVehicle->getY(), 0.0);
        glEnd();
    }
}
