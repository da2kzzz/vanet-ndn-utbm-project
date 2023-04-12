/**
 * \file	ClusterManager.cpp
 * \class	ClusterManager
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	ClusterManager is managing all the meshes of the environment
 */

#include <layers/clustermanagerlayer.h>
#include "vanet.h"

using namespace std;

/**
 * \brief 	Default constructor of class ClusterManager
 *
 * \param 	None
 * \return 	None
 */
ClusterManagerLayer::ClusterManagerLayer(Vanet* parent) : SingleLayer(nullptr) {
    this->parent = parent;
    clusterGrid = std::unique_ptr<Grid>(new Grid(OGRPoint(parent->params.Xmin, parent->params.Ymin, 0), 20000)); //(933225, 2289875, 0)
    clusterGrid->meshEnv(7);
    currentT = -1;
}

/**
 * \brief 	Default destructor of class ClusterManager
 *
 * \param 	None
 * \return 	None
 */
ClusterManagerLayer::~ClusterManagerLayer() {}


/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void ClusterManagerLayer::draw() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	t   a time_t
 * \return 	None
 * Displays all the meshes of the environement
 */
void ClusterManagerLayer::draw(unsigned long long t) {


    cout << " clusterManager draw begin " << endl;
    if (visible) {
        if (currentT != t) {
            currentT = t;

            OGRSpatialReference* displayRef = CoordinateSystemManager::getInstance()->getDisplayRef();
            // Now we cluster all the positions (vehicles)
            std::unordered_map<std::string, Color> tempColors; // The future colors of the clusters

            algoClustering.setDataRef(displayRef);

            //algoClustering.startDBscan(&(parent->population), pair<string, double>("meters", parent->params.vehicleRadius), 1);
            //algoClustering.startKmeans(&population, 20);
            //algoClustering.startClustering(parent->fleet.get(), parent->antMapper.get(), t);

            /////NEW
            algoClustering.startDBscan(parent->fleet.get(), parent->antMapper.get(), t);

            cout << " clusterManager fin starDBSCane  " << endl;

            // We get back the clusters and list them
            unordered_map<string, GeologicCluster*> clusters;
            clusters = algoClustering.getClusters();

            cout << clusters.size() << endl;

           // GeologicCluster* tempC = clusters[];
          //  cout << "na meeeeeeeeeeee " << tempC->id << "   " << tempC->records.size() << endl;
            unordered_map<string, GeologicCluster*>::iterator itc = clusters.begin();

            for (; itc!=clusters.end(); itc++) {

                GeologicCluster* ckm = itc->second;
                //cout << "cluster size " << ckm->records.size() << endl;
                Color c = Color(0.1, 0.1, 0.1, 0.4); //default color

                // In the case of DBscan, we don't do anything for the cluster Noise
                if (ckm->id != "cluster::noise") {

                    // At this moment, we check the cluster ckm
                    // We will look inside the Grid. The Grid contains the positions (centroids) of the clusters from t-1
                    // We get back the closest (t-1) cluster from ckm. The color of this last cluster will become the new color of ckm
                    vector<GeographicalObject*> neigs;

                    // The current mesh within the Grid of ckm
                    GridMesh* ckmMesh = this->clusterGrid->getMesh(ckm, true); //it's an object not already in the mesh, we are not sure its here
                    // We get the neighbors within a limited range (hence the 'true' parameter)
                    if (ckmMesh != nullptr) neigs = ckmMesh->getNeighborObjects(true);

                    //cout << neigs.size() << endl;

                    // If there is no neighbors, this means there wasn't any cluster at t-1 close to ckm
                    if (neigs.size() <= 0) {
                        c = Color((rand()%100)/100.0, (rand()%100)/100.0, (rand()%100)/100.0, 0.6);
                        tempColors.insert(pair<string, Color>(ckm->name, c));
                    } else {
                        // Else, we just get the closest t-1 cluster
                        double bestDistance = -1;
                        GeographicalObject* bestCKm = nullptr;
                        for (unsigned int i=0; i<neigs.size(); i++) {
                            GeographicalObject* nck = neigs[i];
                            double distance = geoMetrics::distanceTo(ckm, nck, displayRef);
                            if (bestDistance == -1 || distance <= bestDistance) {
                                bestDistance = distance;
                                bestCKm = nck;
                            }
                        }

                        // We remove this (t-1) cluster from the Grid
                        // This is to ensure that no other cluster use this color at the same time
                        clusterGrid->getMesh(bestCKm)->mapObjects.erase(bestCKm->name);
                        // We set the color from the last cluster
                        c = this->colors[bestCKm->name];
                        // And we add it to the new set of colors
                        tempColors.insert(pair<string, Color>(ckm->name, c));
                    }

                }
            }



            // We just clear all the (t-1) colors
            this->colors.clear();
            // And set the new colors
            this->colors = std::move(tempColors);

            // We clear the Grid. We remove the positions of the previous clusters (t-1)
            // We use 'true' as a parameter to free the memory created in the next loop
            this->clusterGrid->clearGrid(true);

            //cout << "clusterManager test ici 2" << endl;

            // For all the cluster generated at t, we add them (their centroid position) to the grid
            itc = clusters.begin();
            for (; itc!=clusters.end(); itc++) {
                GeologicCluster* ckm = itc->second;
                if (ckm->name != "cluster::noise") {
                    // We insert the centroid of this cluster to the grid
                    GeographicalObject* ckmBack = new GeographicalObject(ckm);
                    // If the centroid is not contained within the grid (==> the grid is too small), then we delete it
                    if (!(this->clusterGrid->update(ckmBack, ckmBack, 0))) {
                        delete(ckmBack);
                        ckmBack = nullptr;
                    }
                }
            }

        }


        unordered_map<string, GeologicCluster*> clusters = algoClustering.getClusters();
        unordered_map<string, GeologicCluster*>::iterator itc = clusters.begin();
        for (; itc!=clusters.end(); itc++) {
            GeologicCluster* ckm = itc->second;
            Color c = colors[ckm->name];

            if (ckm->id != "cluster::noise") {
                for (unsigned int i=0; i<ckm->records.size(); i++) {
                    Vehicule* currentVehicle = (Vehicule*) ckm->records[i];
                    currentVehicle->color = c;
                    currentVehicle->hasCluster = true;
                }
            }
        }
    } else {
        algoClustering.clear(true);
        this->clusterGrid->clearGrid(true);
        this->colors.clear();
    }


}

/*!
 * \brief SingleLayer::drawCheckbox draw the layer checkbox
 * \param tab the tab widget in which to display the checkboxes
 * \param layout the layout in which to add the checkboc (layout is in tab)
 */
void ClusterManagerLayer::drawCheckbox(QWidget *tab, QVBoxLayout *layout) {
    if (tab != nullptr && layout != nullptr) {
        _my_checkbox = new LayerCheckBox(this, QObject::tr("Clustering ?"), tab);
        layout->addWidget(_my_checkbox);
    }
}
