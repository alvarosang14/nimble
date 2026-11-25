//Include Qt Widgets standard files
//To line up widgets vertically
#include <QVBoxLayout>
//Horizontal menu bar
#include <QMenuBar>
//Menu Widget for use in menu bars, context menus, and other popup menus
#include <QMenu>
//Dialog that allow users to select files or directories
#include <QFileDialog>
//Modal dialog for informing the user or for asking the user a question and receiving an answer
#include <QMessageBox>

//Include Qt GUI standard files
//Abstraction for user commands that can be added to different user interface components
#include <QAction>

//Include Qt Core standard files
//Clock time functions
#include <QTime>

#include <QDebug>

// Include
#include <QCoreApplication>
#include <QObject>
//Include the user-defined needed files
#include "MainWindow.h"
#include "PatternFilters.h"
#include "tinysplinecpp.h"
#include "BSplineSolverFunctions.h"
//UDP Connection Receiver for the exoskeleton
#include "UDPReceiver.h"
#include "UDPSender.h"

#include "GraphicsWidget.h"

//When we start the application is:
//Not recording, the Haptic Button is off, and the Haptic Environment is empty
MainWindow::MainWindow(): m_isRecording(false), timerStart(0), m_prevHapticButtonStatus(false), m_hEnv(0)
{

    //connect signal

    //Set Window title
    setWindowTitle(tr("EYE-HAND COORDINATION TEST"));
    //Set minimum size of the window in pixels (width, height)
    qreal  dpr = window()->devicePixelRatio();
    setMinimumSize(1000/dpr,800/dpr);

    //Initialize haptic environment
    m_hEnv = new HapticEnvironment();

    //Set timer
    m_timer = new QBasicTimer();

    //Graphical User Interface "GUI" Components (parent)
    m_mainWidget = new QWidget(this);

    //std::cout << "Device Pixel Ratio: " << m_mainWidget->devicePixelRatio() << std::endl; //https://doc.qt.io/qt-6/highdpi.html

    //Create actions for menu
    m_exitAct = new QAction("&Exit", this);
    //Connect action to Exit the app
    connect(m_exitAct, &QAction::triggered, this, &MainWindow::exitApp);

    //Add menu to menubar
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    //m_fileMenu->addSeparator(); Add Exit Action to the widget's list of actions
    m_fileMenu->addAction(m_exitAct);

    //GroupBox for Display
    createDisplayGroup(tr("Display"));

    //Initialize gaze environment
    m_irisEnv = new IrisHiruEnvironment(m_gazeWidget);

    //GroupBox for Selecting Device
    createDeviceGroup(tr("Devices"));

    //GroupBox for Controls
    createControlGroup(tr("Controls"));

    //GroupBox for Recording Status
    createRecordingStatusGroup(tr("Recording Status"));

    //GroupBox for Distance outside Borders
    createOutsideXGroup(tr("X Distance"));
    createOutsideYGroup(tr("Y Distance"));

    //Add components to main widget
    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(m_displayGroup, 0, 0, 10, 15);
    mainLayout->addWidget(m_deviceGroup, 0, 15, 3, 5);
    mainLayout->addWidget(m_controlTabWidget, 3, 15, 6, 5);
    mainLayout->addWidget(m_recordingStatusGroup, 9, 15, 1, 1);
    //mainLayout->addWidget(m_exoStatusGroup)
    mainLayout->addWidget(m_outsideXGroup,9,18,1,1);
    mainLayout->addWidget(m_outsideYGroup,9,19,1,1);
    m_mainWidget->setLayout(mainLayout);

    //The widget accepts focus by clicking
    m_mainWidget->setFocusPolicy(Qt::ClickFocus);

    //Set the Main Widget to be the main window's central widget
    setCentralWidget(m_mainWidget);

    //10 millisecond loop for the Widget´s timeout
    m_timer->start(10, this);

    //First patient
    cont = 1;

}

//Define the destructor of the main window
MainWindow::~MainWindow()
{
    //If timer is running
    if(m_timer->isActive())
    {
        //Stop the timer
        m_timer->stop();
    }

    //Delete all the created objects
    delete m_gWidget;
    delete m_patternCombo;
    delete m_hapticCombo;
    delete m_hEnv;
    delete m_traceWidget;
    delete m_pathWidget;
    delete m_irisEnv;
}

//Create the Display Group Box
void MainWindow::createDisplayGroup(const QString &title)
{
    //Create new Display Group Box
    m_displayGroup = new QGroupBox(title);

    //User-visible string
    QLabel *patternLabel = new QLabel(tr("Pattern:"));

    //Load info for each pattern
    m_patternInfo.push_back(PatternInfo());
    m_patternInfo.push_back(PatternInfo(1, tr("Complexity"), tr("patterns/complexity/comp1.bmp")));
    m_patternInfo.push_back(PatternInfo(2, tr("Complexity"), tr("patterns/complexity/comp2.bmp")));
    m_patternInfo.push_back(PatternInfo(3, tr("Complexity"), tr("patterns/complexity/comp3.bmp")));
    m_patternInfo.push_back(PatternInfo(1, tr("Straight to Curvy"),
                                        tr("patterns/straight_to_curvy/stc1.bmp")));
    m_patternInfo.push_back(PatternInfo(2, tr("Straight to Curvy"),
                                        tr("patterns/straight_to_curvy/stc2.bmp")));
    m_patternInfo.push_back(PatternInfo(3, tr("Straight to Curvy"),
                                        tr("patterns/straight_to_curvy/stc3.bmp")));
    m_patternInfo.push_back(PatternInfo(1, tr("Width"), tr("patterns/width/wid1.bmp")));
    m_patternInfo.push_back(PatternInfo(2, tr("Width"), tr("patterns/width/wid2.bmp")));
    m_patternInfo.push_back(PatternInfo(3, tr("Width"), tr("patterns/width/wid3.bmp")));

    //Combobox for selecting pattern (list of options to the user)
    m_patternCombo = new QComboBox();
    m_patternCombo->addItem(tr("None"));
    m_patternCombo->addItem(tr("Complexity: pattern 1"), m_patternInfo[1].location);
    m_patternCombo->addItem(tr("Complexity: pattern 2"), m_patternInfo[2].location);
    m_patternCombo->addItem(tr("Complexity: pattern 3"), m_patternInfo[3].location);
    m_patternCombo->addItem(tr("Straight to Curvy: pattern 1"), m_patternInfo[4].location);
    m_patternCombo->addItem(tr("Straight to Curvy: pattern 2"), m_patternInfo[5].location);
    m_patternCombo->addItem(tr("Straight to Curvy: pattern 3"), m_patternInfo[6].location);
    m_patternCombo->addItem(tr("Width: pattern 1"), m_patternInfo[7].location);
    m_patternCombo->addItem(tr("Width: pattern 2"), m_patternInfo[8].location);
    m_patternCombo->addItem(tr("Width: pattern 3"), m_patternInfo[9].location);


    //Graphics Widget for rendering pattern and haptic stylus graphics
    m_gWidget = new GraphicsWidget(m_hEnv, this);

    //Trace Widget for rendering traces during recording
    m_traceWidget = new TraceWidget();

#if WIN32
    //Object that contains the widgets gaze trace for the main window
    m_gazeWidget = new GazeTraceWidget();
#endif

    //Smooth Path Widget for rendering smooth path tunnel when selected
    m_pathWidget = new SmoothPathWidget();

    //Connection between Pattern Combo and Load Pattern
    connect(m_patternCombo,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::loadPattern);

    //Divide the space into rows and columns, and puts each widgets it manages into the correct cells
    QGridLayout *displayLayout = new QGridLayout();
    displayLayout->addWidget(patternLabel, 0, 0, 1, 1);
    displayLayout->addWidget(m_patternCombo, 0, 1, 1, 5);
    displayLayout->addWidget(m_gWidget, 1, 0, 1, 6);
    //Overlap m_gWidget
    displayLayout->addWidget(m_pathWidget, 1, 0, 1, 6);
    //Overlap m_gWidget
    displayLayout->addWidget(m_traceWidget, 1, 0, 1, 6);
#if WIN32
    //Overlap m_gazeWidget
    displayLayout->addWidget(m_gazeWidget, 1, 0, 1, 6);
#endif

    //Display Layout
    m_displayGroup->setLayout(displayLayout);
    //Maximizes the window
    this->setWindowState(Qt::WindowMaximized);

    //Not Editable Pattern Combo by the user
    m_patternCombo->setEditable(false);
    //Widget does not accept focus in Pattern Combo
    m_patternCombo->setFocusPolicy(Qt::NoFocus);
}

