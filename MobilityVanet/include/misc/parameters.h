/**
 * \file	Parameters.h
 */

#ifndef Parameters_h
#define Parameters_h

#include <string>
#include "Common/color.h"


class Parameters  {

    public:
        //*/ -------------------------------------------------
        Parameters();
        virtual ~Parameters();

        void load(std::string file);
        friend std::ostream& operator<<(std::ostream& os, const Parameters& me);

        //*/ -------------------------------------------------
        bool loaded = false;

        double Xmin = 933225;
        double Ymin = 2289885;
        int nbmaillesX = 2910;
        int nbmaillesY = 4000;
        double pdmRoad = 5;

        double XminA = 933225;
        double YminA = 2289875;
        int nbmaillesXA = 582;
        int nbmaillesYA = 800;
        double pdmA = 25;

        double vehicleRadius = 80;
        int tabouDuration = 120;
        int transmissionDuration = 20;

        std::string data_epsg_code = "EPSG:27572";
        Color defaultColor = Color(0.01, 0.6, 0.9, 0.8);
};

#endif // Parameters_h
