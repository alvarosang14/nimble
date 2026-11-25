//Include QtWidgets, QOpenGL and QCoreApplication standard needed files
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QCoreApplication>

//Include the user-defined needed files
#include "MainWindow.h"

#include "UDPSender.h"

//Main function
int main(int argc, char **argv)
{
    //Initialize the window system and constructs an application object
    QApplication app(argc, argv);

    //Initialize the main window
    MainWindow window;

    //Display the main window
    window.show();
    //Conection MainWindow-UDPSender
   // MainWindow mainWindow;
   // UDPSender udpSender;
   // QObject::connect(&mainWindow, SIGNAL(Reply(QString)),
   //                 &udpSender, SLOT(sendData(QString)));

    //Enters the main event loop and wait until exit is called
    return app.exec();
}
