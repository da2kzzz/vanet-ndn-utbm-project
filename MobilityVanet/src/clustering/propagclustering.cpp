/**
 * \file	DBscan.cpp
 * \class   DBscan
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/05/2015
 * \brief	Methods of Kmeans object
 *
 * Methods of DBscan object
 */

#include "clustering/propagclustering.h"

using namespace std;
using namespace customToolbox;
using namespace std::chrono;


/**
 * \brief 	Default constructor of class Kmeans
 *
 * \param 	None
 * \return 	None
 */
PropagClustering::PropagClustering() {}

/**
 * \brief 	Default destructor of class Kmeans
 *
 * \param 	None
 * \return 	None
 */
PropagClustering::~PropagClustering() {
    this->clear(true);
}

/**
 * \brief 	Default constructor of class Kmeans
 *
 * \param 	None
 * \return 	None
 */
void PropagClustering::startClustering(Fleet* fleet, AntennaMap* amap, time_t t) {
    //*/ -------------------------------------------------
    this->clear(true);
    this->connections.clear();

    //*/ -------------------------------------------------
    unordered_map<string, Vehicule*>::iterator it = fleet->fleet.begin();
    for (; it!=fleet->fleet.end(); it++) {
        Vehicule* currentVehicule = it->second;
        if (currentVehicule->isActive(t)){
            currentVehicule->color = Color(0.01, 0.6, 0.9, 0.4);//default color of vehicules
            notVisited.insert((GeographicalObject*) currentVehicule);
        }
    }

    GeologicCluster* cdb = new GeologicCluster("cluster::noise", this);
    clusters.insert(pair<string, GeologicCluster*>(cdb->id, cdb));

    //*/ -------------------------------------------------
    //cout << "Running PropagClustering..." << endl;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    while (this->notVisited.size() > 0) {
        GeographicalObject* currentObject = *(notVisited.begin());
        notVisited.erase(notVisited.begin());
        visited.insert(currentObject);//add this to the list visited

        GeologicCluster* currentCluster = clusters["cluster::noise"];

        GridMesh* currentMesh_vehiclegrid = fleet->vehicleGrid->getMesh(currentObject); //Get the current mesh of this vehicule
        int meshPos_antennagrid = this->OGR2AntennaMesh(currentObject);//Get the mesh pos********************

        if (amap->AMap->find(meshPos_antennagrid) != amap->AMap->end()) {cout << "noo" << endl;}
        if (amap->AMap->find(meshPos_antennagrid) != amap->AMap->end()) {

            std::vector<std::pair<std::unique_ptr<OGRPoint>, double>>& meshes_antennagrid = amap->AMap->at(meshPos_antennagrid)->meshes;//get the meshes of current antenna
            std::vector<GeographicalObject*> neigs;
            std::unordered_set<GeographicalObject*> connectedNeigs;
            if (currentMesh_vehiclegrid != nullptr){
                //cout << "size " <<neigs.size() << endl;
                neigs = currentMesh_vehiclegrid->getNeighborObjects(true); // Get the neighbor objects of this mesh

            }

            //get real neighbors
            for (unsigned int i=0; i<neigs.size(); i++) { //For each neighbor
                if (((Vehicule*) neigs[i])->isActive(t) && neigs[i] != currentObject) { //if it is active (it still move)
                    int tempPos_antennagrid = this->OGR2AntennaMesh(neigs[i]); //get the meshPos of this neighbor

                    if (amap->AMap->find(tempPos_antennagrid) != amap->AMap->end()) { //if this meshPos exist
                        for (unsigned int j=0; j<meshes_antennagrid.size(); j++) { // for all the meshes of currentObject
                            int meshesPos_antennagrid = this->OGR2AntennaMesh((meshes_antennagrid[j].first).get());
                            if (tempPos_antennagrid == meshesPos_antennagrid) { //if the neighbor is in the mesh of currentObject
                                connectedNeigs.insert(neigs[i]); //Add it to the tempCluster
                                break;
                            }
                        }
                    }
                }
            }

            if (connectedNeigs.size() > 0) {
                connections.insert(pair<GeographicalObject*, unordered_set<GeographicalObject*>>(currentObject, connectedNeigs));
                string clusterName = "cluster::"+str(this->clusters.size()-1);
                currentCluster = new GeologicCluster(clusterName, this);
                clusters.insert(pair<string, GeologicCluster*>(currentCluster->id, currentCluster));
                this->expandCluster(fleet, amap, t, currentCluster, connectedNeigs);
            }
        }
        currentCluster->addObject(currentObject);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::milli> time_span = t2 - t1;
    //cout << "PropagClustering done. " << "It took me " << time_span.count() << " milliseconds." << endl;

    this->clear();
}

