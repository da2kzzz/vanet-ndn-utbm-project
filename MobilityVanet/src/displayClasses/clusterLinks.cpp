/**
 * \file	ClusterLinks.cpp
 * \class	ClusterLinks
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	ClusterLinks is a class representing a ClusterLinks
 */

#include <displayClasses/clusterLinks.h>
#include "vanet.h"

using namespace std;


/**
 * \brief 	Default constructor of class Vehicule
 *
 * \param 	None
 * \return 	None
 */
ClusterLinks::ClusterLinks(ClusterManagerLayer* parent) : Data("Cluster links") {
    this->parent = parent;
}

/**
 * \brief 	Default destructor of class Vehicule
 *
 * \param 	None
 * \return 	None
 */
ClusterLinks::~ClusterLinks() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void ClusterLinks::draw() {
    //std::cout << "clusterLink::draw()" << endl;
}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	t   a time_t
 * \return 	None
 * Display a vehicle as a disc at the current position
 */
void ClusterLinks::draw(unsigned long long t) {

    //cout << "ClusterLinks      §§§§§§§§§§§§§§§§§§§§" << endl;


    OGRSpatialReference* displayRef = CoordinateSystemManager::getInstance()->getDisplayRef();
    unordered_map<string, GeologicCluster*> clusters = parent->algoClustering.getClusters();
    unordered_map<string, GeologicCluster*>::iterator itc = clusters.begin();

    //cout << "clustering soze " << clusters.size() << endl;




    for (; itc!=clusters.end(); itc++){
        GeologicCluster* ckm = itc->second;
        Color color = parent->colors[ckm->name];
        if (ckm->name != "cluster::noise"){
            for (unsigned int i=0; i<ckm->records.size(); i++)  {
                // For all vehicles of this cluster
                Vehicule* currentVehicle = (Vehicule*) ckm->records[i];
                // We create a link between this vehicle and all the vehicle from this cluster
                glm::vec3 currentPosition = glm::vec3(currentVehicle->getX(),currentVehicle->getY(), 0.0);

                //cout << "state                           " << currentVehicle->state << endl;
                if(currentVehicle->state == "head") {
                    glPointSize(12);
                    glColor4d(1.0,0.0,0.0, 1.0); //glColor4d(c.red, c.green, c.blue, .9);
                    glBegin(GL_POINTS);
                    glVertex3d(currentVehicle->getX(), currentVehicle->getY(), 0.0);
                    glEnd();

                }
                for (unsigned int ii=0; ii<ckm->records.size(); ii++) {
                    if (i != ii) {
                        GeographicalObject* neigVehicle = ckm->records[ii];
                        glm::vec3 neigPosition = glm::vec3(neigVehicle->getX(),neigVehicle->getY(), 0.0);
                        glm::vec3 dis = currentPosition - neigPosition;
                        float distance = glm::length(dis);
                        if(distance <= currentVehicle->radius ){
                            glLineWidth(3);
                            glColor4d(color.red, color.green, color.blue, 1.0); //glColor4d(c.red, c.green, c.blue, .9);
                            glBegin(GL_LINE_STRIP);
                            glVertex3d(currentVehicle->getX(), currentVehicle->getY(), 0.0);
                            glVertex3d(neigVehicle->getX(), neigVehicle->getY(), 0.0);
                            glEnd();
                        }

                    }
                }
                /*/
                for (unsigned int ii=0; ii<ckm->records.size(); ii++) {
                    if (i != ii) {
                        GeographicalObject* neigVehicle = ckm->records[ii];
                        glLineWidth(3);
                        glColor4d(color.red, color.green, color.blue, 1.0); //glColor4d(c.red, c.green, c.blue, .9);
                        glBegin(GL_LINE_STRIP);
                        glVertex3d(currentVehicle->getX(), currentVehicle->getY(), 0.0);
                        glVertex3d(neigVehicle->getX(), neigVehicle->getY(), 0.0);
                        glEnd();
                    }
                }

                /*/

            }
        }
    }

    /*/
    unordered_map<GeographicalObject*, unordered_set<GeographicalObject*>>& connections = parent->algoClustering.connections;
    for (; itc!=clusters.end(); itc++) {
        GeologicCluster* ckm = itc->second;
        if (ckm->name != "cluster::noise") {
            for (unsigned int i=0; i<ckm->records.size(); i++) {
                // For all vehicles of this cluster
                Vehicule* currentVehicle = (Vehicule*) ckm->records[i];
                // We create a link between this vehicle and all the vehicle from this cluster
                for (unsigned int ii=0; ii<ckm->records.size(); ii++) {
                    if (i != ii) { // except for myself
                        GeographicalObject* neigVehicle = ckm->records[ii];
                        //if (geoMetrics::distanceTo(neigVehicle, currentVehicle, displayRef) <= currentVehicle->radius) {
                        if (connections.find(ckm->records[i]) != connections.end()) {

                            if (connections.at(ckm->records[i]).find(ckm->records[ii]) != connections.at(ckm->records[i]).end()) {
                            glLineWidth(3);
                            glColor4d(1.0, 0.0, 0.0, 1.0); //glColor4d(c.red, c.green, c.blue, .9);
                            glBegin(GL_LINE_STRIP);
                            glVertex3d(currentVehicle->getX(), currentVehicle->getY(), 0.0);
                            glVertex3d(neigVehicle->getX(), neigVehicle->getY(), 0.0);
                            glEnd();
                            }
                        }
                    }
                }
            }
        }
    }/*/

/*/
    std::cout << "clusterLink::draw(ttttttttttttttttt)" << endl;
    OGRSpatialReference* displayRef = CoordinateSystemManager::getInstance()->getDisplayRef();
    unordered_map<string, GeologicCluster*> clusters = parent->algoClustering.getClusters();
    unordered_map<string, GeologicCluster*>::iterator itc = clusters.begin();
    for (; itc!=clusters.end(); itc++){
        GeologicCluster* ckm = itc->second;
        if (ckm->name != "cluster::noise"){
            for (unsigned int i=0; i<ckm->records.size(); i++)  {
                // For all vehicles of this cluster
                Vehicule* currentVehicle = (Vehicule*) ckm->records[i];
                // We create a link between this vehicle and all the vehicle from this cluster
                for (unsigned int ii=0; ii<ckm->records.size(); ii++) {
                    if (i != ii) {
                        GeographicalObject* neigVehicle = ckm->records[ii];
                        glLineWidth(3);
                        glColor4d(1.0, 0.0, 0.0, 1.0); //glColor4d(c.red, c.green, c.blue, .9);
                        glBegin(GL_LINE_STRIP);
                        glVertex3d(currentVehicle->getX(), currentVehicle->getY(), 0.0);
                        glVertex3d(neigVehicle->getX(), neigVehicle->getY(), 0.0);
                        glEnd();
                    }
                }
            }
        }
    }
/*/
}
