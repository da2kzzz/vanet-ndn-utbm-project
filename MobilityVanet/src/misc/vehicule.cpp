/**
 * \file	Vehicule.cpp
 * \class	Vehicule
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	Vehicule is a class representing a vehicle
 */

#include <misc/vehicule.h>
#include "misc/fleet.h"
#include <geologic-core/utils/customToolbox.h>

using namespace std;


/**
 * \brief 	Default constructor of class Vehicule
 *
 * \param 	None
 * \return 	None
 */
Vehicule::Vehicule(Fleet* environment) {
    this->environment = environment;
    this->id = 0;
    this->name = to_string(this->id);
    this->statut = "";
    this->heureD = 0;
    this->indiceO = 0;
    this->indiceD = 0;
    this->nbPositions = 0;
    this->PosCourante = 0;
    this->hasCluster = false;
    this->radius = 80;
    //this->vPositions={0}; //tableau des positions initialisé à 0
}


/**
 * \brief 	Default destructor of class Vehicule
 *
 * \param 	None
 * \return 	None
 */
Vehicule::~Vehicule() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void Vehicule::draw() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	t   a time_t
 * \return 	None
 * Display a vehicle as a disc at the current position
 */
void Vehicule::draw(unsigned long long t) {

    Trajectory* currentTraj = this->listTrajectories["0"]; // this is the trajectory actually followed by one vehicle

    OGRSpatialReference* lambe = CoordinateSystemManager::getInstance()->getCSbyName("Lambert II etendu");
    //OGRSpatialReference* pm = CoordinateSystemManager::getInstance()->getCSbyName("WGS 84 Pseudo Mercator");
    //OGRSpatialReference* wgs84 = CoordinateSystemManager::getInstance()->getCSbyName("WGS 84");
    OGRSpatialReference* display = lambe;

    std::unique_ptr<Trajectory> tempTraj = make_unique<Trajectory>(); // We will display only a small part of the whole trajectory using this tempTraj
    std::unique_ptr<OGRPoint> temp = currentTraj->display(t); // This is where the vehicle precisely is, on the real trajectory, at this timestamp
    unsigned int until = 30;
    // If the position of the vehicle can be computed, or if we are after the vehicle has stopped
    if (temp.get() != nullptr || t >= currentTraj->trajectory->back()->timestamp) {

        // With anticipation we add the last point where the vehicle was see in the tempTraj
        tempTraj->addPosition(currentTraj->trajectory->back());

        // If the vehicle is somewhere on the real trajectory, we can compute its position, and the last X seconds of is displacement
        if (temp.get() != nullptr && t < currentTraj->trajectory->back()->timestamp) {

            // If the vehicle is somewhere on the real trajectory, we can compute its position, and the last X seconds of is displacement
            // So we must create the trajectory that will be displayed
            //*/---------------------------------------------------
            //geoMetrics::convertTo(temp, lambe, display);
            tempTraj->clear(true);

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
                    } else if (geoMetrics::distanceTo(tempTraj->trajectory->back(), currentTraj->trajectory->at(i), lambe) > 2) {
                        tempTraj->addPosition(currentTraj->trajectory->at(i));
                    }
                }
            }

            // And we don't forget to add the current position of the vehicle !
            std::unique_ptr<TemporalGeoPoint> tgp1 = make_unique<TemporalGeoPoint>(temp.get(), t);
            if (tempTraj->trajectory->size() < 1) tempTraj->addPosition(tgp1.get());
            else if (geoMetrics::distanceTo(tempTraj->trajectory->back(), tgp1.get(), lambe) > 2) tempTraj->addPosition(tgp1.get());
        }

        // Here, no matter what happened, if we are after of during the real trajectory, we convert the points to display
        // (either the last point seen, either the whole tempTraj to display
        for (unsigned int i=0; i<tempTraj->trajectory->size(); i++) geoMetrics::convertTo(tempTraj->trajectory->at(i), lambe, display);

        glColor4d(color.red, color.green, color.blue, color.alpha);
        // If we have more that one point to display, we graphically smoothen the line
        if (tempTraj->trajectory->size() > 1) {
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
                        glColor4d(color.red, color.green, color.blue, 0.2);
                        glBegin(GL_LINE_STRIP);
                    }
                } else if (currentPt->timestamp <= secondSlice) {
                    if (drawing) {
                        glEnd(); // ends the drawing with pt3, and starts a new one with pt4
                        drawing = false;
                        glLineWidth(4);
                        glColor4d(color.red, color.green, color.blue, 0.4);
                        glBegin(GL_LINE_STRIP);
                        glVertex3d(lastPt->getX(), lastPt->getY(), 0.0); // adds again the last point, only for assert there is no gap between 2 line strips
                    }
                } else if (currentPt->timestamp <= thirdSlice) {
                    if (!drawing) {
                        glEnd(); // ends the drawing with pt4, and starts a new one with pt5
                        drawing = true;
                        glLineWidth(5);
                        glColor4d(color.red, color.green, color.blue, 0.6);
                        glBegin(GL_LINE_STRIP);
                        glVertex3d(lastPt->getX(), lastPt->getY(), 0.0);
                    }
                } else {
                    if (drawing) {
                        glEnd();
                        drawing = false;
                        glLineWidth(6);
                        glColor4d(color.red, color.green, color.blue, 0.9);
                        glBegin(GL_LINE_STRIP);
                        glVertex3d(lastPt->getX(), lastPt->getY(), 0.0);
                    }
                }
                glVertex3d(currentPt->getX(), currentPt->getY(), 0.0); // Adds the current point to the line strip
                lastPt = currentPt;
            }
            glEnd();
        // else, if we only have one point to display, we just draw a disc at the current position (can be the last seen position)
        } else {
            //Color c = Color(color.red, color.green, color.blue, 0.4);
            //OGRPoint disc{tempTraj->trajectory->at(0)->getX(), tempTraj->trajectory->at(0)->getY(), 0.0};
            //disc.assignSpatialReference(display);
            //DiscDisplayer::drawDiscFull(c, 50.0, disc, display);
            glPointSize(5.0);
            glColor4d(color.red, color.green, color.blue, 0.5); //glColor4d(1.0, 1.0, 1.0, 1.0);
            //if (!hasCluster) glColor4d(color.red, color.green, color.blue, 0.8);
            glBegin(GL_POINTS);
            glVertex3d(tempTraj->trajectory->at(0)->getX(), tempTraj->trajectory->at(0)->getY(), 0.0);
            glEnd();


        }

        glLineWidth(3);
        glColor4d(color.red, color.green, color.blue, 0.3);
        if (!hasCluster) glColor4d(color.red, color.green, color.blue, 0.8);
        if (t < currentTraj->trajectory->back()->timestamp) {
            double radius = 25;
            float num_segments = 20.0*sqrtf(radius);
            float theta = 2 * 3.1415926 / num_segments;
            float co = cosf(theta);//precalculate the sine and cosine
            float si = sinf(theta);
            float t;

            float x = radius;//we start at angle = 0
            float y = 0;

            glBegin(GL_LINE_LOOP);
            for(int ii = 0; ii < num_segments; ii++)  {
                glVertex2f(x + tempTraj->trajectory->back()->getX(), y + tempTraj->trajectory->back()->getY());//output vertex
                //apply the rotation matrix
                t = x;
                x = co * x - si * y;
                y = si * t + co * y;
            }
            glEnd();
        }
    }
}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void Vehicule::alerteInfo(AlerteInfo alerte, time_t t) {
    if (alerte.peremption < t) return;
    bool isNew = true;
    vector<pair<AlerteInfo, time_t>>::iterator it = setInfo.begin();
    for (; it!=setInfo.end(); it++) {
        if ((it->first).isEqual(alerte)) {
            isNew = false;
            break;
        }
    }
    if (isNew) setInfo.push_back(pair<AlerteInfo, time_t>(alerte, t));
}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void Vehicule::mergeColors(Color color) {
    double red = ((this->color.red * this->mergedColors) + (color.red))/(this->mergedColors+1);
    double green = ((this->color.green * this->mergedColors) + (color.green))/(this->mergedColors+1);
    double blue = ((this->color.blue * this->mergedColors) + (color.blue))/(this->mergedColors+1);
    this->color = Color(red, green, blue, 0.6);
    this->mergedColors++;
}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
bool Vehicule::isActive(time_t t) {
    return (listTrajectories["0"]->trajectory->back()->timestamp > t);
}

