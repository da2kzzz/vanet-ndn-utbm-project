/**
 * \file	PropagationLayer.h
 */

#ifndef PropagationLayer_h
#define PropagationLayer_h

#include <Scene/singlelayer.h>
#include <layercheckbox.h>
#include <unordered_map>


class Vanet;

class PropagationLayer : public SingleLayer {

    public:
        //*/ -------------------------------------------------
        PropagationLayer(Vanet* parent);
        virtual ~PropagationLayer();

        //*/ -------------------------------------------------
        virtual void draw();
        virtual void draw(unsigned long long t);
        virtual void drawCheckbox(QWidget *tab, QVBoxLayout *layout);

        //*/ -------------------------------------------------
        Vanet* parent;
        time_t currentT;
        time_t nextTime;

};

#endif // PropagationLayer_h
