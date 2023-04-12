/**
 * \file	AlerteInfo.cpp
 * \class	AlerteInfo
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	AlerteInfo is a mesh represention used to quickly retrieve all Vehicle within a certain range
 */

#include "misc/alerteinfo.h"

using namespace std;

/**
 * \brief 	Default constructor of class gridmesh
 *
 * \param 	None
 * \return 	None
 */
AlerteInfo::AlerteInfo(string message, string originalSender, time_t emission_time, time_t peremption) {
    this->message = message;
    this->originalSender = originalSender;
    this->emission_time = emission_time;
    this->peremption = peremption;
}

/**
 * \brief 	Default constructor of class gridmesh
 *
 * \param 	None
 * \return 	None
 */
AlerteInfo::AlerteInfo(const AlerteInfo& alerte) {
    this->message = alerte.message;
    this->originalSender = alerte.originalSender;
    this->emission_time = alerte.emission_time;
    this->peremption = alerte.peremption;
}

/**
 * \brief 	Default destructor of class gridmesh
 *
 * \param 	None
 * \return 	None
 */
AlerteInfo::~AlerteInfo() {}

/**
 * \brief 	Default destructor of class gridmesh
 *
 * \param 	None
 * \return 	None
 */
bool AlerteInfo::isEqual(AlerteInfo alerte) {
    return (this->emission_time == alerte.emission_time && this->originalSender == alerte.originalSender && this->message == alerte.message && this->peremption == alerte.peremption);
}

