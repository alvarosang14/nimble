#ifndef UDPSENDER_H
#define UDPSENDER_H

//Include Qt Core standard files
//The base class of al Qt objects
#include <QObject>

//Include Qt Network standard files
//User Datagram Protocol socket
#include <QUdpSocket>


//UDPReceiver class
class UDPSender: public QObject
{
    //QObject Macro
    Q_OBJECT

    //Public members
    public:
        //UDPReceiver Constructor (explicit to avoid implicit conversions of types)
        explicit UDPSender(QObject *parent=nullptr);
        void initSocket();
        void sendData(const QString &str);

    //Private members
    private:
        //Function that process pending datagrams
        void processPendingDatagrams();
        //Function to validate input data
        int validateInput(char hexVal[]);
        //Function to convert from hexadecimal to decimal
        int hexadecimalToDecimal(char hexVal[]);
        //New UDP socket object
        QUdpSocket *socket = nullptr;
        QUdpSocket *socket2 = nullptr;
};

#endif // UDPSENDER_H
