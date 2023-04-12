/**
 * \file	Vanet.cpp
 * \class	Vanet
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	Fleet is a set of different vehicle.
 * Fleet is actually a receptacle of all trajectories followed by every vehicle of the environement
 * This can be modified by saying that fleet is a map of vehicle, and where each vehicle has its own draw method
 */

#include "vanet.h"

using namespace std;
using namespace customToolbox;
using namespace std::chrono;

/**
 * \brief 	Default constructor of class Fleet
 *
 * \param 	None
 * \return 	None
 */
Vanet::Vanet(MovableView* main_view) : Data("Vanet") {
    srand(time(nullptr));
    params.load("./parameters.xml");

    this->main_view = main_view;
    antMapper = make_unique<AntennaMap>();
    fleet = make_unique<Fleet>(this);
    geomap = nullptr;

    clusterManager = make_unique<ClusterManagerLayer>(this);
    propagManager = make_unique<PropagationLayer>(this);

    clusterLinks = make_unique<ClusterLinks>(clusterManager.get());
    vehiclePosition = make_unique<VehiclePosition>(this);
    vehiclePropagCircle = make_unique<VehiclePropagCircle>(this);
    vehiclePropagMeshes = make_unique<VehiclePropagMeshes>(this);
    vehicleTrace = make_unique<VehicleTrace>(this);
}

/**
 * \brief 	Default destructor of class Fleet
 *
 * \param 	None
 * \return 	None
 */
Vanet::~Vanet() {}

/**
 * \brief 	Loads the different trajectories of the vehicles
 *
 * \param 	None
 * \return 	None
 */
