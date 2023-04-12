/**
 * \file	VehiclePosition.h
 */

#ifndef VehiclePosition_h
#define VehiclePosition_h


#include <data/data.h>

class Vanet;

class VehiclePosition : public Data {

    public:
        //*/ -------------------------------------------------
        VehiclePosition(Vanet* parent);
        virtual ~VehiclePosition();

        //*/ -------------------------------------------------
        void draw();
        void draw(unsigned long long);

        //*/ -------------------------------------------------
        Vanet* parent;
};


#endif // VehiclePosition_h
