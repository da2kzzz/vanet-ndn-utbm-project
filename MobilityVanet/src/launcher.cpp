/**
 * \file	Launcher.cpp
 * \class	Launcher
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	Launcher is the main class of this project
 */

#include <exception>
#include <iostream>
#include <cmath>

#include "geologicapplication.h"
#include "mainwindow.h"

#include <Common/config.h>
#include <Scene/singlelayer.h>
#include <Scene/compositelayer.h>
#include <layercheckbox.h>
#include <QVBoxLayout>
#include <zones.h>
#include <data/tilegrid.hpp>
#include <glrastertile.h>
#include <glvectortile.h>
#include <nominatim.h>
#include <buildings.h>

#include <memory>
#include <QNetworkProxy>

#include <unordered_map>
#include "vanet.h"

#include "geologic-core/algorithms/geoMetrics/geoMetrics.h"
#include "geologic-core/core/geologic-launcher/GPXReader.h"
#include "geologic-core/core/geologic-launcher/GeoJsonReader.h"
#include "geologic-core/algorithms/trajectories/TrajectoryManager.h"
#include <geologic-core/algorithms/clustering/kmeans/Kmeans.h>
#include <geologic-core/algorithms/clustering/dbscan/DBscan.h>
#include <geologic-core/algorithms/clustering/hierarchical/Hierarchical.h>



#include <osmrouteloader.h>
#include <geologic-core/objects/urban/roads.h>
#include <geologic-core/graphe/Edge.h>
#include <geologic-core/graphe/Node.h>


using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]) {


    glutInit(&argc, argv);

    GeologicModule* module = GeologicModule::getInstance();
    module->getRoleName();

    GeologicApplication a(argc, argv);
    MainWindow* t = a.getMainWindow();
    //MainWindow* p = new MainWindow();
    //a.setMainWindow(p);

    //*/
    if (a.getConfig().proxy_enabled) { // If configured, set an application wide network proxy
        QNetworkProxy proxy(QNetworkProxy::HttpProxy, a.getConfig().proxy_address, a.getConfig().proxy_port);
        QNetworkProxy::setApplicationProxy(proxy);
    }
    //*/
    t->show();

    //*/
    // Preliminary: get pointers to useful application variables
    CoordinateSystemManager* geoReferences = CoordinateSystemManager::getInstance();
    OGRSpatialReference* dataref = geoReferences->getDataRef();
    OGRSpatialReference* viewref = geoReferences->getViewRef();
    MovableView* v = a.getMainWindow()->getGlPanel()->getMainView();
    QTabWidget* tabs = a.getMainWindow()->getTabs();

    // Build the layers checkbox toggle panel and its layout
    QWidget* ctrl_panel = new QWidget(a.getMainWindow());
    QVBoxLayout* tab_layout = new QVBoxLayout();
    ctrl_panel->setLayout(tab_layout);


    //VectPoints* VM = new VectPoints();
    Vanet* VM = new Vanet(v);
    VM->geomap = GeologicMapper::getInstance();
    VM->loadmobility();
    VM->vehiculeTraj();
    VM->loadAntennas();

    // Start adding data sources
    // Tile grid with OSM vectorial data
//    TileGrid<GLVectorTile> *tiles_v = new TileGrid<GLVectorTile>{"Vector tiles", v, dataref, viewref, a.getConfig().overpass_url_base};
//    auto tiles_v_layer = make_unique<Layer>(tiles_v);
//    LayerCheckBox *chk_tiles_v = new LayerCheckBox(tiles_v_layer.get(), QObject::tr("Données routières"), ctrl_panel);
//    tab_layout->addWidget(chk_tiles_v);
//    v->addLayer(move(tiles_v_layer));

    // Tile grid with OSM raster data

    auto background_layer = make_unique<CompositeLayer>();
    background_layer->setTitle("Fonds de cartes");

    // Tile grid with OSM raster data
    TileGrid<GLRasterTile> *tiles = new TileGrid<GLRasterTile>{"Raster tiles", v, dataref, viewref, a.getConfig().tile_url_base};
    auto tiles_layer = make_unique<SingleLayer>(tiles);
    background_layer->addChild(tiles_layer.get());
    v->addLayer(move(tiles_layer));


//    auto vp_layer = make_unique<Layer>(VM);
//    LayerCheckBox *chk_vp = new LayerCheckBox(vp_layer.get(), QObject::tr("Vehicle positions"), ctrl_panel);
//    tab_layout->addWidget(chk_vp);
//    v->addLayer(move(vp_layer));



    v->registerManager(VM);
    auto vanet_layer = make_unique<SingleLayer>(VM);
    v->addLayer(move(vanet_layer));

    auto fleet_layer = make_unique<CompositeLayer>();
    fleet_layer->setTitle("Flotte de véhicules");


//    auto chk_propag = make_unique<SingleLayer>(VM->propagManager.get()); //, QObject::tr("Propagation ?"), ctrl_panel);
//    fleet_layer->addChild(chk_propag.get());
//    v->addLayer(move(chk_propag));


    fleet_layer->addChild(VM->clusterManager.get());
    v->addLayer(move(VM->clusterManager));

    auto vp_layer = make_unique<SingleLayer>(VM->vehiclePosition.release());
    fleet_layer->addChild(vp_layer.get());
    v->addLayer(move(vp_layer));

    auto vt_layer = make_unique<SingleLayer>(VM->vehicleTrace.release());
    fleet_layer->addChild(vt_layer.get());
    //vt_layer->getCheckBox()->click();
    v->addLayer(move(vt_layer));

    auto vpc_layer = make_unique<SingleLayer>(VM->vehiclePropagCircle.release());
    fleet_layer->addChild(vpc_layer.get());
    v->addLayer(move(vpc_layer));

    auto vpm_layer = make_unique<SingleLayer>(VM->vehiclePropagMeshes.release());
    fleet_layer->addChild(vpm_layer.get());
    //chk_vpm->click();
    v->addLayer(move(vpm_layer));


    auto cl_layer = make_unique<SingleLayer>(VM->clusterLinks.release());
    fleet_layer->addChild(cl_layer.get());
    v->addLayer(move(cl_layer));


/*/
    //ok
    auto testCluster_layer = make_unique<SingleLayer>(VM->clusterLinks.release());
    fleet_layer->addChild(testCluster_layer.get());

    v->addLayer(move(testCluster_layer));

/*/


    // Finalize the checkboxes panel: add a stretch, and add it to the window.
    tab_layout->addStretch();
    tabs->addTab(ctrl_panel, QObject::tr("Couches"));

    // Add an OSM nominatim panel for geocoded search
    NominatimPanel *nom_panel = new NominatimPanel(ctrl_panel, a.getMainWindow()->getGlPanel());
    tabs->addTab(nom_panel, QObject::tr("Search"));

    background_layer->drawCheckbox(tabs, tab_layout);
    fleet_layer->drawCheckbox(tabs, tab_layout);

//    t->setStopTime(QTime(1, 0, 0, 0));
//    t->setTimeScale(1);
//    t->playPauseSwap();

    return a.exec();
}