//Create the Device Group Box
void MainWindow::createDeviceGroup(const QString &title)
{
    //Create new Device Group Box
    m_deviceGroup = new QGroupBox(title);

    //User-visible string
    QLabel *deviceLabel = new QLabel(tr("Select Haptic Device:"));

    //Combobox for selecting haptic device (list of options to the user)
    m_hapticCombo = new QComboBox();
    m_hapticCombo->addItem(tr("Mouse"), false);
    m_hapticCombo->addItem(tr("Haptic Device"), true);

    //Connection between Haptic Combo and the set of the Haptic Device
    connect(m_hapticCombo,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::setDevice);

    //User-visible string
    QLabel *gazeLabel = new QLabel(tr("Gaze Tracker On:"));

    //Combobox for selecting gaze tracker function (list of options to the user)
    m_gazeCombo = new QComboBox();
    m_gazeCombo->addItem(tr("No"), false);
    m_gazeCombo->addItem(tr("Yes"), true);

    //Connection between Gaze Combo and set the Gaze on
    connect(m_gazeCombo,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::setGazeOn);

    //User-visible string
    QLabel *sensorLabel = new QLabel(tr("Inertial Sensors On:"));

    //Combobox for selecting inertial sensors function (list of options to the user)
    m_sensorCombo = new QComboBox();
    m_sensorCombo->addItem(tr("No"), false);
    m_sensorCombo->addItem(tr("Yes"), true);

    //Connection between Sensor Combo and set the Sensor on
    connect(m_sensorCombo,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::setSensorsOn);

    //User-visible string
    QLabel *forcesensorLabel = new QLabel(tr("Force Sensors On:"));

    //Combobox for selecting force sensors function (list of options to the user)
    m_forcesensorCombo = new QComboBox();
    m_forcesensorCombo->addItem(tr("No"), false);
    m_forcesensorCombo->addItem(tr("Yes"), true);

    //Connection between Sensor Combo and set the Sensor on
    connect(m_forcesensorCombo,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::setForceSensorsOn);

    //User-visible string
    QLabel *armLabel = new QLabel(tr("Select arm:"));

    //Combobox for selecting arm (list of options to the user)
    m_armCombo = new QComboBox();
    m_armCombo->addItem(tr("Right arm"), false);
    m_armCombo->addItem(tr("Left arm"), true);

    //Connection between Arm Combo and set the Arm
    connect(m_armCombo,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::setArmOn);

    //User-visible string
    QLabel *exoLabel = new QLabel(tr("Exo On:"));

    //Combobox for selecting haptic device (list of options to the user)
    m_exoCombo = new QComboBox();
    m_exoCombo->addItem(tr("No"), false);
    m_exoCombo->addItem(tr("Yes"), true);

    //Connection between Exo Combo and set the Exo on
    connect(m_exoCombo,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::setExoOn);

    //Divide the space into rows and columns, and puts each widgets it manages into the correct cells
    QGridLayout *deviceLayout = new QGridLayout();
    deviceLayout->addWidget(deviceLabel, 0, 0, 1, 2);
    deviceLayout->addWidget(m_hapticCombo, 0, 2, 1, 3);
    deviceLayout->addWidget(gazeLabel, 1, 0, 1, 2);
    deviceLayout->addWidget(m_gazeCombo, 1, 2, 1, 3);
    deviceLayout->addWidget(sensorLabel, 2, 0, 1, 2);
    deviceLayout->addWidget(m_sensorCombo, 2, 2, 1, 3);
    deviceLayout->addWidget(forcesensorLabel, 3, 0, 1, 2);
    deviceLayout->addWidget(m_forcesensorCombo, 3, 2, 1, 3);
    deviceLayout->addWidget(armLabel, 4, 0, 1, 2);
    deviceLayout->addWidget(m_armCombo, 4, 2, 1, 3);
    deviceLayout->addWidget(exoLabel, 5, 0, 1, 2);
    deviceLayout->addWidget(m_exoCombo, 5, 2, 1, 3);

    //Display Layout
    m_deviceGroup->setLayout(deviceLayout);

    //Not Editable Haptic Combo by the user
    m_hapticCombo->setEditable(false);
    //Widget does not accept focus in Haptic Combo
    m_hapticCombo->setFocusPolicy(Qt::NoFocus);
}

//Create the Control Group Box
void MainWindow::createControlGroup(const QString &title)
{
    //Create new Control Group Box
    m_controlGroup = new QGroupBox(title);

    //Stack of tabbed control widgets
    m_controlTabWidget = new QTabWidget();

    //Inertia Control Tab
    createInertiaControlTab(tr("Inertia"));

    //Recording Info Tab
    createRecordingInfoTab(tr("Recording"));
}

