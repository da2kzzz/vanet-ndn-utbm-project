/**
 * \file	VehiclePropagCircle.cpp
 * \class	VehiclePropagCircle
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	VehiclePropagCircle is a class representing a VehiclePropagCircle
 */

#include <displayClasses/vehiclePropagCircle.h>
#include "vanet.h"

using namespace std;


/**
 * \brief 	Default constructor of class Vehicule
 *
 * \param 	None
 * \return 	None
 */
VehiclePropagCircle::VehiclePropagCircle(Vanet* parent) : Data("Veh. propag. circ.") {
    this->parent = parent;
}

/**
 * \brief 	Default destructor of class Vehicule
 *
 * \param 	None
 * \return 	None
 */
VehiclePropagCircle::~VehiclePropagCircle() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void VehiclePropagCircle::draw() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	t   a time_t
 * \return 	None
 * Display a vehicle as a disc at the current position
 */
void VehiclePropagCircle::draw(unsigned long long t) {

    unordered_map<string, Vehicule*>::iterator it = this->parent->fleet->fleet.begin();
    for (; it!=this->parent->fleet->fleet.end(); it++) {
        Vehicule* currentVehicle = it->second;
        time_t lastT = (currentVehicle->listTrajectories["0"])->trajectory->back()->timestamp;

        if (t < lastT) {
            glLineWidth(3);
            glColor4d(currentVehicle->color.red, currentVehicle->color.green, currentVehicle->color.blue, 0.3);

            if (!currentVehicle->hasCluster)
                glColor4d(currentVehicle->color.red, currentVehicle->color.green, currentVehicle->color.blue, 0.8);

            double radius = currentVehicle->radius;
            float num_segments = 20.0 * sqrtf(radius);
            float theta = 2*M_PI / num_segments;
            float co = cosf(theta);//precalculate the sine and cosine
            float si = sinf(theta);
            float t;

            float x = radius;//we start at angle = 0
            float y = 0;

            glBegin(GL_LINE_LOOP);
            for(int ii=0; ii<num_segments; ii++)  {
                glVertex2f(x+currentVehicle->getX(), y+currentVehicle->getY());//output vertex
                //apply the rotation matrix
                t = x;
                x = co*x - si*y;
                y = si*t + co*y;
            }
            glEnd();
        }
    }
}
