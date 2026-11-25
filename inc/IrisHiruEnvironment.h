//IrisbondEnvironment class
//
//Description: Class to implement irisbond environment irisbondAPI

#ifndef IRISHIRUENVIRONMENT_H
#define IRISHIRUENVIRONMENT_H

#include "IrisbondAPI.h"
#include <QWidget>
#include <Windows.h>
#include <iostream>
#include <typeinfo>

class IrisHiruEnvironment
{
    public:

        IrisHiruEnvironment(QWidget *containerWidget = nullptr);
        ~IrisHiruEnvironment();

        bool initIrisbond();
        void disableIrisbond();
        bool calibrateEyetracker();
        bool userPositioning();
        void getGazePoints();
        bool hiruEnabled();
        std::vector<double> saveData();
        void normalizeGaze();
        void getImage();

    private:

        QWidget *m_containerWidget;
};

#endif // IRISHIRUENVIRONMENT_H
