/**
 * \file	PropagationLayer.cpp
 * \class	PropagationLayer
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	01/01/2018
 * \brief	PropagationLayer is a class holding numerous vehicles
 */

#include <layers/propagationlayer.h>
#include <vanet.h>

using namespace std;


/**
 * \brief 	Default constructor of class PropagationLayer
 *
 * \param 	None
 * \return 	None
 */
PropagationLayer::PropagationLayer(Vanet* parent) : SingleLayer(nullptr) {
    this->parent = parent;
    currentT = -1;
    nextTime = rand()%100+10;
}

/**
 * \brief 	Default destructor of class PropagationLayer
 *
 * \param 	None
 * \return 	None
 */
PropagationLayer::~PropagationLayer() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	None
 * \return 	None
 */
void PropagationLayer::draw() {}

/**
 * \brief 	Inherited method from class Data
 *
 * \param 	t   a time_t
 * \return 	None
 * Displays all the meshes of the environement
 */
void PropagationLayer::draw(unsigned long long t) {

    if (visible) {
        //if (currentT != t) {
            currentT = t;

            unique_ptr<OGRPoint> alterCenterPos = make_unique<OGRPoint>((933225 + 5*2600/2), (2289885 + 5*5250/2), 0);
            glPointSize(6.0);
            glColor4d(1, 1, 1, 1);
            glBegin(GL_POINTS);
            glVertex3d(alterCenterPos->getX(), alterCenterPos->getY(), 0.0);
            glEnd();


            if (t == nextTime) {
                nextTime = t + rand()%50+10;
                AlerteInfo alerteInfo ("Alerte info accident", "vanet", t, t+60);
                this->parent->fleet->sendAlert(alerteInfo, t, alterCenterPos.get());
            }
        //}
        this->parent->fleet->updateAlerts(t);
    } else {
        nextTime = t + rand()%50+10;
    }
}

/*!
 * \brief SingleLayer::drawCheckbox draw the layer checkbox
 * \param tab the tab widget in which to display the checkboxes
 * \param layout the layout in which to add the checkboc (layout is in tab)
 */
void PropagationLayer::drawCheckbox(QWidget *tab, QVBoxLayout *layout) {
    if (tab != nullptr && layout != nullptr) {
        _my_checkbox = new LayerCheckBox(this, QObject::tr("Propagation ?"), tab);
        layout->addWidget(_my_checkbox);
    }
}