//Inertia Control Tab
void MainWindow::createInertiaControlTab(const QString &title)
{
    //Graphical User Interface "GUI" Components (child)
    m_inertiaControlWidget = new QWidget();

    //User-visible string
    QLabel *hapticStiffnessLabel = new QLabel(tr("Haptic Stiffness (%):"));

    //Spin Box for Stiffness
    m_stiffnessSpin = new QSpinBox();
    m_stiffnessSpin->setRange(0, 100);
    m_stiffnessSpin->setSingleStep(1);

    //Horizontal slider for Stiffness
    m_stiffnessSlider = new QSlider(Qt::Horizontal);
    m_stiffnessSlider->setTickPosition(QSlider::TicksBothSides);
    m_stiffnessSlider->setTickInterval(10);
    m_stiffnessSlider->setSingleStep(1);

    //User-visible string
    QLabel *hapticDampingLabel = new QLabel(tr("Haptic Damping (%):"));

    //Spin Box for Damping
    m_dampingSpin = new QSpinBox();
    m_dampingSpin->setRange(0, 100);
    m_dampingSpin->setSingleStep(1);

    //Horizontal slider for Damping
    m_dampingSlider = new QSlider(Qt::Horizontal);
    m_dampingSlider->setTickPosition(QSlider::TicksBothSides);
    m_dampingSlider->setTickInterval(10);
    m_dampingSlider->setSingleStep(1);

    //User-visible string
    QLabel *hapticFrictionLabel = new QLabel(tr("Haptic Friction (%):"));

    //Spin Box for Friction
    m_frictionSpin = new QSpinBox();
    m_frictionSpin->setRange(0, 100);
    m_frictionSpin->setSingleStep(1);

    //Horizontal slider for Friction
    m_frictionSlider = new QSlider(Qt::Horizontal);
    m_frictionSlider->setTickPosition(QSlider::TicksBothSides);
    m_frictionSlider->setTickInterval(10);
    m_frictionSlider->setSingleStep(1);

    //User-visible string
    QLabel *hapticTunnelLabel = new QLabel(tr("Haptic Tunnel Force (%):"));

    //Spin Box for Tunnel Force
    m_tunnelSpin = new QSpinBox();
    m_tunnelSpin->setRange(0, 100);
    m_tunnelSpin->setSingleStep(1);

    //Horizontal slider for Tunnel Force
    m_tunnelSlider = new QSlider(Qt::Horizontal);
    m_tunnelSlider->setTickPosition(QSlider::TicksBothSides);
    m_tunnelSlider->setTickInterval(10);
    m_tunnelSlider->setSingleStep(1);
    m_tunnelSlider->setRange(0, 100);

    //Combobox for selecting Tunnel options (list of options to the user)
    m_tunnelCombo = new QComboBox();
    m_tunnelCombo->addItem("Center Path Tunnel", 0);
    m_tunnelCombo->addItem("Smooth Path Tunnel", 1);

    //Connection for Stiffness
    connect(m_stiffnessSpin,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::setHapticStiffness);

    connect(m_stiffnessSpin,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            m_stiffnessSlider, &QSlider::setValue);

    connect(m_stiffnessSlider, &QSlider::valueChanged,
            m_stiffnessSpin, &QSpinBox::setValue);

    //Connection for Damping
    connect(m_dampingSpin,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::setHapticDamping);

    connect(m_dampingSpin,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            m_dampingSlider, &QSlider::setValue);

    connect(m_dampingSlider, &QSlider::valueChanged, m_dampingSpin,
            &QSpinBox::setValue);

    //Connection for Friction
    connect(m_frictionSpin,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::setHapticFriction);

    connect(m_frictionSpin,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            m_frictionSlider, &QSlider::setValue);

    connect(m_frictionSlider, &QSlider::valueChanged, m_frictionSpin,
            &QSpinBox::setValue);

    //Connection for Tunnel Force
    connect(m_tunnelSpin,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::setHapticTunnelForce);

    connect(m_tunnelSpin,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            m_tunnelSlider, &QSlider::setValue);

    connect(m_tunnelSlider, &QSlider::valueChanged, m_tunnelSpin,
            &QSpinBox::setValue);

    //Connection for Tunnel Force Combobox
    connect(m_tunnelCombo,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::loadHapticTunnel);

    //Divide the space into rows and columns, and puts each widgets it manages into the correct cells
    QGridLayout *controlLayout = new QGridLayout();
    controlLayout->addWidget(hapticStiffnessLabel, 0, 0, 1, 1);
    controlLayout->addWidget(m_stiffnessSpin, 0, 1, 1, 1);
    controlLayout->addWidget(m_stiffnessSlider, 1, 0, 1, 2);
    controlLayout->addWidget(hapticDampingLabel, 2, 0, 1, 1);
    controlLayout->addWidget(m_dampingSpin, 2, 1, 1, 1);
    controlLayout->addWidget(m_dampingSlider, 3, 0, 1, 2);
    controlLayout->addWidget(hapticFrictionLabel, 4, 0, 1, 1);
    controlLayout->addWidget(m_frictionSpin, 4, 1, 1, 1);
    controlLayout->addWidget(m_frictionSlider, 5, 0, 1, 2);
    controlLayout->addWidget(hapticTunnelLabel, 6, 0, 1, 1);
    controlLayout->addWidget(m_tunnelSpin, 6, 1, 1, 1);
    controlLayout->addWidget(m_tunnelSlider, 7, 0, 1, 2);
    controlLayout->addWidget(m_tunnelCombo, 8, 0, 1, 2);

    //Display Layout
    m_inertiaControlWidget->setLayout(controlLayout);

    //Default values
    m_stiffnessSpin->setValue(50);
    m_dampingSpin->setValue(10);
    m_frictionSpin->setValue(10);
    m_tunnelSpin->setValue(0);

    //Adds a tab with the Inertia control Widget page and label to the tab widget
    m_controlTabWidget->addTab(m_inertiaControlWidget, title);
}

//Recording Info Tab
void MainWindow::createRecordingInfoTab(const QString &title)
{
    //New Recording Control Widget
    m_recordingControlWidget = new QWidget();

    //User-visible string
    QLabel *outputLabel = new QLabel(tr("Output Location:"));
    //User enter and edit a single line of plain text (Output location)
    m_recordingLocationLine = new QLineEdit();
    //User can browse through the system to locate the Recording Info
    m_recordingLocationButton = new QPushButton(tr("Browse"));

    //User-visible string
    QLabel *baseFileNameLabel = new QLabel(tr("Output Base Filename:"));
    //User enter and edit a single line of plain text (Base File name)
    m_baseFileNameLine = new QLineEdit();

    //User-visible string
    QLabel *patientIDLabel = new QLabel(tr("Patient ID:"));
    //User enter and edit a single line of plain text (Patient identification)
    m_patientIDLine = new QLineEdit();

    //User-visible string
    QLabel *dateLabel = new QLabel(tr("Date:"));
    //User enter and edit a single line of plain text (Date)
    m_dateLine = new QLineEdit();

    //User-visible string
    QLabel *locationLabel = new QLabel(tr("Location:"));
    //User enter and edit a single line of plain text (Location)
    m_locationLine = new QLineEdit();

    //User-visible string
    QLabel *classGroupLabel = new QLabel(tr("Classification Group:"));
    //User enter and edit a single line of plain text (Classification Group)
    m_classificationGroupLine = new QLineEdit();

    //User-visible string
    QLabel *workspaceLabel = new QLabel(tr("Workspace:"));
    //User enter and edit a single line of plain text (Workspace)
    m_workspaceLine = new QLineEdit();

    //Connection between Recording Location and the Directory in the system
    connect(m_recordingLocationButton, &QAbstractButton::clicked, this, &MainWindow::chooseOutputDir);

    //Divide the space into rows and columns, and puts each widgets it manages into the correct cells
    QGridLayout *recordingLayout = new QGridLayout();
    recordingLayout->addWidget(outputLabel, 0, 0, 1, 1);
    recordingLayout->addWidget(m_recordingLocationLine, 1, 0, 1, 2);
    recordingLayout->addWidget(m_recordingLocationButton, 2, 1, 1, 1);
    recordingLayout->addWidget(baseFileNameLabel, 3, 0, 1, 1);
    recordingLayout->addWidget(m_baseFileNameLine, 4, 0, 1, 2);
    recordingLayout->addWidget(patientIDLabel, 5, 0, 1, 1);
    recordingLayout->addWidget(m_patientIDLine, 6, 0, 1, 2);
    recordingLayout->addWidget(dateLabel, 7, 0, 1, 1);
    recordingLayout->addWidget(m_dateLine, 8, 0, 1, 2);
    recordingLayout->addWidget(locationLabel, 9, 0, 1, 1);
    recordingLayout->addWidget(m_locationLine, 10, 0, 1, 2);
    recordingLayout->addWidget(classGroupLabel, 11, 0, 1, 1);
    recordingLayout->addWidget(m_classificationGroupLine, 12, 0, 1, 2);
    recordingLayout->addWidget(workspaceLabel, 13, 0, 1, 1);
    recordingLayout->addWidget(m_workspaceLine, 14, 0, 1, 2);

    //Application's current directory
    m_recordingLocation = QDir::current();

    //Edit a single line of plain text (Recording Location)
    m_recordingLocationLine->setText(m_recordingLocation.absolutePath());

    //Display Recording Layout
    m_recordingControlWidget->setLayout(recordingLayout);

    //Adds a tab with the Recording control Widget page and label to the tab widget
    m_controlTabWidget->addTab(m_recordingControlWidget, title);
}

//Create the Recording Status Group Box
void MainWindow::createRecordingStatusGroup(const QString &title)
{
    //Create new Recording Status Group Box
    m_recordingStatusGroup = new QGroupBox(title);

    //User-visible string
    m_recordingStatusLabel = new QLabel(tr("Not Recording"));
    //Customize the color of the label
    m_recordingStatusLabel->setStyleSheet("QLabel { color : red; }");

    //Divide the space into rows and columns, and puts each widgets it manages into the correct cells
    QGridLayout *recordingStatusLayout = new QGridLayout();
    recordingStatusLayout->addWidget(m_recordingStatusLabel, 0, 0, 1, 1);

    //Display Recording Status Layout
    m_recordingStatusGroup->setLayout(recordingStatusLayout);
}

