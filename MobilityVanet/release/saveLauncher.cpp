    /*/
    TemporalGeoPoint* tgp1 = new TemporalGeoPoint(934700, 2298038, 0, 0);
    TemporalGeoPoint* tgp2 = new TemporalGeoPoint(934700, 2307470, 0, 100);
    TemporalGeoPoint* tgp3 = new TemporalGeoPoint(943637, 2307470, 0, 200);
    TemporalGeoPoint* tgp4 = new TemporalGeoPoint(943637, 2298038, 0, 300);
    TemporalGeoPoint* tgp5 = new TemporalGeoPoint(934700, 2298038, 0, 400);
    //Trajectory* tt = new Trajectory();
    //tt->addPosition(tgp1); tt->addPosition(tgp2); tt->addPosition(tgp3); tt->addPosition(tgp4); tt->addPosition(tgp5);

    OGRSpatialReference* lambertII = CoordinateSystemManager::getInstance()->getCSbyName("Lambert II etendu");
    OGRSpatialReference* wgs84 = CoordinateSystemManager::getInstance()->getCSbyName("WGS 84");

    string gpxfile = "./trace1.gpx"; //carteGPX
    GPXReader* gpxr = GPXReader::getInstance();
    gpxr->load(gpxfile, lambertII, lambertII);

    string geojsonfile = "./carteGeoJson.geojson";
    GeoJsonReader* gjr = GeoJsonReader::getInstance();
    gjr->load(geojsonfile, wgs84, lambertII);

    MobileObject* mobile = gjr->getMobile();

    Trajectory* tt = mobile->listTrajectories["0"];
    cout << "size:: " << tt->trajectory->size() << endl;
    tt->dummyFill(10);
    TrajectoryManager trajman;
    //Trajectory* ttt = trajman.cleanTrajectory(tt, 2, lambertII);
    trajman.cleanTrajectory(*tt, 2, lambertII);
    cout << "size:: " << tt->size() << endl;

    Fleet* fleet = new Fleet();
    fleet->listTrajectories.insert(pair<string, Trajectory*>("tt", tt));
    fleet->colors.insert(pair<string, Color>("tt", Color((rand()%100)/100.0, (rand()%100)/100.0, (rand()%100)/100.0, 1)));//0.4
    auto pt_layer = make_unique<Layer>(fleet); //VM->fleet


    /*/
    auto pt_layer = make_unique<Layer>(VM->fleet);
    VM->fleet->AMAP = VM->antMapper;
    //*/

    LayerCheckBox *chk_pt = new LayerCheckBox(pt_layer.get(), QObject::tr("Fleet"), ctrl_panel);
    tab_layout->addWidget(chk_pt);
    v->addLayer(move(pt_layer));

    /*/
    auto ant_layer = make_unique<Layer>(VM->antMapper);
    LayerCheckBox *chk_ant = new LayerCheckBox(ant_layer.get(), QObject::tr("Coverage"), ctrl_panel);
    tab_layout->addWidget(chk_ant);
    v->addLayer(move(ant_layer));
    //*/

//    auto mesh_layer = make_unique<Layer>(VM->gmm);
//    LayerCheckBox *chk_mesh = new LayerCheckBox(mesh_layer.get(), QObject::tr("Meshes"), ctrl_panel);
//    tab_layout->addWidget(chk_mesh);
//    v->addLayer(move(mesh_layer));














    //We first read a document where we have clustering issues
    VectPoints* mapPoints = new VectPoints();
    string line;
    ifstream fichierTrace("points.csv");
    while (getline(fichierTrace, line)) {
        vector<string> lineTab = split(line, ',');

        string id = lineTab[0];
        int x = atoi(lineTab[1].c_str());
        int y = atoi(lineTab[2].c_str());
        GeographicalObject* pt = new GeographicalObject(id, x, y);
        mapPoints->points.push_back(pt);
    }




    /*/
    Kmeans algoClustering;
    algoClustering.setDataRef(CoordinateSystemManager::getInstance()->getCSbyRef("EPSG:27572"));
    algoClustering.percStill = 0.001;
    algoClustering.startKmeans(&(mapPoints->points), 6);
    /*/
    DBscan algoClustering;
    algoClustering.setDataRef(CoordinateSystemManager::getInstance()->getCSbyRef("EPSG:27572"));
    algoClustering.startDBscan(&(mapPoints->points), 0.04, 2);
    /*/
    Hierarchical algoClustering;
    algoClustering.setDataRef(CoordinateSystemManager::getInstance()->getCSbyRef("EPSG:27572"));
    algoClustering.startHCA(&(mapPoints->points), 8);


    //*/
    unordered_map<string, GeologicCluster*> clusters = algoClustering.getClusters();
    unordered_map<string, GeologicCluster*>::iterator it = clusters.begin();
    cout << "" << endl;
    for (; it!=clusters.end(); it++) {
        GeologicCluster* ckm = it->second;

        Color c = Color((rand()%100)/100.0, (rand()%100)/100.0, (rand()%100)/100.0, 0.6);
        if (ckm->id == "cluster::noise") c = Color(0.9, 0.9, 0.9, 0.4);

        pair<vector<GeographicalObject*>, Color> p(ckm->records, c);
        mapPoints->clusters.insert(pair<string, pair<vector<GeographicalObject*>, Color> >(it->first, p));
    }

    //kmeans.startKmeans(0, NULL);
    //*/
    auto points_layer = make_unique<Layer>(mapPoints);
    LayerCheckBox *chk_points = new LayerCheckBox(points_layer.get(), QObject::tr("Points"), ctrl_panel);
    tab_layout->addWidget(chk_points);
    v->addLayer(move(points_layer));
    //*/


    //algoClustering.~Kmeans();