void Vanet::loadmobility() {
    cout << "Loading vehicles..." << endl;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    string line;
    ifstream fichierTrace("fich3.csv");
    vector<string> motifHeaders;
    bool header = true;
    while (getline(fichierTrace, line)) {
        vector<string> lineTab = split(line, ';');
        if (header) {
            for (unsigned int i=0; i<lineTab.size(); i++) motifHeaders.push_back(lineTab[i]);
            header = false;
            continue;
        }

        Vehicule* veh = new Vehicule(this->fleet.get());
        veh->id = atoi(lineTab[0].c_str());
        ((GeographicalObject*) veh)->name = lineTab[0];
        veh->statut = lineTab[1];
        veh->heureD = atoi(lineTab[2].c_str());
        veh->indiceO = atoi(lineTab[3].c_str());
        veh->indiceD = atoi(lineTab[4].c_str());
        veh->nbPositions = atoi(lineTab[5].c_str());

        veh->transmissionDuration = params.transmissionDuration;
        veh->tabouDuration = params.tabouDuration;


        for (unsigned int j=6; j<lineTab.size(); j++) veh->vPositions.push_back(stoi(lineTab[j].c_str()));
        fleet->fleet.insert(pair<string, Vehicule*>(lineTab[0], veh));
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::milli> time_span = t2 - t1;
    cout << "Loading done. " << "It took me " << time_span.count() << " milliseconds." << endl;
}

/**
 * \brief 	Constructs the trajectories of all the vehicles
 *
 * \param 	None
 * \return 	None
 * Converts the mesh values to lat lon positions, converts to display Coordinate system, etc.
 */
void Vanet::vehiculeTraj() {
    cout << "Loading trajs..." << endl;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    unique_ptr<OGRSpatialReference> dataRef = CoordinateSystemManager::getInstance()->getCSbyRef(params.data_epsg_code);
    OGRSpatialReference* displayRef = CoordinateSystemManager::getInstance()->getDisplayRef();

    string idveh;
    // For each vehicle, we construct a Trajectory object from its positions
    auto p = fleet->fleet.begin();
    for (; p!=fleet->fleet.end(); p++) {
        Trajectory* VTraj = new Trajectory();
        TemporalGeoPoint* lastPos = nullptr;

        for (int j=0; j<p->second->nbPositions; j++) {
            unique_ptr<OGRPoint> pt = maille2OGR(p->second->vPositions[j], params.Xmin, params.Ymin, params.nbmaillesX, params.pdmRoad);
            unique_ptr<TemporalGeoPoint> Tgeopoint = make_unique<TemporalGeoPoint>(pt.get(), j);
            idveh = customToolbox::str(p->second->id);

            // If the distance between the last point and the current point is more than 2 meters
            if (lastPos != nullptr && geoMetrics::distanceTo(Tgeopoint.get(), lastPos, dataRef.get()) > 2) {
                geoMetrics::convertTo(Tgeopoint.get(), dataRef.get(), displayRef);
                VTraj->addPosition(Tgeopoint.get());
            }

            if (lastPos == nullptr) {
                geoMetrics::convertTo(Tgeopoint.get(), dataRef.get(), displayRef);
                VTraj->addPosition(Tgeopoint.get());
            }
            lastPos = VTraj->trajectory->back();
        }

        // We add that Trajectory to the list of traj of that vehicle
        p->second->listTrajectories.insert(pair<std::string, Trajectory*>("0", VTraj));
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::milli> time_span = t2 - t1;
    cout << "Loading done. " << "It took me " << time_span.count() << " milliseconds." << endl;
}

/**
 * \brief 	Loads the road meshes of the environement
 *
 * \param 	None
 * \return 	None
 */
void Vanet::loadAntennas() {
    cout << "Loading antennas..." << endl;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    duration<double, std::milli> time_span1;

    unique_ptr<OGRSpatialReference> dataRef = CoordinateSystemManager::getInstance()->getCSbyRef(params.data_epsg_code);
    OGRSpatialReference* displayRef = CoordinateSystemManager::getInstance()->getDisplayRef();

    string line;
    ifstream fichierTrace("routePowerAll.csv");
    while (getline(fichierTrace, line)) {
        vector<string> lineTab = split(line, ';');

        Antenna* antenna = new Antenna();
        ((GeoZone*) antenna)->name = lineTab[0];
        antenna->centroidMesh = atoi(lineTab[0].c_str());
        unique_ptr<OGRPoint> tmp = maille2OGR(antenna->centroidMesh, params.XminA, params.YminA, params.nbmaillesXA, params.pdmA);
        geoMetrics::convertTo(tmp.get(), dataRef.get(), displayRef);

        antenna->setX(tmp->getX());
        antenna->setY(tmp->getY());
        antenna->pdm = params.pdmA;

        for (unsigned int i=1; i<lineTab.size(); i+=2) {
            double newVal = atof(lineTab[i+1].c_str());
            if (newVal > -80) {
                unique_ptr<OGRPoint> tmp2 = maille2OGR(atoi(lineTab[i].c_str()), params.XminA, params.YminA, params.nbmaillesXA, params.pdmA);
                geoMetrics::convertTo(tmp2.get(), dataRef.get(), displayRef);
                antenna->meshes.push_back(pair<unique_ptr<OGRPoint>, double>(std::move(tmp2), newVal));

            }
        }
        antenna->color = Color((rand()%100)/100.0, (rand()%100)/100.0, (rand()%100)/100.0, 1);
        antMapper->AMap->insert(pair<int, Antenna*>(antenna->centroidMesh, std::move(antenna)));
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::milli> time_span = t2 - t1;
    cout << "Loading done. " << "It took me " << time_span.count() << " milliseconds." << endl;
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
unique_ptr<OGRPoint> Vanet::maille2OGR(int meshPos, double Xmin, double Ymin, int nbmaillesX, double pdm) {
    unique_ptr<OGRPoint> pt = make_unique<OGRPoint>();
    double x = (meshPos%nbmaillesX)*pdm + Xmin;
    double y = (meshPos/nbmaillesX)*pdm + Ymin;

    pt->setX(x);
    pt->setY(y);
    return pt;
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
int Vanet::OGR2AntennaMesh(OGRPoint* temp) {
    return params.nbmaillesXA * ((int) ((temp->getY() - params.YminA)/params.pdmA)) + ((int) ((temp->getX() - params.XminA)/params.pdmA));
}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void Vanet::draw() {
}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	t   a time_t
 * \return 	None
 * Display all movements of the vehicle of the fleet.
 * Each vehicle is represented by a stroke corresponding to its last 30 seconds of displacement.
 */
void Vanet::draw(unsigned long long t) {
    // We create a population with the current positions of every vehicle of the fleet


    population.clear();
    unordered_map<string, Vehicule*>::iterator it = this->fleet->fleet.begin();
    for (; it!=this->fleet->fleet.end(); it++) {
        Vehicule* currentVehicle = it->second;
        currentVehicle->color = params.defaultColor;
        currentVehicle->hasCluster = false;
        currentVehicle->radius = params.vehicleRadius;

        std::unique_ptr<OGRPoint> temp = currentVehicle->listTrajectories["0"]->display(t);
        if (temp != nullptr) {
            currentVehicle->setX(temp->getX());
            currentVehicle->setY(temp->getY());
            population.push_back(currentVehicle);
            this->fleet->vehicleGrid->update(currentVehicle, currentVehicle, t);
        }
    }


    if (trackedVehicle != nullptr) {
        Trajectory* currentTraj = trackedVehicle->listTrajectories["0"];

        //start
        glColor4d(0, 1, 0, 0.4); //green
        glPointSize(20.0);
        glBegin(GL_POINTS);
        glVertex3d(currentTraj->trajectory->front()->getX(), currentTraj->trajectory->front()->getY(), 0.0);
        glEnd();

        glLineWidth(10);
        glColor4d(1, 1, 1, 0.6); //09
        glBegin(GL_LINE_STRIP);
        for (unsigned int i=0; i<currentTraj ->trajectory->size(); i++) {
            TemporalGeoPoint* currentPt = currentTraj->trajectory->at(i);
            glVertex3d(currentPt->getX(), currentPt->getY(), 0.0);
        }
        glEnd();

        //end
        glColor4d(1, 0, 0, 0.4); //red
        glPointSize(20.0);
        glBegin(GL_POINTS);
        glVertex3d(currentTraj->trajectory->back()->getX(), currentTraj->trajectory->back()->getY(), 0.0);
        glEnd();

        //current
        glColor4d(1, 1, 0, 0.4); //gold
        glPointSize(20.0);
        glBegin(GL_POINTS);
        glVertex3d(trackedVehicle->getX(), trackedVehicle->getY(), 0.0);
        glEnd();
    }

    //cout << "<<<<<<<< done >>>>>>>>>" << endl;

//    std::unordered_map<int, Antenna*>::iterator itt = antMapper->AMap->begin();
//    for (; itt!=antMapper->AMap->end(); itt++) {
//        Antenna* antenna = itt->second;
//        glColor4d(0, 1, 0, 0.2);
//        glBegin(GL_QUADS);
//            glVertex3d(antenna->getX(), antenna->getY(), 0.0);
//            glVertex3d(antenna->getX(), antenna->getY()+params.pdmA, 0.0);
//            glVertex3d(antenna->getX()+params.pdmA, antenna->getY()+params.pdmA, 0.0);
//            glVertex3d(antenna->getX()+params.pdmA, antenna->getY(), 0.0);
//        glEnd();
//    }


//    int meshPos1 = 304007;
//    double x1 = 938320.4274;
//    double y1 = 2302954.103;
//    //int meshPos2 = 7608034;
//    int meshPos2 = 7604849;

//    double Ymin = 2289885;
//    int Ymin1 = 2289885;


//    cout << std::setprecision(10) << params.XminA << ", " << params.YminA << ", " << params.nbmaillesXA << ", " <<  params.pdmA << endl;
//    unique_ptr<OGRPoint> tmp2 = maille2OGR(meshPos1, params.Xmin, params.Ymin, params.nbmaillesXA, params.pdmA);
//    cout << meshPos2 << " -> " << tmp2->getX() << ", " << tmp2->getY() << endl;

//    unique_ptr<OGRPoint> tmp = make_unique<OGRPoint>(x1, y1);
//    int meshPos = OGR2AntennaMesh(tmp.get());
//    cout << tmp->getX() << ", " << tmp->getY() << " -> " << meshPos << endl;


//    glPointSize(6.0);
//    glColor4d(1, 1, 1, 1);
//    glBegin(GL_POINTS);
//    glVertex3d(tmp->getX(), tmp->getY(), 0.0);
//    glEnd();

//    glPointSize(6.0);
//    glColor4d(0, 0, 1, 1);
//    glBegin(GL_POINTS);
//    glVertex3d(tmp2->getX(), tmp2->getY(), 0.0);
//    glEnd();

//    glPointSize(6.0);
//    glColor4d(1, 0, 0, 1);
//    glBegin(GL_POINTS);
//    glVertex3d(tmp2->getX()+25, tmp2->getY(), 0.0);
//    glEnd();

}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void Vanet::hoverTrackSignal(OGRPoint* location, time_t t) {
    if (!isVehicleSelected) {
        OGRSpatialReference* displayRef = CoordinateSystemManager::getInstance()->getDisplayRef();
        GridMesh* meshPos = this->fleet->vehicleGrid->getMeshFromPoint(location);
        vector<GeographicalObject*> objects = meshPos->getNeighborObjects(true);

        double bestDistance = -1;
        trackedVehicle = nullptr;
        for (unsigned int i=0; i<objects.size(); i++) {
            GeographicalObject* obj = objects[i];
            double distance = geoMetrics::distanceTo(location, obj, displayRef);
            if (bestDistance == -1 || distance <= bestDistance) {
                bestDistance = distance;
                trackedVehicle = (Vehicule*) obj;
            }
        }
    }
}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void Vanet::hoverClickSignal(OGRPoint* location, time_t t) {
    OGRSpatialReference* displayRef = CoordinateSystemManager::getInstance()->getDisplayRef();
    GridMesh* meshPos = this->fleet->vehicleGrid->getMeshFromPoint(location);
    vector<GeographicalObject*> objects = meshPos->getNeighborObjects(true);

    double bestDistance = -1;
    trackedVehicle = nullptr;
    for (unsigned int i=0; i<objects.size(); i++) {
        GeographicalObject* obj = objects[i];
        double distance = geoMetrics::distanceTo(location, obj, displayRef);
        if (bestDistance == -1 || distance <= bestDistance) {
            bestDistance = distance;
            trackedVehicle = (Vehicule*) obj;
        }
    }
    if (trackedVehicle != nullptr) isVehicleSelected = true;
}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void Vanet::hoverReleaseSignal() {
    isVehicleSelected = false;
    trackedVehicle = nullptr;
}

/**
 * \brief 	Default constructor of class Kmeans
 *
 * \param 	None
 * \return 	None
 */
const std::vector<GeographicalObject*>& Vanet::getPopulation() {
    return population;
}
