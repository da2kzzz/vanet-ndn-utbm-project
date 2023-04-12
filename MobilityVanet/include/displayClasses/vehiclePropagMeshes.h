/**
 * \file	VehiclePropagMeshes.h
 */

#ifndef VehiclePropagMeshes_h
#define VehiclePropagMeshes_h

#include <data/data.h>
#include <geologic-core/objects/geo/untemporal/Grid.h>


class Vanet;
class Fleet;

class VehiclePropagMeshes : public Data {

    public:
        //*/ -------------------------------------------------
        VehiclePropagMeshes(Vanet* parent);
        virtual ~VehiclePropagMeshes();

        //*/ -------------------------------------------------
        void draw();
        void draw(unsigned long long);

        void drawGridMesh(Fleet* fleet, GridMesh* mesh);

        //*/ -------------------------------------------------
        Vanet* parent;
};


#endif // VehiclePropagMeshes_h
