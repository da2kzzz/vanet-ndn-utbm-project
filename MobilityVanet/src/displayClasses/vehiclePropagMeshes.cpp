/**
 * \file	VehiclePropagMeshes.cpp
 * \class	VehiclePropagMeshes
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	VehiclePropagMeshes is a class representing a VehiclePropagMeshes
 */

#include <displayClasses/vehiclePropagMeshes.h>
#include "vanet.h"

using namespace std;


/**
 * \brief 	Default constructor of class Vehicule
 *
 * \param 	None
 * \return 	None
 */
VehiclePropagMeshes::VehiclePropagMeshes(Vanet* parent) : Data("Veh. propag. mesh.") {
    this->parent= parent;
}

/**
 * \brief 	Default destructor of class Vehicule
 *
 * \param 	None
 * \return 	None
 */
VehiclePropagMeshes::~VehiclePropagMeshes() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void VehiclePropagMeshes::draw() {

}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	t   a time_t
 * \return 	None
 * Display a vehicle as a disc at the current position
 */
void VehiclePropagMeshes::draw(unsigned long long t) {

    unordered_map<string, Vehicule*>::iterator it = this->parent->fleet->fleet.begin();
    for (; it!=this->parent->fleet->fleet.end(); it++) {
        Vehicule* currentVehicle = it->second;
        time_t lastT = (currentVehicle->listTrajectories["0"])->trajectory->back()->timestamp;

        if (t < lastT) {
            int meshPos = parent->OGR2AntennaMesh(currentVehicle);
            if (parent->antMapper->AMap->find(meshPos) != parent->antMapper->AMap->end()) {
                Antenna* ant = parent->antMapper->AMap->at(meshPos);
                ant->color = currentVehicle->color;
                ant->draw(t);
            }
        }
    }
}

void VehiclePropagMeshes::drawGridMesh(Fleet* fleet, GridMesh* mesh) {
    // Check if mesh is at lowest level
    if (mesh->subMeshes.size() > 0) {
        for (unsigned int i=0; i<mesh->subMeshes.size(); i++) this->drawGridMesh(fleet, mesh->subMeshes[i]);
    } else {
        glColor4d(fleet->colorsMesh[mesh].red, fleet->colorsMesh[mesh].green, fleet->colorsMesh[mesh].blue, 0.2);
        glBegin(GL_QUADS);
        for (unsigned int i=0; i<mesh->getNumPoints(); i++) {
            OGRPoint* pt = mesh->getPoint(i).get();
            glVertex3d(pt->getX(), pt->getY(), pt->getZ());
        }
        glEnd();
    }
}
