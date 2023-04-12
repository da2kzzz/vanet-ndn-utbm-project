#include "testclustering.h"

//
//  dbscan.cpp


#include <stdio.h>
#include "glm/glm.hpp"

#include "geologic-core/algorithms/clustering/dbscan/DBscan.h"

using namespace std;
using namespace customToolbox;
using namespace std::chrono;


/**
 * \brief     Default constructor of class dbscan
 *
 * \param     None
 * \return     None
 */
testClustering::testClustering() {

}

/**
 * \brief     Default destructor of class
 *
 * \param     None
 * \return     None
 */
testClustering::~testClustering() {
    this->clear(true);
}

/**
 * \brief     main fonction
 *
 * \param
 * \return
 */
void testClustering::startDBscan(Fleet* fleet, AntennaMap*  amap, time_t t) {

    cout << " starDBSCan begin " << endl;

    //*/ -------------------------------------------------
    this->clear(true);


    //*/ -------------------------------------------------

    if (fleet == nullptr) return;
    if (fleet->fleet.size() <= 0) return;

    //init the information of all the object in the time t

    unordered_map<string, Vehicule*>::iterator it = fleet->fleet.begin();
    for (; it!=fleet->fleet.end(); it++) {
        Vehicule* currentVehicule = it->second;
        if (currentVehicule->isActive(t)){
            //cout << "alive " << endl;
            currentVehicule->color = Color(0.01, 0.6, 0.9, 0.4);//default color of vehicules
            currentVehicule->state = "undefined"; ////////  it need to add a new parameter in class vehicule: string state < HEAD, MEMBER, UNDEFINED>
            currentVehicule->parameterMobilite = 0; // new parameter added in class vehicule double parameterMobilite to stock the parameter mobility
            //GeographicalObject* currentOBject =  (GeographicalObject*) currentVehicule;

            unordered_set<Vehicule*> neighbors = this->getNeighbors(fleet, amap, t, currentVehicule);
            //cout << "neigh nb " << neighbors.size() << endl;


            this->calculateParameter(currentVehicule, t, neighbors, fleet);
        }
        //

    }

    //cout << "nb vehicule alive " << VehiculeAlive.size() << endl ;
    //*/ -------------------------------------------------
    cout << "Running testClustering..." << endl;


    //calculate all the parameters


    if (fleet->fleet.size() <= 0) return;
    //*/ -------------------------------------------------
    high_resolution_clock::time_point t1 = high_resolution_clock::now();


    //for each object

    unordered_map<string, Vehicule*>::iterator i_vehicule = fleet->fleet.begin();
    for (; i_vehicule!=fleet->fleet.end(); i_vehicule++) {

        Vehicule* currentVehicule = i_vehicule->second;

        if (currentVehicule->isActive(t)) {

             //GeographicalObject* currentOBject =  (GeographicalObject*) currentVehicule;

             //cout << "alive x y " << currentVehicule->getX() << "  " << currentVehicule->getY() << endl;
             //cout << "ra " << currentVehicule->radius << endl;
            //cout << "vvehicule id " << currentVehicule->id << endl;
            //cout << "vvehicule name " << currentVehicule->name << endl;

            unordered_set<Vehicule*> neighbors = this->getNeighbors(fleet, amap, t, currentVehicule);


             if(currentVehicule->state == "undefined" && neighbors.size() > 1){

                 //connections.insert(pair<Vehicule*, unordered_set<Vehicule*>>(currentVehicule, neighbors));
                 //get the neighbors vehicules of currentVehicule (include the currentVehicule itself)



                 GeologicCluster* cdb = new GeologicCluster("cluster::noise", this);
                 clusters.insert(pair<string, GeologicCluster*>(cdb->id, cdb));
                 //get the head
                 Vehicule* vehiculeFound = this->getHead(currentVehicule, neighbors, fleet);
                 //cout << " starDBSCan test 1" << endl;

                 //Vehicule* headFound = fleet->fleet.at(vehiculeFound->name);



                 if(  vehiculeFound->state == "undefined") {
                     //cout << " starDBSCan test undefined" << endl;

                     //if the head found is " undefined", it should creat a new cluster by using this objec as the head Cluster
                     string clusterName = "cluster::"+str(this->clusters.size()-1);
                     //string clusterName = headFound->name; ///
                     GeologicCluster* currentCluster = clusters["cluster::noise"];
                     currentCluster = new GeologicCluster(clusterName, this); //new cluster


                    //fleet->fleet.at(vehiculeFound->name)->state = "head";


                     neighbors.erase(vehiculeFound);

                     vehiculeFound->headName = currentCluster->id;
                     vehiculeFound->state = "head";

                     fleet->fleet.at(vehiculeFound->name)->state = "head";
                     fleet->fleet.at(vehiculeFound->name)->headName = currentCluster->id;

                     if(currentVehicule->id == vehiculeFound->id) {

                        // new parameter added in object/vehicule to define the head of each oject

                     } else {

                     }

                     currentCluster->addObject(vehiculeFound);


                     while (neighbors.size() > 0) {/////

                         Vehicule* record = *(neighbors.begin());

                         neighbors.erase(neighbors.begin());

                         fleet->fleet.at(record->name)->state = "member";
                         fleet->fleet.at(record->name)->headName = currentCluster->id;
                         currentCluster->addObject(record);
                     }

                     clusters.insert(pair<string, GeologicCluster*>(currentCluster->id, currentCluster));// new cluster

                 } else if (vehiculeFound->state == "head") {
                     //cout << " starDBSCan head " << endl;

                     neighbors.erase(vehiculeFound);


                     fleet->fleet.at(currentVehicule->name)->state = "member";

                     //neighbors.erase(fleet->fleet.at(vehiculeFound->name));//////test

                     string headBelong = vehiculeFound->headName;

                     while (neighbors.size() > 0) {
                         Vehicule* record = *(neighbors.begin());////////???????
                         neighbors.erase(neighbors.begin());
                         fleet->fleet.at(record->name)->state = "member";
                         fleet->fleet.at(record->name)->headName = headBelong;

                         clusters.at(headBelong)->addObject(record);
                     }
                     //clusters.at(headBelong)->addObject(currentVehicule);

                 } else if (vehiculeFound->state == "member") {

                     //cout << " starDBSCan head memeber" << endl;
                     neighbors.erase(vehiculeFound);


                     //neighbors.erase(fleet->fleet.at(vehiculeFound->name));//////test
                     //string headId = fleet->fleet.at(vehiculeFound->headName)->headName;

                     //get the head id of vehicule of the vehiculeFound belong tp
                     string headBelong = vehiculeFound->headName;

                     //cout << "head Belong  " << headBelong << endl;

                     //cout << "neigh size   " << neighbors.size() << endl;

                     while (neighbors.size() > 0) {
                         Vehicule* record = *(neighbors.begin());///////???


                         fleet->fleet.at(record->name)->state = "member";
                         fleet->fleet.at(record->name)->headName = headBelong;

                         clusters.at(headBelong)->addObject(record);
                         //clusters.at(vehiculeFound->headName)->addObject(record);
                         neighbors.erase(neighbors.begin());
                         //cout << " ici test2 " << endl;

                     }
                     //cout << " ici test fin dans member " << endl;

                     //clusters.at(headBelong)->addObject(currentVehicule);
                 }
             }

        }





    }
    //cout << "Running PropagClustering..." << endl;
    //cout << clusters.size() << endl;

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::milli> time_span = t2 - t1;

    this->clear();
    cout << "fin testClustering  ..." << endl;

}



