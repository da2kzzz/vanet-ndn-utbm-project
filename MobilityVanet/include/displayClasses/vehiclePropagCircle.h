/**
 * \file	VehiclePropagCircle.h
 */

#ifndef VehiclePropagCircle_h
#define VehiclePropagCircle_h


#include <data/data.h>

class Vanet;

class VehiclePropagCircle : public Data {

    public:
        //*/ -------------------------------------------------
        VehiclePropagCircle(Vanet* parent);
        virtual ~VehiclePropagCircle();

        //*/ -------------------------------------------------
        void draw();
        void draw(unsigned long long);

        //*/ -------------------------------------------------
        Vanet* parent;
};


#endif // VehiclePropagCircle_h
