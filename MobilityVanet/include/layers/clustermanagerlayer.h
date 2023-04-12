/**
 * \file	ClusterManagerLayer.h
 */

#ifndef CLUSTERMANAGERLAYER_H
#define CLUSTERMANAGERLAYER_H

#include <Scene/singlelayer.h>
#include <layercheckbox.h>
#include "geologic-core/objects/geo/untemporal/Grid.h"
#include <geologic-core/algorithms/clustering/dbscan/DBscan.h>
#include <geologic-core/algorithms/clustering/kmeans/Kmeans.h>
#include "clustering/propagclustering.h"
#include "Common/color.h"
#include <clustering/testclustering.h>


class Vanet;

class ClusterManagerLayer : public SingleLayer {

    public:
        //*/ -------------------------------------------------
        ClusterManagerLayer(Vanet* parent);
        virtual ~ClusterManagerLayer();

        //*/ -------------------------------------------------
        virtual void draw();
        virtual void draw(unsigned long long t);
        virtual void drawCheckbox(QWidget *tab, QVBoxLayout *layout);

        //*/ -------------------------------------------------
        Vanet* parent;
        std::unique_ptr<Grid> clusterGrid;

        //std::unordered_map<std::string, GeologicCluster*> currentClusters;
        //DBscan algoClustering;
        //PropagClustering algoClustering;
        testClustering  algoClustering;

        std::unordered_map<std::string, Color> colors; // A color for each Cluster
        time_t currentT;
};

#endif // CLUSTERMANAGER_H
