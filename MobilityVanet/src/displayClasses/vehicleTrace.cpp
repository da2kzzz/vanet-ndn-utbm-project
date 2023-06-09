/**
 * \file	VehicleTrace.cpp
 * \class	VehicleTrace
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	VehicleTrace is a class representing a VehicleTrace
 */

#include <displayClasses/vehicleTrace.h>
#include "vanet.h"

using namespace std;


/**
 * \brief 	Default constructor of class Vehicule
 *
 * \param 	None
 * \return 	None
 */
VehicleTrace::VehicleTrace(Vanet* parent) : Data("Vehicle traces") {
    this->parent = parent;
}

/**
 * \brief 	Default destructor of class Vehicule
 *
 * \param 	None
 * \return 	None
 */
VehicleTrace::~VehicleTrace() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void VehicleTrace::draw() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	t   a time_t
 * \return 	None
 * Display a vehicle as a disc at the current position
 */
void VehicleTrace::draw(unsigned long long t) {

    unordered_map<string, Vehicule*>::iterator it = this->parent->fleet->fleet.begin();
    for (; it!=this->parent->fleet->fleet.end(); it++) {
        Vehicule* currentVehicle = it->second;
        Trajectory* currentTraj = currentVehicle->listTrajectories["0"]; // this is the trajectory actually followed by one vehicle


        //OGRSpatialReference* lambe = CoordinateSystemManager::getInstance()->getCSbyName("Lambert II etendu");
        //OGRSpatialReference* pm = CoordinateSystemManager::getInstance()->getCSbyName("WGS 84 Pseudo Mercator");
        //OGRSpatialReference* wgs84 = CoordinateSystemManager::getInstance()->getCSbyName("WGS 84");
        OGRSpatialReference* displayRef = CoordinateSystemManager::getInstance()->getDisplayRef();

        std::unique_ptr<Trajectory> tempTraj = make_unique<Trajectory>(); // We will display only a small part of the whole trajectory using this tempTraj
        std::unique_ptr<OGRPoint> temp = currentTraj->display(t); // This is where the vehicle precisely is, on the real trajectory, at this timestamp
        unsigned int until = 30;


        // If the vehicle is somewhere on the real trajectory, we can compute its position, and the last X seconds of is displacement
        if (temp.get() != nullptr && t < currentTraj->trajectory->back()->timestamp) {

            // If the vehicle is somewhere on the real trajectory, we can compute its position, and the last X seconds of is displacement
            // So we must create the trajectory that will be displayed
            // Get the last X seconds of the real trajectory
            std::unique_ptr<TemporalGeoPoint> tgp = currentTraj->getNewPositionAt(t-until); // Position at -X seconds
            if (tgp.get() == nullptr) tgp = currentTraj->getClosestPoint(t-until); // Get a position at any cost
            tempTraj->addPosition(tgp.get()); // It's the first point that will be displayed

            // Now, for the rest of the points to display
            for (unsigned int i=0; i<currentTraj->trajectory->size(); i++) {
                time_t minTime = 0;
                if (t >= until) minTime = t-until; // We get the -X seconds

                // If a point is between the actual position and this -X seconds point, then we add it
                if (currentTraj->trajectory->at(i)->timestamp > minTime && currentTraj->trajectory->at(i)->timestamp < t) {
                    // If its the first point we are adding, then we add it
                    if (tempTraj->trajectory->size() < 1) {
                        tempTraj->addPosition(currentTraj->trajectory->at(i));
                    // Otherwise, if it's not the first point, we check that this point is actually at least 2 meters away from the previous
                    // point of the display trajectory
                    } else if (geoMetrics::distanceTo(tempTraj->trajectory->back(), currentTraj->trajectory->at(i), displayRef) > 2) {
                        tempTraj->addPosition(currentTraj->trajectory->at(i));
                    }
                }
            }

            // And we don't forget to add the current position of the vehicle !
            std::unique_ptr<TemporalGeoPoint> tgp1 = make_unique<TemporalGeoPoint>(temp.get(), t);
            if (tempTraj->trajectory->size() < 1) tempTraj->addPosition(tgp1.get());
            else if (geoMetrics::distanceTo(tempTraj->trajectory->back(), tgp1.get(), displayRef) > 2) tempTraj->addPosition(tgp1.get());
        }


        if (tempTraj->trajectory->size() > 1) {
            // Here, no matter what happened, if we are after of during the real trajectory, we convert the points to display
            // (either the last point seen, either the whole tempTraj to display

            /*/
            Bspline bs = Bspline(10);
            bs.setDataRef(lambe);
            tempTraj = bs.computeBsplineTrajectory(tempTraj.get(), 3);
            //*/

            //for (unsigned int i=0; i<tempTraj->trajectory->size(); i++) geoMetrics::convertTo(tempTraj->trajectory->at(i), lambe, display);
            // If we have more that one point to display, we graphically smoothen the line

            // We divide the tempTraj into 4 slices
            // One slice of 3pt, one of 4pt, one of 5pt, and the last one (the last -(X/2) seconds) in 6pt
            time_t totalT = tempTraj->trajectory->back()->timestamp - tempTraj->trajectory->front()->timestamp;
            time_t firstSlice = tempTraj->trajectory->front()->timestamp + totalT/6.0;
            time_t secondSlice = firstSlice + totalT/6.0;
            time_t thirdSlice = secondSlice + totalT/6.0;
            // To assert a nice splitting of the positions
            tempTraj->insertPosition(tempTraj->getNewPositionAt(firstSlice).get());
            tempTraj->insertPosition(tempTraj->getNewPositionAt(secondSlice).get());
            tempTraj->insertPosition(tempTraj->getNewPositionAt(thirdSlice).get());

            // We draw all the points according to their slice
            bool drawing = false;
            TemporalGeoPoint* lastPt = tempTraj->trajectory->front();
            for (unsigned int i=0; i<tempTraj->trajectory->size(); i++) {
                TemporalGeoPoint* currentPt = tempTraj->trajectory->at(i);

                if (currentPt->timestamp <= firstSlice) {
                    if (!drawing) {
                        drawing = true;
                        glLineWidth(3); // starts the drawing with pt3
                        glColor4d(currentVehicle->color.red, currentVehicle->color.green, currentVehicle->color.blue, 0.2); //02
                        glBegin(GL_LINE_STRIP);
                    }
                } else if (currentPt->timestamp <= secondSlice) {
                    if (drawing) {
                        glEnd(); // ends the drawing with pt3, and starts a new one with pt4
                        drawing = false;
                        glLineWidth(4);
                        glColor4d(currentVehicle->color.red, currentVehicle->color.green, currentVehicle->color.blue, 0.4); //04
                        glBegin(GL_LINE_STRIP);
                        glVertex3d(lastPt->getX(), lastPt->getY(), 0.0); // adds again the last point, only for assert there is no gap between 2 line strips
                    }
                } else if (currentPt->timestamp <= thirdSlice) {
                    if (!drawing) {
                        glEnd(); // ends the drawing with pt4, and starts a new one with pt5
                        drawing = true;
                        glLineWidth(5);
                        glColor4d(currentVehicle->color.red, currentVehicle->color.green, currentVehicle->color.blue, 0.6); //06
                        glBegin(GL_LINE_STRIP);
                        glVertex3d(lastPt->getX(), lastPt->getY(), 0.0);
                    }
                } else {
                    if (drawing) {
                        glEnd();
                        drawing = false;
                        glLineWidth(6);
                        glColor4d(currentVehicle->color.red, currentVehicle->color.green, currentVehicle->color.blue, 0.9); //09
                        glBegin(GL_LINE_STRIP);
                        glVertex3d(lastPt->getX(), lastPt->getY(), 0.0);
                    }
                }
                glVertex3d(currentPt->getX(), currentPt->getY(), 0.0); // Adds the current point to the line strip
                lastPt = currentPt;
            }
            glEnd();
        }
    }
}