/**
 * \brief 	Sets the graphical type. Inherited from GeologicGraphicalObject
 *
 * \param 	None
 * \return 	None
 */
void Vehicule::setType() {
    this->type = "Vehicle";
}

/**
 * \brief 	Sets the graphical type. Inherited from GeologicGraphicalObject
 *
 * \param 	None
 * \return 	None
 */
std::string Vehicule::getInfos() {
    std::string infos = "";
    infos += this->type + " at address: " + customToolbox::str(this);
    return infos;
}


//
void Vehicule::setVelocity(time_t t){

    // get the position of currentV in the time t and t+1
    std::unique_ptr<OGRPoint> currentPosition = this->listTrajectories["0"]->display(t);
    std::unique_ptr<OGRPoint> nextPosition = this->listTrajectories["0"]->display(t+1);

    if(currentPosition != nullptr && nextPosition != nullptr){

        this->velocity.x = nextPosition->getX() - currentPosition->getX();
        this->velocity.y = nextPosition->getY() - currentPosition->getY();
        this->velocity.z = 0;

    }else {
        this->velocity.x = 0;
        this->velocity.y = 0;
        this->velocity.z = 0;
    }

}

glm::vec3 Vehicule::getVelocity(time_t t) {
    this->setVelocity(t);

    return  this->velocity;
}