/**
 * \brief 	Default constructor of class Kmeans
 *
 * \param 	None
 * \return 	None
 */
void PropagClustering::expandCluster(Fleet* fleet, AntennaMap* amap , time_t t, GeologicCluster* currentCluster, unordered_set<GeographicalObject*>& neighbors) {
    //cout << "Expanding cluster " << currentCluster->id << endl;
    while (neighbors.size() > 0) {
        GeographicalObject* record = *(neighbors.begin());
        neighbors.erase(neighbors.begin());

        if (this->visited.find(record) == this->visited.end()) {
            this->visited.insert(record);

            int meshPos_antennagrid = this->OGR2AntennaMesh(record);
            if (amap->AMap->find(meshPos_antennagrid) != amap->AMap->end()) { ///TODO will always be ok

                std::vector<std::pair<std::unique_ptr<OGRPoint>, double>>& meshes_antennagrid = amap->AMap->at(meshPos_antennagrid)->meshes;
                GridMesh* currentVehicleMesh = fleet->vehicleGrid->getMesh(record); //Get the current mesh of this vehicule

                vector<GeographicalObject*> otherNeighbors;
                unordered_set<GeographicalObject*> otherConnectedNeighbors;
                if (currentVehicleMesh != nullptr) otherNeighbors = currentVehicleMesh->getNeighborObjects(true); // Get the neighbor objects of this mesh

                //get real neighbors
                for (unsigned int i=0; i<otherNeighbors.size(); i++) { //For each neighbor
                    if (((Vehicule*) otherNeighbors[i])->isActive(t) && otherNeighbors[i] != record) { //if it is active (it still move)
                        int tempPos_antennagrid = this->OGR2AntennaMesh(otherNeighbors[i]); //get the meshPos of this neighbor

                        if (amap->AMap->find(tempPos_antennagrid) != amap->AMap->end()) { //if this meshPos exist
                            for (unsigned int j=0; j<meshes_antennagrid.size(); j++) { // for all the meshes of currentObject
                                int meshesPos_antennagrid = this->OGR2AntennaMesh((meshes_antennagrid[j].first).get());
                                if (tempPos_antennagrid == meshesPos_antennagrid) { //if the neighbor is in the mesh of currentObject
                                    otherConnectedNeighbors.insert(otherNeighbors[i]); //Add it to the tempCluster
                                    break;
                                }
                            }
                        }
                    }
                }
                connections.insert(pair<GeographicalObject*, unordered_set<GeographicalObject*>>(record, otherConnectedNeighbors));
                neighbors.insert(otherConnectedNeighbors.begin(), otherConnectedNeighbors.end());
            }
        }

        if (this->notVisited.find(record) != this->notVisited.end()) {
            //cout << record->name << " is wihtin NotVisited set. We remove it from NotVisited, and add it to the cluster " << currentCluster->id << endl;
            this->notVisited.erase(record);
            currentCluster->addObject(record);
        }
    }
}

/**
 * \brief 	Default constructor of class Kmeans
 *
 * \param 	None
 * \return 	None
 */
void PropagClustering::clear(bool hard) {
    if (hard) {
        this->clearCluster();
        this->population.clear();
    }
    this->bbox.clear();


    this->visited.clear();
    this->notVisited.clear();
}

/**
 * \brief 	Converts a mesh Id to a OGRPoint
 *
 * \param 	meshPos The Id of the mesh
 * \param   Xmin    The min value in the longitude axis
 * \param   Ymin    The min value in the latitude axis
 * \param   pdm     The size of one mesh
 * \return 	None
 */
int PropagClustering::OGR2AntennaMesh(OGRPoint* temp) {
    return nbmaillesXA * ((int) ((temp->getY() - YminA)/pdmA)) + ((int) ((temp->getX() - XminA)/pdmA));
}

/**
 * \brief 	Converts a mesh Id to a OGRPoint
 *
 * \param 	meshPos The Id of the mesh
 * \param   Xmin    The min value in the longitude axis
 * \param   Ymin    The min value in the latitude axis
 * \param   pdm     The size of one mesh
 * \return 	None
 */
void PropagClustering::setAntennagridParams(double xmin, double ymin, int nbmailles, int pdm) {
    XminA = xmin;
    YminA = ymin;
    nbmaillesXA = nbmailles;
    pdmA = pdm;
}