void MainWindow::createOutsideXGroup(const QString &title)
{
    //Create new Recording Status Group Box
    m_outsideXGroup = new QGroupBox(title);

    //User-visible string
    m_outsideXLabel = new QLabel(tr(" "));
    //Customize the color of the label
    //m_outsideLabel->setStyleSheet("QLabel { color : red; }");

    //Divide the space into rows and columns, and puts each widgets it manages into the correct cells
    QGridLayout *OutsideXLayout = new QGridLayout();
    OutsideXLayout->addWidget(m_outsideXLabel, 0, 0, 1, 1);

    //Display Recording Status Layout
    m_outsideXGroup->setLayout(OutsideXLayout);
}

void MainWindow::createOutsideYGroup(const QString &title)
{
    //Create new Recording Status Group Box
    m_outsideYGroup = new QGroupBox(title);

    //User-visible string
    m_outsideYLabel = new QLabel(tr(" "));
    //Customize the color of the label
    //m_outsideLabel->setStyleSheet("QLabel { color : red; }");

    //Divide the space into rows and columns, and puts each widgets it manages into the correct cells
    QGridLayout *OutsideYLayout = new QGridLayout();
    OutsideYLayout->addWidget(m_outsideYLabel, 0, 0, 1, 1);

    //Display Recording Status Layout
    m_outsideYGroup->setLayout(OutsideYLayout);
}
//Load selected Pattern
void MainWindow::loadPattern(int i)
{
    //Load selected Pattern with the Graphics Widget
    m_gWidget->loadPattern(QLatin1String(m_patternCombo->itemData(i).toString().toLatin1()));

    //Clear previous trace rendering
    m_traceWidget->clearTracePoints();
    m_traceWidget->update();

    //Setup haptic tunnel
    loadHapticTunnel(m_tunnelCombo->currentIndex());
}

//Set Device with set values
void MainWindow::setDevice(int i)
{
    //Set Values
    HAPI::HAPIFloat stiffness = (double) m_stiffnessSpin->value() / 100;
    HAPI::HAPIFloat damping = (double) m_dampingSpin->value() / 100;
    HAPI::HAPIFloat friction = (double) m_frictionSpin->value() / 100;

    //If there are data in the Haptic Combo box,
    //and if is Haptic device enable or Haptic Device has been initialized
    if(m_hapticCombo->itemData(i) != QVariant::Invalid && m_hapticCombo->itemData(i).toBool() &&
            (m_hEnv->isDeviceEnabled() || m_hEnv->initHD()))
    {
        //Create Bound Box with Inertia values
        m_hEnv->createBoundBox(stiffness, damping, friction);
    }
    else
    {
        //Disable Haptic Device
        m_hEnv->disableHD();
        //Set index of no value
        m_hapticCombo->setCurrentIndex(0);
    }
}

//Set Gaze trace Widget in the main window
void MainWindow::setGazeOn(int i)
{
    //If there are data in the Gaze Combo box, and Gaze Device has been initialized
    if(m_gazeCombo->itemData(i) != QVariant::Invalid &&
            m_gazeCombo->itemData(i).toBool() && m_irisEnv->initIrisbond())
    {
        //Set index if there is value
        m_gazeCombo->setCurrentIndex(1);

        //Hide the Main Window while calibrating the eye tracker
        do
        {
            hide();
        }
        while (m_irisEnv->calibrateEyetracker());

        //Continue showing the main window
        show();
    }
    //If there are no data in the Gaze Combo box, and Gaze Device has not been initialized
    else
    {
        //Disable Iris Bond
        m_irisEnv->disableIrisbond();
        //Set index of no value
        m_gazeCombo->setCurrentIndex(0);
    }
}

//Set Inertial Sensors Widget in the main window
void MainWindow::setSensorsOn(int i)
{
    //If there are data in the Sensor Combo box, and Sensor Device has been initialized
    if(m_sensorCombo->itemData(i) != QVariant::Invalid && m_sensorCombo->itemData(i).toBool())
    {
        //Set index if there is value
        m_sensorCombo->setCurrentIndex(1);
    }
    //If there are no data in the Sensor Combo box, and Sensor Device has not been initialized
    else
    {
        //Set index of no value
        m_sensorCombo->setCurrentIndex(0);
    }
}

//Set Force Sensors Widget in the main window
void MainWindow::setForceSensorsOn(int i)
{
    //If there are data in the Force Sensor Combo box, and Force Sensor Device has been initialized
    if(m_forcesensorCombo->itemData(i) != QVariant::Invalid && m_forcesensorCombo->itemData(i).toBool())
    {
        //Set index if there is value
        m_forcesensorCombo->setCurrentIndex(1);
    }
    //If there are no data in the Force Sensor Combo box, and Force Sensor Device has not been initialized
    else
    {
        //Set index of no value
        m_forcesensorCombo->setCurrentIndex(0);
    }
}

//Set Arm in the main window
void MainWindow::setArmOn(int i)
{
    //If there are data in the Arm Combo box, and Arm Device has been initialized
    if(m_armCombo->itemData(i) != QVariant::Invalid && m_armCombo->itemData(i).toBool())
    {
        //Set index if there is value
        m_armCombo->setCurrentIndex(1);
    }
    //If there are no data in the Arm Combo box, and Arm Device has not been initialized
    else
    {
        //Set index of no value
        m_armCombo->setCurrentIndex(0);
    }
}

//Set Inertial Sensors Widget in the main window
void MainWindow::setExoOn(int i)
{
    //If there are data in the Sensor Combo box, and Sensor Device has been initialized
    if(m_exoCombo->itemData(i) != QVariant::Invalid && m_exoCombo->itemData(i).toBool())
    {
        //Set index if there is value
        m_exoCombo->setCurrentIndex(1);
    }
    //If there are no data in the Sensor Combo box, and Sensor Device has not been initialized
    else
    {
        //Set index of no value
        m_exoCombo->setCurrentIndex(0);
    }
}

//Set Haptic Stiffness
void MainWindow::setHapticStiffness(int i)
{
    //Set Values
    HAPI::HAPIFloat stiffness = (double) i / 100;
    HAPI::HAPIFloat damping = (double) m_dampingSpin->value() / 100;
    HAPI::HAPIFloat friction = (double) m_frictionSpin->value() / 100;

    //If Haptic Device enable
    if(m_hEnv->isDeviceEnabled())
    {
        //Create Bound Box with Inertia values
        m_hEnv->createBoundBox(stiffness, damping, friction);
    }
}

//Set Haptic Damping
void MainWindow::setHapticDamping(int i)
{
    //Set Values
    HAPI::HAPIFloat stiffness = (double) m_stiffnessSpin->value() / 100;
    HAPI::HAPIFloat damping = (double) i / 100;
    HAPI::HAPIFloat friction = (double) m_frictionSpin->value() / 100;

    //If Haptic Device enable
    if(m_hEnv->isDeviceEnabled())
    {
        //Create Bound Box with Inertia values
        m_hEnv->createBoundBox(stiffness, damping, friction);
    }
}

//Set Haptic Friction
void MainWindow::setHapticFriction(int i)
{
    //Set Values
    HAPI::HAPIFloat stiffness = (double) m_stiffnessSpin->value() / 100;
    HAPI::HAPIFloat damping = (double) m_dampingSpin->value() / 100;
    HAPI::HAPIFloat friction = (double) i / 100;

    //If Haptic Device enable
    if(m_hEnv->isDeviceEnabled())
    {
        //Create Bound Box with Inertia values
        m_hEnv->createBoundBox(stiffness, damping, friction);
    }
}