/**
 * \brief      Fonction to get neighbor of currentObject
 *
 * \param
 * \return
 */
unordered_set<Vehicule*> testClustering::getNeighbors(Fleet* fleet, AntennaMap* amap, time_t t, Vehicule* currentVehicule) {

/*/
    GridMesh* currentMesh_vehiclegrid = fleet->vehicleGrid->getMesh(currentObject); //Get the current mesh of this vehicule


    int meshPos_antennagrid = this->OGR2AntennaMesh(currentObject);//Get the mesh pos********************


    //int meshPos_antennagrid = fleet->parent->OGR2AntennaMesh(currentObject);

    std::unordered_set<GeographicalObject*> connectedNeigs;

    //cout << "a map size " << amap->AMap->size() << endl;
    //cout << endl;

    if (amap->AMap->find(meshPos_antennagrid) != amap->AMap->end()) {

        std::vector<std::pair<std::unique_ptr<OGRPoint>, double>>& meshes_antennagrid = amap->AMap->at(meshPos_antennagrid)->meshes;//get the meshes of current antenna
        std::vector<GeographicalObject*> neigs;


        if (currentMesh_vehiclegrid != nullptr) {
            neigs = currentMesh_vehiclegrid->getNeighborObjects(true); // Get the neighbor objects of this mesh
            cout << "neigh mesh    " << neigs.size() << endl;
        } else {
            cout << " nooo neigh mesh    " << endl;
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






         //connection
        if (connectedNeigs.size() > 0) {
            connections.insert(pair<GeographicalObject*, unordered_set<GeographicalObject*>>(currentObject, connectedNeigs));
            string clusterName = "cluster::"+str(this->clusters.size()-1);
            currentCluster = new GeologicCluster(clusterName, this);
            clusters.insert(pair<string, GeologicCluster*>(currentCluster->id, currentCluster));
            this->expandCluster(fleet, amap, t, currentCluster, connectedNeigs);
      }

    }
/*/

    std::unordered_set<Vehicule*> connectedNeigs;

    glm::vec3 currentPosition = glm::vec3(currentVehicule->getX(), currentVehicule->getY(), 0.0);

    unordered_map<string, Vehicule*>::iterator i_vehicule = fleet->fleet.begin();
    for (; i_vehicule!=fleet->fleet.end(); i_vehicule++) {
        Vehicule* tempVehicule = i_vehicule->second;
        glm::vec3 temptPosition = glm::vec3(tempVehicule->getX(), tempVehicule->getY(), 0.0);
        glm::vec3 distance = temptPosition - currentPosition;
        float dis = glm::length(distance);
        if (dis <= currentVehicule->radius ) {
            connectedNeigs.insert(tempVehicule);
        }

    }

    //cout << "connection nei "<< connectedNeigs.size() << endl;

    return connectedNeigs;

}



