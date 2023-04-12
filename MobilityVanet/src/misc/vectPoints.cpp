/**
 * \file	VectPoints.cpp
 * \class	VectPoints
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	VectPoints is a class managing all the antennas of the environement
 */

#include <misc/vectPoints.h>
#include <geologic-core/utils/customToolbox.h>
#include <geologic-core/algorithms/clustering/dbscan/DBscan.h>
#include <geologic-core/objects/dynamic/Trajectory.h>
#include <geologic-core/algorithms/trajectories/Bspline.h>

using namespace std;
using namespace customToolbox;

/**
 * \brief 	Default constructor of class AntennaMap
 *
 * \param 	None
 * \return 	None
 */
VectPoints::VectPoints() : Data("Points") {}

/**
 * \brief 	Default destructor of class AntennaMap
 *
 * \param 	None
 * \return 	None
 */
VectPoints::~VectPoints() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void VectPoints::draw() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	t   a time_t
 * \return 	None
 * Asks all antenas to display
 */
void VectPoints::draw(unsigned long long t) {


}
//    //*/--------------------------------------------------------
//    unique_ptr<Trajectory> traj = make_unique<Trajectory>();
//    int Xmin = 933225 + 5*2100/2;
//    int Ymin = 2289885 + 5*5300/2;

//    traj->addPosition(make_unique<TemporalGeoPoint>(Xmin+100, Ymin+0, 0, 0).get());
//    traj->addPosition(make_unique<TemporalGeoPoint>(Xmin+200, Ymin+200, 0, 0).get());
//    traj->addPosition(make_unique<TemporalGeoPoint>(Xmin+300, Ymin+0, 0, 0).get());
//    traj->addPosition(make_unique<TemporalGeoPoint>(Xmin+400, Ymin-100, 0, 0).get());
//    traj->addPosition(make_unique<TemporalGeoPoint>(Xmin+500, Ymin+0, 0, 0).get());
//    traj->addPosition(make_unique<TemporalGeoPoint>(Xmin+600, Ymin+100, 0, 0).get());
//    traj->dummyFill(1);


//    glLineWidth(5);
//    Color color = Color(0.01, 0.6, 0.9, 1);
//    glColor4d(color.red, color.green, color.blue, 0.8);

//    for (unsigned int i=3; i<=6; i++) {
//    glBegin(GL_LINE_STRIP);

//    Bspline bs;
//    unique_ptr<Trajectory> tempTraj = bs.computeBsplineTrajectory(traj.get(), i);

//    TemporalGeoPoint* lastPt = tempTraj->trajectory->front();
//    for (unsigned int i=0; i<tempTraj->trajectory->size(); i++) {
//        TemporalGeoPoint* currentPt = tempTraj->trajectory->at(i);


//        glVertex3d(lastPt->getX(), lastPt->getY(), 0.0);
//        glVertex3d(currentPt->getX(), currentPt->getY(), 0.0); // Adds the current point to the line strip
//        lastPt = currentPt;
//    }
//    glEnd();
//    }
//}


//    //*/--------------------------------------------------------
//    double radius = 151;
//    string line;
//    ifstream fichierTrace("points.csv");
//    vector<GeographicalObject*> population;
//    while (getline(fichierTrace, line)) {
//        vector<string> lineTab = split(line, ',');
//        double posX = atof(lineTab[1].c_str());
//        double posY = atof(lineTab[2].c_str());

//        if (t<=0) {
//            GeographicalObject* obj = new GeographicalObject(lineTab[0], posX, posY);
//            population.push_back(obj);
//        } else {
//            Color color = colors[lineTab[0]];
//            glPointSize(10.0);
//            glColor4d(color.red, color.green, color.blue, 1); //glColor4d(1.0, 1.0, 1.0, 1.0);
//            glBegin(GL_POINTS);
//            glVertex3d(posX, posY, 0.0);
//            glEnd();

//            float num_segments = 10.0*sqrtf(radius);
//            float theta = 2 * 3.1415926 / num_segments;
//            float co = cosf(theta);//precalculate the sine and cosine
//            float si = sinf(theta);
//            float t;

//            float x = radius;//we start at angle = 0
//            float y = 0;

//            glBegin(GL_LINE_LOOP);
//            for(int ii = 0; ii < num_segments; ii++)  {
//                glVertex2f(x + posX, y + posY);//output vertex
//                //apply the rotation matrix
//                t = x;
//                x = co * x - si * y;
//                y = si * t + co * y;
//            }
//            glEnd();
//        }
//    }

//    if (t<=0) {
//        colors.clear();
//        DBscan algoClustering;
//        unique_ptr<OGRSpatialReference> dataRef = CoordinateSystemManager::getInstance()->getCSbyRef("EPSG:27572");
//        algoClustering.setDataRef(dataRef.get());
//        algoClustering.startDBscan(&population, pair<string, double>("meters", radius), 1);

//        // We get back the clusters and list them
//        unordered_map<string, GeologicCluster*> clusters = algoClustering.getClusters();
//        unordered_map<string, GeologicCluster*>::iterator itc = clusters.begin();
//        for (; itc!=clusters.end(); itc++) {
//            GeologicCluster* ckm = itc->second;
//            cout << ckm->id << " - " << ckm->records.size() << endl;


//            Color color = Color((rand()%100)/100.0, (rand()%100)/100.0, (rand()%100)/100.0, 0.6);
//            if (ckm->id == "cluster::noise") color = Color(0.01, 0.6, 0.9, 0.4);

//            for (unsigned int i=0; i<ckm->records.size(); i++) {
//                GeographicalObject* obj = ckm->records[i];
//                cout << "   -> " << obj->name << endl;
//                colors.insert(pair<string, Color>(obj->name, color));
//            }
//        }
//    }
//    for (unsigned int i=0; i<population.size(); i++) {
//        delete(population[i]);
//        population[i] = nullptr;
//    }
//    population.clear();
//}
