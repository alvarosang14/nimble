#ifndef WINDOW_H
#define WINDOW_H
#include <QCoreApplication>
#include <QtGui>
#include <QMainWindow>
#include <QWidget>
#include <QTabWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <HAPI/AnyHapticsDevice.h>
#include <HAPI/GodObjectRenderer.h>
#include <opencv2/opencv.hpp>

#include "GraphicsWidget.h"
#include "TraceWidget.h"
#include "SmoothPathWidget.h"
#include "HapticEnvironment.h"
#include "HapticButtonEvent.h"
#include "HapticTraceData.h"
#include "Correlation.h"
#include "UDPSender.h"
// AGREGADO
#include "IrisHiruEnvironment.h"
#include "GazeTraceData.h"
#include "GazeTraceWidget.h"
//Exo Data
#include "SensorData.h"
//Force Sensor Data
#include "ForceSensorData.h"

#if 0
  #include "MyGazeEnvironment.h"
  #include "GazeTraceWidget.h"
#endif

struct PatternInfo
{
    PatternInfo():level(0), type(""), location(""){}
    PatternInfo(int level, QString type, QString location):level(level), type(type), location(location){}

    int level;
    QString type;
    QString location;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();
        void Reply(QString str);
        //MainWindow(QObject *parent = nullptr) : QObject(parent){}
        //Obtener el patrón seleccionado
        QString getCurrentPattern();
        void AutoRecording();
        bool getRecording();

    protected:
        bool event(QEvent *event);
        void timerEvent(QTimerEvent *e);
        void keyPressEvent(QKeyEvent* event);

    private slots:
        void loadPattern(int i);
        void setDevice(int i);
        void setGazeOn(int i);
        void setSensorsOn(int i);
        void setForceSensorsOn(int i);
        void setArmOn(int i);
        void setExoOn(int i);
        void setHapticStiffness(int i);
        void setHapticDamping(int i);
        void setHapticFriction(int i);
        void setHapticTunnelForce(int i);
        void loadHapticTunnel(int i);
        void chooseOutputDir();
        void exitApp();
        //void handleStartAnotherPart(); // Nuevo slot para manejar la señal startAnotherPart


    private:
        void createDisplayGroup(const QString &title);
        void createDeviceGroup(const QString &title);
        void createControlGroup(const QString &title);
        void createInertiaControlTab(const QString &title);
        void createRecordingInfoTab(const QString &title);
        void createRecordingStatusGroup(const QString &title);
        void createOutsideXGroup(const QString &title);
        void createOutsideYGroup(const QString &title);

        void recordingAction();
        void hapticTunnelAction();

        std::vector<cv::Point2d> bsplineCurveApprox1(std::vector<cv::Point2d> points);
        std::vector<cv::Point2d> bsplineCurveApprox2(std::vector<cv::Point2d> points, std::vector<cv::Point2d> pathRegionContour);

        void scoreRecording();
        void Send_Distance(HapticTraceEvent evnt);

        QMenu    *m_fileMenu;
        QAction  *m_exitAct;

        //Group Box for display
        QGroupBox *m_displayGroup;
        QComboBox *m_patternCombo;
        //Widget for rendering environmental graphics
        GraphicsWidget *m_gWidget;
        //Widget for rendering haptic trace
        TraceWidget *m_traceWidget;
        #if WIN32
            GazeTraceWidget *m_gazeWidget;
        #endif

        //Widget for rendering smooth path
        SmoothPathWidget *m_pathWidget;
        std::vector<PatternInfo> m_patternInfo;

        //Group Box for devices
        QGroupBox *m_deviceGroup;
        QComboBox *m_hapticCombo;
        QComboBox *m_gazeCombo;
        QComboBox *m_sensorCombo;
        QComboBox *m_forcesensorCombo;
        QComboBox *m_armCombo;
        QComboBox *m_exoCombo;

        //Group Box for controls
        QGroupBox *m_controlGroup;
        QTabWidget *m_controlTabWidget;
        //Widget for inertia controls
        QWidget *m_inertiaControlWidget;
        QSlider *m_stiffnessSlider;
        QSpinBox *m_stiffnessSpin;
        QSlider *m_dampingSlider;
        QSpinBox *m_dampingSpin;
        QSlider *m_frictionSlider;
        QSpinBox *m_frictionSpin;
        QSlider *m_tunnelSlider;
        QSpinBox *m_tunnelSpin;
        QComboBox *m_tunnelCombo;
        //Widget for recording controls
        QWidget *m_recordingControlWidget;
        QLineEdit *m_recordingLocationLine;
        QPushButton *m_recordingLocationButton;
        QLineEdit *m_baseFileNameLine;
        QLineEdit *m_patientIDLine;
        QLineEdit *m_dateLine;
        QLineEdit *m_locationLine;
        QLineEdit *m_classificationGroupLine;
        QLineEdit *m_workspaceLine;
        QLabel *m_recordingStatusLabel;

        //Group Box for recording status
        QGroupBox *m_recordingStatusGroup;

        //Timer used for recording updates
        QBasicTimer *m_timer;

        //Recorded Haptic trace data
        HapticTraceData m_trace;

        //Recorded Exo data
        SensorData m_Sensor;

        //Recorded Force Sensor data
        ForceSensorData m_ForceSensor;

        //Recorded Gaze trace data
        GazeTraceData m_GazeTrace;

        //Current curve for haptic tunnel
        std::vector<HAPI::Vec3> m_curve;

        //Variable that indicates if the app is recording or not ("true" if yes, and "false" if not)
        bool m_isRecording;

        //Variable that indicates when the timer starts
        double timerStart;

        //Last read status of haptic device's button ("true" if the button has been pressed, and "false" if not)
        bool m_prevHapticButtonStatus;

        //Location and filename for trace file
        QDir m_recordingLocation;
        QString m_traceFileName;
        QString m_traceScoreFileName;
        #ifdef WIN32
            QString m_gazeTraceFileName;
        #endif

        //Sensor
        QString m_SensorFileName;
        QString m_ForceSensorFileName;

        //Mats for scoring traces
        cv::Mat m_scoringPathRegionMat;
        cv::Mat m_scoringMidPathMat;

        //Main Widget
        QWidget *m_mainWidget;

        //Haptics Environment
        HapticEnvironment *m_hEnv;

        //Irisbond
        IrisHiruEnvironment *m_irisEnv;

        //Patient counter
        int cont;
        QString actualFileBaseName;

        //Distance Outiside
        QGroupBox *m_outsideXGroup;
        QLabel *m_outsideXLabel;
        QGroupBox *m_outsideYGroup;
        QLabel *m_outsideYLabel;

        UDPSender *udp;
        QString str;

        void UpdateOutsideLabel(double i,double j);

        QString hex_Angles(int i,int j);
        int GetAngleX(int i);
        int GetAngleY(int i);
        //void SendDataInLoop(int i, int j);
        #if 0
            //Gaze
            MyGazeEnvironment *m_gazeEnv;
        #endif
};

//extern int min_x;
//extern int min_y;

#endif // MAIN_WINDOW
