//Include standard files
//Defines the standard input/output stream objects
#include <iostream>
//Defines several functions to manipulate C strings and arrays
#include <cstring>
#include "UDPSender.h"
//#include "MainWindow.h"
#include <QObject>

#include <QUdpSocket>

UDPSender::UDPSender(QObject *parent) : QObject(parent)
{
    //socket2 = new QUdpSocket(this);
    socket = new QUdpSocket(this);
}

void UDPSender::initSocket()
{
    //Create a UDP socket for the sensors
    /*socket = new QUdpSocket(this);
    socket2 = new QUdpSocket(this);
    bool result =  socket->bind(QHostAddress::LocalHost, 2345);
    bool result2 =  socket2->bind(QHostAddress::LocalHost, 25000);
    if(result || result2)
    {
        //Write debugging information
        //qDebug() << "Socket PASS";
    }
    //If it doesn't bind to the Host address and correct port
    else
    {
        //Write debugging information
        qDebug() << "Socket FAIL";
    }*/
    //If it binds to the Host address and correct port
    bool result = socket->bind(QHostAddress::LocalHost, 2345);
    if (!result) {
        qDebug() << "Error al vincular el socket de fuerza:" << socket->errorString();
    }

    // Vincular el segundo socket para los sensores inerciales
    bool result2 = socket2->bind(QHostAddress::LocalHost, 25000);
    if (!result2) {
        qDebug() << "Error al vincular el socket de inercia:" << socket2->errorString();
    }
}

void UDPSender::sendData(const QString &str)
{
   //ocket = new QUdpSocket(this);
    //Sender IP Address
   // QHostAddress sender;
    //IP port
   // quint16 port;

    QByteArray message = str.toUtf8();
    //socket -> writeDatagram(message,message.size(),,2345);
    socket->writeDatagram(message,QHostAddress::LocalHost,2345);
    //socket2->writeDatagram(message,QHostAddress::LocalHost,25000);
    //qDebug() << "Socket PASS";
    /*QByteArray message = str.toUtf8();
    if (socket->state() == QAbstractSocket::BoundState) {
        socket->writeDatagram(message, QHostAddress::LocalHost, 2345);
    } else {
        qDebug() << "Socket de inercia no está vinculado correctamente.";
    }

    if (socket2->state() == QAbstractSocket::BoundState) {
        socket2->writeDatagram(message, QHostAddress::LocalHost, 25000);
    } else {
        qDebug() << "Socket de fuerza no está vinculado correctamente.";
    }*/
}
