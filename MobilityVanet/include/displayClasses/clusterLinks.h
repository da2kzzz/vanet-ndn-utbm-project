/**
 * \file	ClusterLinks.h
 */

#ifndef ClusterLinks_h
#define ClusterLinks_h


#include <data/data.h>
#include "layers/clustermanagerlayer.h"

class Vanet;

class ClusterLinks : public Data {

    public:
        //*/ -------------------------------------------------
        ClusterLinks(ClusterManagerLayer* parent);
        virtual ~ClusterLinks();

        //*/ -------------------------------------------------
        void draw();
        void draw(unsigned long long t);

        //*/ -------------------------------------------------
        ClusterManagerLayer* parent;
};


#endif // ClusterLinks_h