/**
 * \brief     Fonction to clear
 *
 * \param
 * \return
 */
void testClustering::clear(bool hard) {
    if (hard) {
        this->clearCluster();
        this->population.clear();
    }
    this->bbox.clear();

    //this->VehiculeAlive.clear();
}



/**
 * \brief     Fonction to get the object convenable in the group of currentObject and its neighbors
 *
 * \param
 * \return
 */
Vehicule* testClustering::getHead(Vehicule* currentObject, unordered_set<Vehicule*> neighbors, Fleet* fleet){

    //add new parameter in class geoObject : parameterMobilite
    //get parameter of currentObject


    Vehicule* newHead = currentObject;

    for ( auto it = neighbors.begin(); it != neighbors.end(); ++it )  {
        Vehicule* current = *it;
        //Vehicule* currentVehicule = fleet->fleet.at(currentVehicule->name);
        if(current->parameterMobilite >= currentObject->parameterMobilite ) {
            newHead = current;
        }
    }


    return newHead;

}

//calculate the parameter of each object

void testClustering::calculateParameter(Vehicule* currentObject, time_t t, std::unordered_set<Vehicule*> neighbors,  Fleet* fleet){

    glm::vec3 tempvVelocity = currentObject->getVelocity(t);
    //paraameter of the velocity of each object in the neighbors


    for ( auto it = neighbors.begin(); it != neighbors.end(); ++it ) {
        Vehicule* current = *it;
         tempvVelocity += current->getVelocity(t);;

    }
    currentObject->parameterMobilite = currentObject->parameterMobilite - glm::length(tempvVelocity);
    currentObject->parameterMobilite += neighbors.size();

}


void testClustering::setAntennagridParams(double xmin, double ymin, int nbmailles, int pdm) {
    XminA = xmin;
    YminA = ymin;
    nbmaillesXA = nbmailles;
    pdmA = pdm;
}


int testClustering::OGR2AntennaMesh(OGRPoint* temp) {

    //cout << " x y " << temp->getX() << "  " << temp->getY() << endl;

    //cout << nbmaillesXA * ((int) ((temp->getY() - YminA)/pdmA)) + ((int) ((temp->getX() - XminA)/pdmA)) << endl;

    return nbmaillesXA * ((int) ((temp->getY() - YminA)/pdmA)) + ((int) ((temp->getX() - XminA)/pdmA));
}


void testClustering::findRealConnection() {
/*/
    auto it = clusters.begin();

    for (; it != clusters.end(); it++) {
        GeologicCluster* currentCluster = it->second;

   }
   /*/
}