//Set Haptic Tunnel Force
void MainWindow::setHapticTunnelForce(int i)
{
    //Set value
    HAPI::HAPIFloat force = (double) i / 100;

    //If Haptic Device enable
    if(m_hEnv->isDeviceEnabled())
    {
        //Create Tunnel Force Effect
        m_hEnv->createTunnelEffect(m_curve, force);
    }
}

//Load the Haptic Tunnel selected
void MainWindow::loadHapticTunnel(int i)
{
    hapticTunnelAction();

    //Clear widget for rendering smooth path
    m_pathWidget->clearPath();

    //Index of smooth path haptic tunnel
    if(i == 1)
    {
        //Add normalized path points
        for(int i = 0; i < m_curve.size(); i++)
        {
            m_pathWidget->addNormPathPoint(QPointF(m_curve[i].x, m_curve[i].y));
        }
    }
}

//Output Directory in the system
void MainWindow::chooseOutputDir()
{
    //Name of the directory in the system
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select Output Directory for Recordings"),
                                                        QDir::currentPath());

    //Acces to directory and its contents
    m_recordingLocation = QDir(dirName);
    //Edit a single line of plain text (Recording Location)
    m_recordingLocationLine->setText(m_recordingLocation.absolutePath());
}

//Exit Application
void MainWindow::exitApp()
{
    close();
}

//Haptic Button Event
bool MainWindow::event(QEvent *event)
{
    //Handle recording on/off upon haptic button press
    if(event->type() == HapticButtonEvent::EVENT_TYPE)
    {
        //Save time data at the beginning of recording the action of the user (test)
        timerStart = QTime::currentTime().msecsSinceStartOfDay();
        //Recording the action of the user (test)
        recordingAction();
    }

    return QWidget::event(event);
}

//Keyboard Button Event (Ctrl+R)
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    //Handle recording on/off upon Ctrl-R press
    if( (event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Space)
    {
        //Save time data at the beginning of recording the action of the user (test)
        timerStart = QTime::currentTime().msecsSinceStartOfDay();
        //Recording the action of the user (test)
        recordingAction();
    }
}

bool MainWindow::getRecording()
{
    return m_isRecording;
}

//2 segundo en zona de inicio/fin
void MainWindow::AutoRecording()
{
    //Save time data at the beginning of recording the action of the user (test)
    timerStart = QTime::currentTime().msecsSinceStartOfDay();
    //Recording the action of the user (test)
    recordingAction();
}


//Recording Action data
void MainWindow::recordingAction()
{

    QString fileBaseName = QStringLiteral("%1").arg(QDateTime::currentMSecsSinceEpoch());
    QString userBaseName = QStringLiteral("%3").arg(QDateTime::currentMSecsSinceEpoch());

    //If the app is recording
    if(!m_isRecording)
    {
        //Clear previous Haptic trace data
        m_trace.clear();

        //Clear previous Haptic trace rendering
        m_traceWidget->clearTracePoints();
        m_traceWidget->update();

        //Set Data
        m_trace.setPatientID(m_patientIDLine->text().toUtf8().constData());
        m_trace.setDate(m_dateLine->text().toUtf8().constData());
        m_trace.setLocation(m_locationLine->text().toUtf8().constData());
        m_trace.setPatternType(m_patternInfo[m_patternCombo->currentIndex()].type.toUtf8().constData());
        m_trace.setPatternLevel(m_patternInfo[m_patternCombo->currentIndex()].level);
        m_trace.setClassificationGroup(m_classificationGroupLine->text().toInt());
        m_trace.setWorkspace(m_workspaceLine->text().toUtf8().constData());
        m_trace.setCoordinateSpace("Default");

        //Clear previous Exo sensors trace data
        m_Sensor.clear();

        //Set Data
        m_Sensor.setPatientID(m_patientIDLine->text().toUtf8().constData());
        m_Sensor.setDate(m_dateLine->text().toUtf8().constData());
        m_Sensor.setLocation(m_locationLine->text().toUtf8().constData());
        m_Sensor.setPatternType(m_patternInfo[m_patternCombo->currentIndex()].type.toUtf8().constData());
        m_Sensor.setPatternLevel(m_patternInfo[m_patternCombo->currentIndex()].level);
        m_Sensor.setClassificationGroup(m_classificationGroupLine->text().toInt());
        m_Sensor.setWorkspace(m_workspaceLine->text().toUtf8().constData());
        m_Sensor.setCoordinateSpace("default");

        //Clear previous Force sensors trace data
        m_ForceSensor.clear();

        //Set ForceData
        m_ForceSensor.setPatientID(m_patientIDLine->text().toUtf8().constData());
        m_ForceSensor.setDate(m_dateLine->text().toUtf8().constData());
        m_ForceSensor.setLocation(m_locationLine->text().toUtf8().constData());
        m_ForceSensor.setPatternType(m_patternInfo[m_patternCombo->currentIndex()].type.toUtf8().constData());
        m_ForceSensor.setPatternLevel(m_patternInfo[m_patternCombo->currentIndex()].level);
        m_ForceSensor.setClassificationGroup(m_classificationGroupLine->text().toInt());
        m_ForceSensor.setWorkspace(m_workspaceLine->text().toUtf8().constData());
        m_ForceSensor.setCoordinateSpace("default");


        //If Base File Name is not empty
        if(!m_baseFileNameLine->text().trimmed().isEmpty())
        {
            //Save File Base Name
            fileBaseName = m_baseFileNameLine->text().trimmed();
            //Save User Base Name
            userBaseName = m_patientIDLine->text().trimmed();
        }

        //Haptic Trace File Name
        m_traceFileName = QStringLiteral("haptic_%1%2_%3.csv").arg(fileBaseName)
                .arg(cont).arg(userBaseName);
        //Haptic Trace Score File Name
        m_traceScoreFileName = QStringLiteral("haptic_%1%2_%3_score.txt").arg(fileBaseName)
                .arg(cont).arg(userBaseName);

        //Exo Trace File Name
        m_SensorFileName = QStringLiteral("sensor_%1%2_%3.csv").arg(fileBaseName)
                .arg(cont).arg(userBaseName);

        //Force Sensor(esfera) File Name
        m_ForceSensorFileName = QStringLiteral("esfera_%1%2_%3.csv").arg(fileBaseName)
                .arg(cont).arg(userBaseName);

        //Sets all the information for the rehabilitation software
        if(m_irisEnv != nullptr && m_irisEnv->hiruEnabled())
        {
            //Clear previous gaze trace data
            m_GazeTrace.clear();

            //Clear previous gaze trace points
            m_gazeWidget->clearTracePoints();
            m_gazeWidget->update();

            //Set Data
            m_GazeTrace.setPatientID(m_patientIDLine->text().toUtf8().constData());
            m_GazeTrace.setDate(m_dateLine->text().toUtf8().constData());
            m_GazeTrace.setLocation(m_locationLine->text().toUtf8().constData());
            m_GazeTrace.setPatternType(m_patternInfo[m_patternCombo->currentIndex()]
                    .type.toUtf8().constData());
            m_GazeTrace.setPatternLevel(m_patternInfo[m_patternCombo->currentIndex()].level);
            m_GazeTrace.setClassificationGroup(m_classificationGroupLine->text().toInt());
            m_GazeTrace.setWorkspace(m_workspaceLine->text().toUtf8().constData());
            m_GazeTrace.setCoordinateSpace("default");

            //If Base File Name is not empty
            if(!m_baseFileNameLine->text().trimmed().isEmpty())
            {
                fileBaseName = m_baseFileNameLine->text().trimmed();
                userBaseName = m_patientIDLine->text().trimmed();
            }

            //Gaze Trace File Name
            m_gazeTraceFileName = QStringLiteral("gaze_%1%2_%3.csv").arg(fileBaseName)
                    .arg(cont).arg(userBaseName);
        }

        //Change status label to "Recording"
        m_recordingStatusLabel->setText(tr("Recording"));
        m_recordingStatusLabel->setStyleSheet("QLabel { color : green; }");

        //Put that the app is recording
        m_isRecording = true;

        //We add one to the patient counter
        cont = cont + 1;
    }
    //If the app has finished recording
    else
    {
        //Put that the app is not recording
        m_isRecording = false;

        //Export Yaml File with Haptic trace data
        m_trace.exportYamlFile(
                    std::string(m_recordingLocationLine->text().toUtf8().constData()) + "/" +
                    std::string(m_traceFileName.toUtf8().constData()));

        //If Sensor Environment is enable
        if (m_sensorCombo->currentIndex() == 1)
        {
            //Export Yaml File with Exo trace data
            m_Sensor.exportYamlFile(
                        std::string(m_recordingLocationLine->text().toUtf8().constData()) + "/" +
                        std::string(m_SensorFileName.toUtf8().constData()));
        }

        //If Force Sensor(esfera) Environment is enable
        if (m_forcesensorCombo->currentIndex() == 1)
        {
            //Export Yaml File with Exo trace data
            m_ForceSensor.exportYamlFile(
                        std::string(m_recordingLocationLine->text().toUtf8().constData()) + "/" +
                        std::string(m_ForceSensorFileName.toUtf8().constData()));
        }

        //If Iris Environment enable
        if(m_irisEnv != nullptr && m_irisEnv->hiruEnabled())
        {
            //Exports Gaze Trace data into a Yaml File
            m_GazeTrace.exportYamlFile(
                        std::string(m_recordingLocationLine->text().toUtf8().constData()) + "/" +
                        std::string(m_gazeTraceFileName.toUtf8().constData()));
        }

        //Change status label to "Not Recording"
        m_recordingStatusLabel->setText(tr("Not Recording"));
        m_recordingStatusLabel->setStyleSheet("QLabel { color : red; }");

        //End Exo Communication
        QString close = "15A5A" ;
        auto sender = new UDPSender(0);
        sender -> sendData(close);

        //Activation of Score Recording
        scoreRecording();

    }
}

