//IrisbondEnvironment class implementation

#include "IrisHiruEnvironment.h"
#include <Windows.h>
#include <iostream>
#include <typeinfo>
#include <QImage>
#include <QVector3D>
#include <QMatrix3x3>

using namespace std;
bool deviceEnable=false;
std::vector<double> gazeData(8);
std::vector<double> normGaze(2);

IrisHiruEnvironment::IrisHiruEnvironment(QWidget *containerWidget)
    :m_containerWidget(containerWidget)
{

}

IrisHiruEnvironment::~IrisHiruEnvironment()
{
    disableIrisbond();
}

//function used to initialize Irisbond device this will recognize if a device is connected
bool IrisHiruEnvironment::initIrisbond()
{
    IrisbondAPI::START_STATUS status = IrisbondAPI::start();
    if(status==IrisbondAPI::START_STATUS::START_OK)
    {
        cout<<"Eyetracker connected...."<<endl;
        //normalizeGaze();
        deviceEnable=true;
        return true;
    }
    else
    {
        cout<<"Eyetracker not found...."<<endl;
        deviceEnable=false;
        return false;
    }
}

//function used to disconnect the device if it isn´t going to be used in the main window
void IrisHiruEnvironment::disableIrisbond()
{
    IrisbondAPI::stop();
    cout<<"Eyetracker disconnected...."<<endl;
    deviceEnable=false;
}

//boolean function which stores the device status connection
bool IrisHiruEnvironment::hiruEnabled()
{
    return deviceEnable;
}

//function used to enable calibration of the Irisbond device
bool IrisHiruEnvironment::calibrateEyetracker()
{
    IrisbondAPI::showCalibrationGUI(true);
    IrisbondAPI::showCalibrationPointErrorGUI(true);
    IrisbondAPI::showCalibrationResultsGUI(true);
    IrisbondAPI::startCalibration(5);
    IrisbondAPI::waitForCalibrationToEnd(1);
    return false;
}

//function used to call all the data that the Irisbond API provides
void gazePointsData (long timestamp,
                    float mouseX, float mouseY,
                    float mouseRawX, float mouseRawY,
                    int screenWidth, int screenHeight,
                    bool leftEyeDetected, bool rightEyeDetected,
                    int imageWidth, int imageHeight,
                    float leftEyeX, float leftEyeY, float leftEyeSize,
                    float rightEyeX, float rightEyeY, float rightEyeSize,
                    float distanceFactor)
{

    //array positions 1, 2, 3 and 4 has the information needed for the gaze analysis
    gazeData[0] = mouseX;
    gazeData[1] = mouseY;
    gazeData[2] = mouseRawX;
    gazeData[3] = mouseRawY;
    //positions of the eyes relative to the screen workspace
    gazeData[4] = leftEyeX;
    gazeData[5] = leftEyeY;
    gazeData[6] = rightEyeX;
    gazeData[7] = rightEyeY;

}

//function used to call the data of the gaze
void IrisHiruEnvironment::getGazePoints()
{
    //normalizeGaze();
    IrisbondAPI::setDataCallback(IrisbondAPI::DATA_CALLBACK(gazePointsData));
    normalizeGaze();
}

//function used to normalize the gaze data into the main window
void IrisHiruEnvironment::normalizeGaze()
{
    qreal dpr = m_containerWidget->window()->devicePixelRatio();

    QPoint globalGaze(gazeData[2]/dpr,gazeData[3]/dpr);

    QPointF localGaze = m_containerWidget->mapFromGlobal(globalGaze);

    localGaze.setX(localGaze.x() / m_containerWidget->width() * 2 - 1);
    localGaze.setY(-localGaze.y() / m_containerWidget->height() * 2 + 1);

    localGaze.setX(std::max(localGaze.x(), -1.0));
    localGaze.setY(std::max(localGaze.y(), -1.0));
    localGaze.setX(std::min(localGaze.x(), 1.0));
    localGaze.setY(std::min(localGaze.y(), 1.0));

    normGaze[0] = localGaze.x();
    normGaze[1] = localGaze.y();
}

//function that gets the normalize data to store it in a CSV file
std::vector<double> IrisHiruEnvironment::saveData()
{
    //normalizeGaze();
    return normGaze;
}

void streamImage (uintptr_t data, int rows, int cols, int channels, long timestamp)
{

}

void IrisHiruEnvironment::getImage()
{
    IrisbondAPI::setImageCallback(IrisbondAPI::IMAGE_CALLBACK(streamImage));
}

bool IrisHiruEnvironment::userPositioning()
{
    IrisbondAPI::showPositioningWindow();
    //IrisbondAPI::hidePositioningWindow();
    return false;
}
