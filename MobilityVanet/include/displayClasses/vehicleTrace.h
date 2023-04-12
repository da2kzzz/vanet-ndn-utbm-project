/**
 * \file	VehicleTrace.h
 */

#ifndef VehicleTrace_h
#define VehicleTrace_h


#include <data/data.h>

class Vanet;

class VehicleTrace : public Data {

    public:
        //*/ -------------------------------------------------
        VehicleTrace(Vanet* parent);
        virtual ~VehicleTrace();

        //*/ -------------------------------------------------
        void draw();
        void draw(unsigned long long);

        //*/ -------------------------------------------------
        Vanet* parent;
};


#endif // VehicleTrace_h