//Timer Event
void MainWindow::timerEvent(QTimerEvent *e)
{
    //Handle trace sampling if recording
    if(m_isRecording)
    {
        //Save Current Time every step
        double timeStamp = QTime::currentTime().msecsSinceStartOfDay();

        //Set the saved time at the start of the test
        double startTime = timerStart;

        //To calculate the current time of each step in seconds
        timeStamp = (timeStamp - startTime)/1000;

        //Set Haptic device elapsed total time
        m_trace.setTotalTime(timeStamp);

        //Set Gaze device elapsed total time
        m_GazeTrace.setTotalTime(timeStamp);

        //Set Exo sensors device elapsed total time
        m_Sensor.setTotalTime(timeStamp);

        //Set Force sensors device elapsed total time
        m_ForceSensor.setTotalTime(timeStamp);

        //Sample points
        HAPI::Vec3 pos;

        //If Haptic Environment is enable
        if(m_hEnv != 0 && m_hEnv->isDeviceEnabled())
        {
            //Haptic device position
            pos = m_hEnv->getNormPosition();
        }
        //If there are any environmental graphics
        else if (m_gWidget != 0)
        {
            //Mouse position
            pos = m_gWidget->getMouseNormPosition();

        }

        //Stores gaze points data and enables the irisbond api functions
        std::vector<double> gazeEyeData;

        //Stores Exo points data
        std::vector<double> sensorData;

        //Stores Force Sensors points data
        std::vector<double> ForcesensorData;

        //If Sensor Environment is enable
        if (m_sensorCombo->currentIndex() == 1)
        {
            auto client = new UDPReceiver(0);
            client -> initSocketSensors();

            //Function that gets the normalize data to store it in a CSV file
            sensorData = client->saveData();

            if(m_armCombo->currentIndex() == 0)
            {
                m_Sensor.setArm(0);
            }
            else
            {
                m_Sensor.setArm(1);
            }
        }

        //If Force Sensor Environment is enable
        if (m_forcesensorCombo->currentIndex() == 1)
        {
            auto client = new UDPReceiver(0);
            client -> initForceSensors();

            //Function that gets the normalize data to store it in a CSV file
            ForcesensorData = client->saveDataForce();

            if(m_armCombo->currentIndex() == 0)
            {
                m_ForceSensor.setArm(0);
            }
            else
            {
                m_ForceSensor.setArm(1);
            }
        }

        //If Iris Environment is enable
        if(m_irisEnv != nullptr && m_irisEnv->hiruEnabled())
        {
            //Iris device position
            m_irisEnv->getGazePoints();
            //Function that gets the normalize data to store it in a CSV file
            gazeEyeData = m_irisEnv->saveData();
        }

        //Create events
        HapticTraceEvent evt1(pos.x, pos.y, pos.z, timeStamp);
        //Save trace information for file output
        m_trace.push_back(evt1);
        //qDebug()<<timeStamp;
        static double last_sent;
        if (timeStamp<0.1)
        {
            //qDebug()<<"hace el if";
            last_sent=0;
            //qDebug()<<last_sent;
        }
        else
        {
            //qDebug()<<last_sent;
            if (timeStamp>last_sent+0.1)
            {
               //qDebug()<<"Llega a la función";
                last_sent=timeStamp;
                Send_Distance(evt1);
                QCoreApplication::processEvents();
            }
        }


        //Draw trace
        m_traceWidget->addTracePoint(evt1);
        m_traceWidget->update();

        //Creates and event that gets data and update them in real time
        if (m_sensorCombo->currentIndex() == 1)
        {
            //Create events
            SensorEvent evt3(sensorData[0], sensorData[1], sensorData[2], sensorData[3], sensorData[4], sensorData[5], sensorData[6], timeStamp);
            //Save trace information for file output
            m_Sensor.push_back(evt3);
        }

        //Creates and event that gets data and update them in real time
        if (m_forcesensorCombo->currentIndex() == 1)
        {
            //Create events
            ForceSensorEvent evt4(ForcesensorData[0], ForcesensorData[1], ForcesensorData[2], ForcesensorData[3], timeStamp);
            //Save trace information for file output
            m_ForceSensor.push_back(evt4);
        }

        //Creates and event that gets the data and update data in real time
        if(m_irisEnv != nullptr && m_irisEnv->hiruEnabled())
        {
            //Gaze trace event
            GazeTraceEvent evt2(gazeEyeData[0],gazeEyeData[1],timeStamp);

            //Gaze trace data
            m_GazeTrace.push_back(evt2);

            //m_gazeWidget->addTracePoint(evt2);
            m_gazeWidget->update();
        }
    }

    //Post haptic button press events
    if(m_hEnv != 0 && m_hEnv->isDeviceEnabled())
    {
        //Get the Haptic Button Status
        bool buttonStatus = m_hEnv->getButtonStatus(0);

        //If Haptic Button is enable and the Haptic Button has not been pressed
        if(buttonStatus && !m_prevHapticButtonStatus)
        {
            //Haptic button press
            HapticButtonEvent *bEvent = new HapticButtonEvent();
            QCoreApplication::postEvent(this, bEvent);
        }

        //Save the Haptic Button Status
        m_prevHapticButtonStatus = buttonStatus;
    }
}

