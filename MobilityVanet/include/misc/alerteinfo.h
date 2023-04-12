/**
 * \file	AlerteInfo.h
 */

#ifndef ALERTEINFO_H
#define ALERTEINFO_H

#include <ogr_geometry.h>
#include <string>


class AlerteInfo  {

    public:
        //*/ -------------------------------------------------
        AlerteInfo(const AlerteInfo& alerte);
        AlerteInfo(std::string message, std::string originalSender, time_t emission_time, time_t peremption=10);
        virtual ~AlerteInfo();

        bool isEqual(AlerteInfo alerte);

        //*/ -------------------------------------------------
        std::string message;
        std::string originalSender;
        time_t emission_time;
        time_t peremption;
};

#endif // ALERTEINFO_H
