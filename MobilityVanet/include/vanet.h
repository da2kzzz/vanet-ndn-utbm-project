#ifndef VANET_H
#define VANET_H

#include <iostream>
#include "misc/parameters.h"
#include "misc/alerteinfo.h"
#include "misc/fleet.h"
#include "misc/antennamap.h"

#include "Scene/movableview.h"

#include "layers/clustermanagerlayer.h"
#include "layers/propagationlayer.h"
#include "displayClasses/clusterLinks.h"
#include "displayClasses/vehiclePosition.h"
#include "displayClasses/vehiclePropagCircle.h"
#include "displayClasses/vehiclePropagMeshes.h"
#include "displayClasses/vehicleTrace.h"

#include <geologic-core/core/GeologicModule.h>
#include <geologic-core/core/geologic-mapper/GeologicMapper.h>



class Vanet : public DataManager, public Data {

    public:
        //*/ -------------------------------------------------
        Vanet(MovableView* main_view);
        virtual ~Vanet();

        //*/ -------------------------------------------------
        void draw();
        void draw(unsigned long long t);

        void hoverTrackSignal(OGRPoint* location, time_t t);
        void hoverClickSignal(OGRPoint* location, time_t t);
        void hoverReleaseSignal();
        Vehicule* trackedVehicle = nullptr;
        bool isVehicleSelected = false;

        //*/ -------------------------------------------------
        void loadmobility();
        void vehiculeTraj();
        void loadAntennas();

        std::unique_ptr<OGRPoint> maille2OGR(int meshPos, double Xmin, double Ymin, int nbmaillesX, double pdm);
        int OGR2AntennaMesh(OGRPoint* temp);

        const std::vector<GeographicalObject*>& getPopulation();
        int getVehicleRadius();

        //*/ -------------------------------------------------
        GeologicMapper* geomap;
        MovableView* main_view;

        Parameters params;
        std::unique_ptr<Fleet> fleet;
        std::unique_ptr<AntennaMap> antMapper;
        std::unique_ptr<ClusterManagerLayer> clusterManager;
        std::unique_ptr<PropagationLayer> propagManager;

        std::unique_ptr<ClusterLinks> clusterLinks;
        std::unique_ptr<VehiclePosition> vehiclePosition;
        std::unique_ptr<VehiclePropagCircle> vehiclePropagCircle;
        std::unique_ptr<VehiclePropagMeshes> vehiclePropagMeshes;
        std::unique_ptr<VehicleTrace> vehicleTrace;
        //std::unique_ptr<> testCluster;


        std::vector<GeographicalObject*> population;
        time_t nextTime;
};

#endif // VANET_H
