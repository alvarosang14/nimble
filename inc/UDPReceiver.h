//UDPReceiver.h
#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

//Include Qt Core standard files
//The base class of al Qt objects
#include <QObject>

//Include Qt Network standard files
//User Datagram Protocol socket
#include <QUdpSocket>

//UDPReceiver class
class UDPReceiver: public QObject
{
    //QObject Macro
    Q_OBJECT

    //Public members
    public:
        //UDPReceiver Constructor (explicit to avoid implicit conversions of types)
        explicit UDPReceiver(QObject *parent);
        //Function to initialize the socket of the Sensors
        void initSocketSensors();
        void initForceSensors();
        //Function that gets the Sensor data to store it in a CSV file
        std::vector<double> saveData();
        std::vector<double> saveDataForce();

    //Private members
    private:
        //Function that process pending datagrams
        void processPendingDatagrams();
        void processPendingDatagramsForce();//esta para los sensores de fuerza
        //Function to validate input data
        int validateInput(char hexVal[]);
        //Function to convert from hexadecimal to decimal
        int hexadecimalToDecimal(char hexVal[]);
        //New UDP socket object
        QUdpSocket *socket = nullptr;
        QUdpSocket *socket2 = nullptr;
};

#endif // UDPRECEIVER_H
