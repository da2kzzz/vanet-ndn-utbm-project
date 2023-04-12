/**
 * \file	Fleet.cpp
 * \class	Fleet
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	Fleet is a set of different vehicle.
 * Fleet is actually a receptacle of all trajectories followed by every vehicle of the environement
 * This can be modified by saying that fleet is a map of vehicle, and where each vehicle has its own draw method
 */

#include "misc/fleet.h"
#include "vanet.h"

using namespace std;

/**
 * \brief 	Default constructor of class Fleet
 *
 * \param 	None
 * \return 	None
 */
Fleet::Fleet(Vanet* parent) : Data("fleet") {
    this->parent = parent;
    vehicleGrid = std::unique_ptr<Grid>(new Grid(OGRPoint(parent->params.Xmin, parent->params.Ymin, 0), 20000)); //(933225, 2289875, 0)
    vehicleGrid->meshEnv(5);
    //this->setColors(vehicleGrid->firstMesh.get());
    currentT = 0;
}

void Fleet::setColors(GridMesh* mesh) {
    if (mesh->subMeshes.size() > 0) {
        for (unsigned int i=0; i<mesh->subMeshes.size(); i++) this->setColors(mesh->subMeshes[i]);
    } else {
        Color c = Color((rand()%100)/100.0, (rand()%100)/100.0, (rand()%100)/100.0, 0.6);
        this->colorsMesh.insert(pair<GridMesh*, Color>(mesh, c));
    }
}

/**
 * \brief 	Default destructor of class Fleet
 *
 * \param 	None
 * \return 	None
 */
Fleet::~Fleet() {}

/**
 * \brief 	Default destructor of class Fleet
 *
 * \param 	None
 * \return 	None
 */
void Fleet::sendAlert(AlerteInfo alerte, time_t t, OGRPoint* location) {

    if (alerte.peremption < t) return;

    bool isNew = true;
    vector<pair<AlerteInfo, Color>>::iterator it = colorsInfo.begin();
    for (; it!=colorsInfo.end(); it++) {
        if ((it->first).isEqual(alerte)) {
            isNew = false;
            break;
        }
    }
    if (isNew) colorsInfo.push_back(pair<AlerteInfo, Color>(alerte, Color((rand()%100)/100.0, (rand()%100)/100.0, (rand()%100)/100.0, 0.6)));

    OGRSpatialReference* displayRef = CoordinateSystemManager::getInstance()->getDisplayRef();
    vector<GeographicalObject*> vehiclesNeighbors;
    GridMesh* vm = vehicleGrid->getMeshFromPoint(location);
    if (vm != nullptr) vehiclesNeighbors = vm->getNeighborObjects(true);
    for (unsigned int i=0; i<vehiclesNeighbors.size(); i++) {
        Vehicule* currentVehicle = (Vehicule*) vehiclesNeighbors[i];
        if (currentVehicle->isActive(t) && geoMetrics::distanceTo(location, currentVehicle, displayRef) < parent->params.vehicleRadius) {
            currentVehicle->alerteInfo(alerte, t);
        }
    }
}

/**
 * \brief 	Default destructor of class Fleet
 *
 * \param 	None
 * \return 	None
 */
void Fleet::updateAlerts(time_t t) {
    if (currentT != t) {
        currentT = t;
        OGRSpatialReference* displayRef = CoordinateSystemManager::getInstance()->getDisplayRef();
        unordered_map<string, Vehicule*>::iterator it = this->fleet.begin();
        for (; it!=this->fleet.end(); it++) {
            Vehicule* currentVehicle = it->second;

            if (currentVehicle->setInfo.size() > 0) {
                    vector<GeographicalObject*> vehiclesNeighbors;
                    GridMesh* vm = vehicleGrid->getMesh(currentVehicle);
                    if (vm != nullptr) vehiclesNeighbors = vm->getNeighborObjects();

                    for (unsigned int i=0; i<vehiclesNeighbors.size(); i++) {
                        Vehicule* neig = (Vehicule*) vehiclesNeighbors[i];
                        if (neig->isActive(t) && geoMetrics::distanceTo(neig, currentVehicle, displayRef) < parent->params.vehicleRadius) {
                            vector<pair<AlerteInfo, time_t>>::iterator infoIt = currentVehicle->setInfo.begin();
                            for (; infoIt!=currentVehicle->setInfo.end(); infoIt++) {
                                if ((infoIt->second) + currentVehicle->transmissionDuration >= t)
                                    neig->alerteInfo(infoIt->first, t);
                            }
                        }
                    }
            }
        }
    }

    unordered_map<string, Vehicule*>::iterator it = this->fleet.begin();
    for (; it!=this->fleet.end(); it++) {
        Vehicule* currentVehicle = it->second;

        if (currentVehicle->setInfo.size() > 0) {
            currentVehicle->mergedColors = 0;
            vector<pair<AlerteInfo, time_t>>::iterator itt = currentVehicle->setInfo.begin();
            while (itt!=currentVehicle->setInfo.end()) {
                if ((itt->second + currentVehicle->transmissionDuration) >= t) {
                    currentVehicle->mergeColors(getColorsInfo(itt->first));
                }
                if ((itt->first).peremption < t || (itt->first).emission_time > t || (itt->second + currentVehicle->tabouDuration) < t) {
                    itt = currentVehicle->setInfo.erase(itt);
                } else {
                    itt++;
                }
            }
        }
    }

    vector<pair<AlerteInfo, Color>>::iterator itC = colorsInfo.begin();
    while (itC!=colorsInfo.end()) {
        if ((itC->first).peremption < t || (itC->first).emission_time > t) {
            itC = colorsInfo.erase(itC);
        } else {
            itC++;
        }
    }
}

/**
 * \brief 	Default destructor of class Fleet
 *
 * \param 	None
 * \return 	None
 */
Color Fleet::getColorsInfo(AlerteInfo alerte) {
    for (unsigned int i=0; i<colorsInfo.size(); i++) if (colorsInfo[i].first.isEqual(alerte)) return colorsInfo[i].second;
    return Color(1, 0, 0, 0.6);
}

void Fleet::draw() {}
void Fleet::draw(unsigned long long t) {

//   // population.clear();
//    //cout << ">>>>>>>  vanet draw  <<<<<<<" << endl;
//    unordered_map<string, Vehicule*>::iterator it = this->fleet.begin();
//    for (; it!=this->fleet.end(); it++) {
//        Vehicule* currentVehicle = it->second;
//        //currentVehicle->color = params.defaultColor;
//        currentVehicle->hasCluster = false;
//        currentVehicle->radius = 80;

//        std::unique_ptr<OGRPoint> temp = currentVehicle->listTrajectories["0"]->display(t);
//        if (temp != nullptr) {
//            currentVehicle->setX(temp->getX());
//            currentVehicle->setY(temp->getY());
//            //population.push_back(currentVehicle);
//            this->vehicleGrid->update(currentVehicle, currentVehicle, t);
//        }
//    }
}
