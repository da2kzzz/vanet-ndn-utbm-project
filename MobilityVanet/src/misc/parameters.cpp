/**
 * \file	Parameters.cpp
 * \class	Parameters
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	Parameters is a mesh represention used to quickly retrieve all Vehicle within a certain range
 */

#include "misc/parameters.h"
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QFile>
#include <QObject>
#include <iostream>

using namespace std;

/**
 * \brief 	Default constructor of class AntennaMap
 *
 * \param 	None
 * \return 	None
 */
Parameters::Parameters() {}

/**
 * \brief 	Default constructor of class AntennaMap
 *
 * \param 	None
 * \return 	None
 */
Parameters::~Parameters() {}

/**
 * \brief 	Default constructor of class AntennaMap
 *
 * \param 	None
 * \return 	None
 */
void Parameters::load(string file) {
    QFile f(file.c_str());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QXmlStreamReader reader(&f);
    while (!reader.atEnd() && !reader.hasError()) {
        QXmlStreamReader::TokenType tok = reader.readNext();
        if (tok == QXmlStreamReader::StartElement) {
            if (reader.name().toString() == "bbox") {
                Xmin = reader.attributes().value("xmin").toDouble();
                Ymin = reader.attributes().value("ymin").toDouble();
            } else if (reader.name().toString() == "dataReference") {
                data_epsg_code = reader.attributes().value("refCode").toString().toStdString();
            } else if (reader.name().toString() == "meshes") {
                pdmRoad = reader.attributes().value("meshSize").toDouble();
                nbmaillesX = reader.attributes().value("nbMeshesX").toInt();
                nbmaillesY = reader.attributes().value("nbMeshesY").toInt();
            } else if (reader.name().toString() == "antennas") {
                pdmA = reader.attributes().value("meshSize").toDouble();
                XminA = reader.attributes().value("xmin").toDouble();
                YminA = reader.attributes().value("ymin").toDouble();
                nbmaillesXA = reader.attributes().value("nbMeshesX").toInt();
                nbmaillesYA = reader.attributes().value("nbMeshesY").toInt();
            } else if (reader.name().toString() == "vehicle") {
                vehicleRadius = reader.attributes().value("radius").toDouble();
            } else if (reader.name().toString() == "propagation") {
                transmissionDuration = reader.attributes().value("transmissionDuration").toInt();
                tabouDuration = reader.attributes().value("tabouDuration").toInt();
            } else if (reader.name().toString() == "defaultColor") {
                double red = reader.attributes().value("red").toDouble();
                double green = reader.attributes().value("green").toDouble();
                double blue = reader.attributes().value("blue").toDouble();
                double alpha = reader.attributes().value("alpha").toDouble();
                defaultColor = Color(red, green, blue, alpha);
            }
        }
    }
    loaded = true;
}

ostream& operator <<(ostream& os, const Parameters& me) {
    cout << "Bbox:: Xmin:" << me.Xmin << " Ymin:" << me.Ymin << endl;
    cout << "Data ref EPSG:" << me.data_epsg_code << endl;
    cout << "Meshes:: meshSize:" << me.pdmRoad << " nbMeshesX:" << me.nbmaillesX << " nbMeshesY:" << me.nbmaillesY << endl;
    cout << "Antennas:: meshSize:" << me.pdmA << " nbMeshesX:" << me.nbmaillesXA << " nbMeshesY:" << me.nbmaillesYA << endl;
    cout << "Vehicle:: radius:" << me.vehicleRadius << " transmissionDuration:" << me.transmissionDuration << " tabouDuration:" << me.tabouDuration << endl;
    cout << "DefaultColor::" << me.defaultColor.red << ", " << me.defaultColor.green << ", " << me.defaultColor.blue << ", " << me.defaultColor.alpha << endl;
    return os;
}