//Haptic Tunnel Action
void MainWindow::hapticTunnelAction()
{
    //Read the pattern selected
    cv::Mat src = cv::imread(m_patternCombo->currentData().toString().toUtf8().constData());

    //If the src is not empty
    if(!src.empty())
    {
        //Sample Points
        std::vector<cv::Point2d> sPoints;
        //Curve
        std::vector<cv::Point2d> curve;
        //Mid Path Points
        std::vector<cv::Point2d> points = extractMidPathPoints(src);

        //If the tunnel Combo box has not data
        if(m_tunnelCombo->currentData().toInt() == 0)
        {
            //Downsample points
            for(int i = 0; i < points.size() - 1; i += 20)
            {
                sPoints.push_back(points[i]);
            }

            sPoints.push_back(points[points.size()-1]);

            //Approximate curve
            curve = bsplineCurveApprox1(sPoints);
        }
        //If the tunnel Combo box has any data
        else
        {
            //Extract Path Region Points
            std::vector<cv::Point2d> contour = extractPathRegionPoints(src);

            //Downsample points
            for(int i = 0; i < points.size()-1; i += 60)
            {
                sPoints.push_back(points[i]);
            }

            sPoints.push_back(points[points.size()-1]);

            //Approximate curve
            curve = bsplineCurveApprox2(sPoints, contour);
        }

        //Normalize curve:
        HAPI::Vec2 minCorner(0,0);
        HAPI::Vec2 maxCorner(src.cols,src.rows);
        HAPI::Vec2 normMinCorner(-1.0, -1.0);
        HAPI::Vec2 normMaxCorner(1.0, 1.0);

        //Scale matrix for scaling the image coordinates to the norm coordinates
        HAPI::Matrix3 scaleTransform(
                    (normMaxCorner.x - normMinCorner.x)/(maxCorner.x - minCorner.x), 0, 0,
                    0, (normMaxCorner.y - normMinCorner.y)/(maxCorner.y - minCorner.y), 0,
                    0, 0, 1);

        HAPI::Vec3 newMaxCorner = scaleTransform *
                HAPI::Vec3(maxCorner.x, maxCorner.y, 1);

        //Translation matrix for moving the image coordinates after scaling
        HAPI::Matrix3 translationTransform(
                    1, 0, (normMaxCorner.x - newMaxCorner.x),
                    0, 1, (normMaxCorner.y - newMaxCorner.y),
                    0, 0, 1);

        //Norm affine transform is to perform the scale matrix multiplication first, then do translation
        HAPI::Matrix3 normTransform = translationTransform * scaleTransform;

        m_curve.clear();

        for(int i = 0; i < curve.size(); i++)
        {
            HAPI::Vec3 p(curve[i].x, curve[i].y, 1);

            HAPI::Vec3 newP = normTransform * p;

            m_curve.push_back(HAPI::Vec3(newP.x, -newP.y, -1));
        }

        //Set new haptic tunnel
        setHapticTunnelForce(m_tunnelSpin->value());

        m_scoringMidPathMat = drawCurve(curve, src.rows, src.cols);
        m_scoringPathRegionMat = getPathRegion(src);
    }
    //If the src is empty
    else
    {
        m_scoringMidPathMat = cv::Mat();
        m_scoringPathRegionMat = cv::Mat();
    }
}

//B-spline Curve Approximation 1
std::vector<cv::Point2d> MainWindow::bsplineCurveApprox1(std::vector<cv::Point2d> points)
{
    //Flatten points
    std::vector<double> sPoints;

    for(int i = 0; i < points.size() - 1; i++)
    {
        sPoints.push_back(points[i].x);
        sPoints.push_back(points[i].y);
    }

    sPoints.push_back(points[points.size()-1].x);
    sPoints.push_back(points[points.size()-1].y);

    //Create B-spline from control points
    ts::BSpline bSpline(5, 2, sPoints.size()/2, TS_CLAMPED);
    bSpline.setCtrlp(sPoints);

    //Sample 200 points from B-spline
    std::vector<cv::Point2d> curve;

    for(int i = 0; i <= 200; i++)
    {
        double t = i / 200.0;
        std::vector<double> val = bSpline.evaluate(t).result();
        curve.push_back(cv::Point2d(val[0], val[1]));
    }

    return curve;
}

//B-spline Curve Approximation 2
std::vector<cv::Point2d> MainWindow::bsplineCurveApprox2(std::vector<cv::Point2d> points,
                                                         std::vector<cv::Point2d> pathRegionContour)
{
    //Flatten points
    std::vector<double> sPoints;

    for(int i = 0; i < points.size() - 1; i++)
    {
        sPoints.push_back(points[i].x);
        sPoints.push_back(points[i].y);
    }

    sPoints.push_back(points[points.size()-1].x);
    sPoints.push_back(points[points.size()-1].y);

    //Flatten points of path region contour
    std::vector<double> contour;

    for(int i = 0; i < pathRegionContour.size(); i++)
    {
        contour.push_back(pathRegionContour[i].x);
        contour.push_back(pathRegionContour[i].y);
    }

    //Prepare data for constraints
    SmoothMeasConstraintData data;
    data.contour = contour;

    //Set starting point for searching for optimized solution
    std::vector<double> Q(sPoints.size());
    std::copy(sPoints.begin(), sPoints.end(), Q.begin());

    //Solve for optimized solution
    double minf;

    //Create B-spline from optimized control points Q[]
    ts::BSpline bSpline(5, 2, Q.size()/2, TS_CLAMPED);
    bSpline.setCtrlp(Q);

    //Sample 200 points from B-spline
    std::vector<cv::Point2d> curve;

    for(int i = 0; i <= 200; i++)
    {
        double t = i / 200.0;
        std::vector<double> val = bSpline.evaluate(t).result();
        curve.push_back(cv::Point2d(val[0], val[1]));
    }

    return curve;
}

