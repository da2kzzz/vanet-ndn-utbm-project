/**
 * \file	AntennaMap.cpp
 * \class	AntennaMap
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	AntennaMap is a class managing all the antennas of the environement
 */

#include <misc/antennamap.h>


/**
 * \brief 	Default constructor of class AntennaMap
 *
 * \param 	None
 * \return 	None
 */
AntennaMap::AntennaMap() : Data("Coverage") {
   AMap = new std::unordered_map<int, Antenna*>();
}

/**
 * \brief 	Default destructor of class AntennaMap
 *
 * \param 	None
 * \return 	None
 */
AntennaMap::~AntennaMap() {
    std::unordered_map<int, Antenna*>::iterator it = AMap->begin();
    for (; it!=AMap->end(); it++) delete(it->second);
    AMap->clear();
    delete(AMap);
    AMap = NULL;
}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void AntennaMap::draw() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	t   a time_t
 * \return 	None
 * Asks all antenas to display
 */
void AntennaMap::draw(unsigned long long t) {
    std::unordered_map<int, Antenna*>::iterator it = AMap->begin();
    for (; it!=AMap->end(); it++) {
        Antenna* antenna = it->second;
        antenna->draw(t);
    }
}