//Score Recording
void MainWindow::scoreRecording()
{
    //Variables
    int validRegionPoints = 0;
    int validPathPoints = 0;
    double score;

    //If Trace Data, and Scoring Path Region and Scoring Mid Path are not empty
    if(!m_trace.empty() && !m_scoringPathRegionMat.empty() && !m_scoringMidPathMat.empty())
    {
        for(int i = 0; i < m_trace.size(); i++)
        {
            HapticTraceEvent event = m_trace[i];

            cv::Point2d p0(event.x, event.y);

            //Unnormalize p0
            p0.x *= m_scoringPathRegionMat.cols / 2.0;
            p0.y *= -m_scoringPathRegionMat.rows / 2.0;
            p0.x += m_scoringPathRegionMat.cols / 2.0;
            p0.y += m_scoringPathRegionMat.rows / 2.0;

            //Convert p0 into an integer point p1
            cv::Point p1(qRound(p0.x), qRound(p0.y));

            //Count number of pixels in valid regions
            uchar pixel1 = m_scoringPathRegionMat.at<uchar>(p1.y, p1.x);
            uchar pixel2 = m_scoringMidPathMat.at<uchar>(p1.y, p1.x);

            //Pixels count of scoring Path Region
            if(pixel1 > 0)
            {
                validRegionPoints++;
            }

            //Pixels count of scoring Mid Path Region
            if(pixel2 > 0)
            {
                validPathPoints++;
            }
        }

        //Calculate score if there are any valid Region Points
        if(validRegionPoints > 0)
        {
            score = 0.75 * ((double)validRegionPoints / m_trace.size()) +
                    0.25 * ((double)validPathPoints / validRegionPoints);
        }
        else
        {
            score = 0.0;
        }

        //To make the correlation
#if WIN32
        double correlation = 0;

        if(!m_GazeTrace.empty() && m_irisEnv->hiruEnabled())
        {
            //Vectors
            std::vector<double> hapticTraceMagX(m_trace.size());
            std::vector<double> gazeTraceMagY(m_GazeTrace.size());
            std::vector<double> gazeHapticProduct(m_trace.size());
            std::vector<double> squareX(m_trace.size());
            std::vector<double> squareY(m_GazeTrace.size());

            //Variables
            double totalXY;
            double totalX;
            double totalY;
            double totalSquareX;
            double totalSquareY;
            double totalX_totalX;
            double totalY_totalY;
            int n = m_trace.size();

            for(int i = 0; i < m_trace.size() && i < m_GazeTrace.size(); i++)
            {
                hapticTraceMagX[i] = sqrt((m_trace[i].x*m_trace[i].x)+(m_trace[i].y*m_trace[i].y));
                gazeTraceMagY[i] = sqrt((m_GazeTrace[i].x*m_GazeTrace[i].x)+
                                        (m_GazeTrace[i].y*m_GazeTrace[i].y));

                gazeHapticProduct[i] = hapticTraceMagX[i]*gazeTraceMagY[i];
                squareX[i] = hapticTraceMagX[i]*hapticTraceMagX[i];
                squareY[i] = gazeTraceMagY[i]*gazeTraceMagY[i];

                totalXY = totalXY + gazeHapticProduct[i];
                totalX = totalX + hapticTraceMagX[i];
                totalY = totalY + gazeTraceMagY[i];
                totalSquareX = totalSquareX + squareX[i];
                totalSquareY = totalSquareY + squareY[i];
            }

            totalX_totalX = totalX*totalX;
            totalY_totalY = totalY*totalY;

            correlation = Correlation::pearson(totalXY,totalX,totalY,totalSquareX,totalSquareY,
                                               totalX_totalX,totalY_totalY,n);
        }
#endif

        //Output scoring data to file
        QFile file(m_recordingLocationLine->text() + "/" + m_traceScoreFileName);

        if (file.open(QIODevice::ReadWrite))
        {
#define endl "\n"
            //Stream the output scoring data
            QTextStream stream(&file);

            stream << "Patient ID: " << m_patientIDLine->text() << endl;
            stream << "Date: " << m_dateLine->text() << endl;
            stream << "Pattern type: " << m_patternInfo[m_patternCombo->currentIndex()].type << endl;
            stream << "Level: " << m_patternInfo[m_patternCombo->currentIndex()].level << endl;
            stream << "Stiffness: " << QString::number(m_stiffnessSpin->value()) << "\%" << endl;
            stream << "Damping: " << QString::number(m_dampingSpin->value()) << "\%" << endl;
            stream << "Friction: " << QString::number(m_frictionSpin->value()) << "\%" << endl;
            stream << "Haptic tunnel mode: " << m_tunnelCombo->currentText() << endl;
            stream << "Haptic tunnel force: " << QString::number(m_tunnelSpin->value()) << "\%" << endl;
            stream << "Points in valid path region: " << QString::number(validRegionPoints)
                   << " out of " << QString::number(m_trace.size()) << endl;
            stream << "Points on selected path within valid path region: "
                   << QString::number(validPathPoints)
                   << " out of " << QString::number(validRegionPoints) << endl;
            stream << "Total haptic score: " << QString::number(score * 100,'f',2) << "\%" << endl;
            stream << "Elapsed time to complete the test: " << QString::number(m_trace.getTotalTime()) << "s" << endl;

#if WIN32
            //If the Gaze Traze Data is not empty and the hiru is enable
            if(!m_GazeTrace.empty() && m_irisEnv->hiruEnabled())
            {
                stream << "Haptic-Gaze Pearson Correlation Coeff: "
                       << QString::number(correlation, 'f', 2) << endl;
            }
#endif
        }

        //Create Message box for user
        QMessageBox msgBox;

        msgBox.setWindowTitle("Haptic Score");
        msgBox.setText(QString("Points in valid path region: %1 out of %2\n")
                       .arg(QString::number(validRegionPoints), QString::number(m_trace.size()))
                       + QString("Points on selected path within valid path region: %1 out of %2\n")
                       .arg(QString::number(validPathPoints), QString::number(validRegionPoints))
                       + QString("Total haptic score: %1\%\n").arg(QString::number(score * 100,'f',2))
                       + QString("Elapsed time to complete the test: %1\s").arg(QString::number(m_trace.getTotalTime())));

#if WIN32
        //If the Gaze Traze Data is not empty and the hiru is enable
        if(!m_GazeTrace.empty() && m_irisEnv->hiruEnabled())
        {
            msgBox.setText(msgBox.text() + tr("\n")
                           + QString("Haptic-Gaze Pearson Correlation Coeff: %1\n")
                           .arg(QString::number(correlation,'f',6)));
        }
#endif
        msgBox.exec();
    }
}

void MainWindow::UpdateOutsideLabel(double minDistanceX, double minDistanceY){
    m_outsideXLabel->setText(QString::number(minDistanceX,'f',2));
    m_outsideYLabel->setText(QString::number(minDistanceY,'f',2));
}

QString MainWindow::hex_Angles(int dist_x, int dist_y){
    dist_x = qBound(-150,dist_x,150);
    dist_y = qBound(-150,dist_y,150);

    int Angle_x = GetAngleX(dist_x);
    int Angle_y = GetAngleY(dist_y);

    QString Angle_x_Hex = QString::number(Angle_x,16).toUpper();
    QString Angle_y_Hex = QString::number(Angle_y,16).toUpper();

    QString ExoOn = QString::number(2,16).toUpper();
     if (m_exoCombo->currentIndex() == 0){
         ExoOn = QString::number(1,16).toUpper();
     }

    QString message =ExoOn + Angle_x_Hex + Angle_y_Hex;
    return message;

}

int MainWindow::GetAngleX(int distance){
    distance=-distance;
    //Devuelve el ángulo centrado en 90
    if (qAbs(distance) <= 20)
    {
        return 90;
    }

    if (distance > 20)
    {
        int Angle = ((distance-20)*13/130)+90;
        return Angle;
    }

    if (distance < -20)
    {
        int Angle = ((distance+20)*23/130)+90;
        return Angle;
    }
}

int MainWindow::GetAngleY(int distance){
    distance=-distance;
    if (qAbs(distance) <= 20)
    {
        return 90;
    }

    if (distance > 20)
    {
        int Angle = ((distance-20)*25/130)+90;
        return Angle;
    }

    if (distance < -20)
    {
        int Angle = ((distance+20)*31/130)+90;
        return Angle;
    }
}

void MainWindow::Send_Distance(HapticTraceEvent event){
    //qDebug()<<"Entra en la funcion";
    //If Trace Data, and Scoring Path Region and Scoring Mid Path are not empty
    if(!m_trace.empty() && !m_scoringPathRegionMat.empty() && !m_scoringMidPathMat.empty())
    {

       // HapticTraceEvent event = m_trace[-1];

        cv::Point2d p0(event.x, event.y);

        //Unnormalize p0
        p0.x *= m_scoringPathRegionMat.cols / 2.0;
        p0.y *= -m_scoringPathRegionMat.rows / 2.0;
        p0.x += m_scoringPathRegionMat.cols / 2.0;
        p0.y += m_scoringPathRegionMat.rows / 2.0;

        //Convert p0 into an integer point p1
        cv::Point p1(qRound(p0.x), qRound(p0.y));

        //Find distance to the midpad
        cv::Point closestPoint;
        double minDistance = std::numeric_limits<double>::max();
        double min_y;
        double min_x;

        for (int y = 0; y<m_scoringMidPathMat.rows; y++)
        {
            for (int x = 0; x<m_scoringMidPathMat.cols; x++)
            {
                if (m_scoringMidPathMat.at<uchar>(y,x)>0)
                {
                    cv::Point currentPoint(x,y);
                    double distance = cv::norm(p1 - currentPoint);

                    if (distance < minDistance)
                    {
                        minDistance = distance;
                        min_x = p1.x - currentPoint.x;
                        min_y = currentPoint.y - p1.y;
                        closestPoint = currentPoint;
                    }
                }
            }
        }
        UpdateOutsideLabel(min_x, min_y);
        QString str = hex_Angles(min_x,min_y);
        auto sender = new UDPSender(0);
        sender -> sendData(str);
    }

}

//para ver en que mapa estoy
QString MainWindow::getCurrentPattern() {
    int selectedIndex = m_patternCombo->currentIndex();
    if (selectedIndex > 0) {
        return m_patternCombo->currentText();
    } else {
        return "Ningún patrón seleccionado";
    }
}


