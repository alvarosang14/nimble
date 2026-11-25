//UDPReceiver.cpp

//Include the user-defined needed files
//UDP Connection Receiver for the exoskeleton
#include "UDPReceiver.h"

//Include standard files
//Defines the standard input/output stream objects
#include <iostream>
//Defines several functions to manipulate C strings and arrays
#include <cstring>
//Declares a set of functions to compute common mathematical operations and transformations
#include <cmath>

//Sensor data vector
std::vector<double> SensorDataVector(7);
std::vector<double> SensorForceDataVector(4);

//UDPReceiver Constructor
UDPReceiver::UDPReceiver(QObject *parent): QObject(parent)
{
}

//Function to initialize the socket of the inertial sensors
void UDPReceiver::initSocketSensors()
{
    //Create a UDP socket for the sensors
    socket = new QUdpSocket(this);
    //Socket bind to Host Address and IP Port
    bool result =  socket->bind(QHostAddress::LocalHost, 1234);

    //If it binds to the Host address and correct port
    if(result)
    {
        //Write debugging information
        //qDebug() << "Socket PASS";
    }
    //If it doesn't bind to the Host address and correct port
    else
    {
        //Write debugging information
        qDebug() << "Socket FAIL";
    }

    //Process pending datagrams
    processPendingDatagrams();
    //Connects socket
    connect(socket, &QUdpSocket::readyRead, this, &UDPReceiver::processPendingDatagrams);
}

//Process pending datagrams
void UDPReceiver::processPendingDatagrams()
{
    //Write debugging information
    //qDebug() << "Processing datagram";

    //Sender IP Address
    QHostAddress sender;
    //IP port
    quint16 port;
    //Datagram string
    std::string full_datagram;

    //Sensor Variables
    int wrist_flex_ext = 0;
    int wrist_abd_add = 0;
    int elbow_flex_ext = 0;
    int elbow_pron_sup = 0;
    int shoulder_flex_ext = 0;
    int shoulder_abd_add = 0;
    int shoulder_ext_int_rot = 0;

    //Run while at least one datagram is waiting to be read
    while (socket->hasPendingDatagrams())
    {
        //Byte array
        QByteArray datagram_array;

        //Resize byte array with the size of the first pending UDP datagram
        datagram_array.resize(socket->pendingDatagramSize());

        //Receives the datagram and stores it in byte array data
        socket->readDatagram(datagram_array.data(),datagram_array.size(),&sender,&port);

        //Write debugging information
        //qDebug() <<"Message From :: " << sender.toString();
        //qDebug() <<"Port From :: "<< port;
        //qDebug() <<"Message :: " << datagram_array.toHex();
        //qDebug() <<"Message :: " << datagram_array.size();

        //Conversion from QByteArray to QString
        QString data_qstring = QString(datagram_array.toHex());

        //Conversion from QString to string
        std::string data_string = data_qstring.toStdString();
        //Datagram
        full_datagram = data_string;
    }

    //If there is datagram
    if(full_datagram != "")
    {
        //Conversion from little-endian to big-endian
        //Least signifcant byte 1
        std::string lsb_1 = full_datagram.substr(0,2);
        //Most signifcant byte 1
        std::string msb_1 = full_datagram.substr(2,2);
        //Least signifcant byte 2
        std::string lsb_2 = full_datagram.substr(4,2);
        //Most signifcant byte 2
        std::string msb_2 = full_datagram.substr(6,2);

        //Least signifcant byte 3
        std::string lsb_3 = full_datagram.substr(8,2);
        //Most signifcant byte 3
        std::string msb_3 = full_datagram.substr(10,2);
        //Least signifcant byte 4
        std::string lsb_4 = full_datagram.substr(12,2);
        //Most signifcant byte 4
        std::string msb_4 = full_datagram.substr(14,2);

        //Least signifcant byte 5
        std::string lsb_5 = full_datagram.substr(16,2);
        //Most signifcant byte 5
        std::string msb_5 = full_datagram.substr(18,2);
        //Least signifcant byte 6
        std::string lsb_6 = full_datagram.substr(20,2);
        //Most signifcant byte 6
        std::string msb_6 = full_datagram.substr(22,2);
        //Least signifcant byte 7
        std::string lsb_7 = full_datagram.substr(24,2);
        //Most signifcant byte 7
        std::string msb_7 = full_datagram.substr(26,2);

        //Big-endian hexadecimal for wrist flexion-extension
        std::string hex1;
        hex1.append(msb_1).append(lsb_1);
        //Big-endian hexadecimal for wrist abduction-adduction
        std::string hex2;
        hex2.append(msb_2).append(lsb_2);

        //Big-endian hexadecimal for elbow flexion-extension
        std::string hex3;
        hex3.append(msb_3).append(lsb_3);
        //Big-endian hexadecimal for elbow pronation-supination
        std::string hex4;
        hex4.append(msb_4).append(lsb_4);

        //Big-endian hexadecimal for shoulder flexion-extension
        std::string hex5;
        hex5.append(msb_5).append(lsb_5);
        //Big-endian hexadecimal for shoulder abduction-adduction
        std::string hex6;
        hex6.append(msb_6).append(lsb_6);
        //Big-endian hexadecimal for shoulder external-internal rotation
        std::string hex7;
        hex7.append(msb_7).append(lsb_7);

        //We transform the string of the first data given in lowercase to uppercase for the conversion from hexadecimal (char[]) to decimal (int)
        transform(hex1.begin(), hex1.end(), hex1.begin(), [](char const &c)
        {
            //We transform each character of the string to uppercase
            return toupper(c);
        });

        //We transform the string of the second data given in lowercase to uppercase for the conversion from hexadecimal (char[]) to decimal (int)
        transform(hex2.begin(), hex2.end(), hex2.begin(), [](char const &c)
        {
            //We transform each character of the string to uppercase
            return toupper(c);
        });

        //We transform the string of the second data given in lowercase to uppercase for the conversion from hexadecimal (char[]) to decimal (int)
        transform(hex3.begin(), hex3.end(), hex3.begin(), [](char const &c)
        {
            //We transform each character of the string to uppercase
            return toupper(c);
        });

        //We transform the string of the second data given in lowercase to uppercase for the conversion from hexadecimal (char[]) to decimal (int)
        transform(hex4.begin(), hex4.end(), hex4.begin(), [](char const &c)
        {
            //We transform each character of the string to uppercase
            return toupper(c);
        });

        //We transform the string of the second data given in lowercase to uppercase for the conversion from hexadecimal (char[]) to decimal (int)
        transform(hex5.begin(), hex5.end(), hex5.begin(), [](char const &c)
        {
            //We transform each character of the string to uppercase
            return toupper(c);
        });

        //We transform the string of the second data given in lowercase to uppercase for the conversion from hexadecimal (char[]) to decimal (int)
        transform(hex6.begin(), hex6.end(), hex6.begin(), [](char const &c)
        {
            //We transform each character of the string to uppercase
            return toupper(c);
        });

        //We transform the string of the second data given in lowercase to uppercase for the conversion from hexadecimal (char[]) to decimal (int)
        transform(hex7.begin(), hex7.end(), hex7.begin(), [](char const &c)
        {
            //We transform each character of the string to uppercase
            return toupper(c);
        });

        //Hexadecimal for wrist flexion-extension char array
        char hex1_array[5];
        //Hexadecimal for wrist abduction-adduction char array
        char hex2_array[5];

        //Hexadecimal for elbow flexion-extension char array
        char hex3_array[5];
        //Hexadecimal for elbow pronation-supination char array
        char hex4_array[5];

        //Hexadecimal for shoulder flexion-extension char array
        char hex5_array[5];
        //Hexadecimal for shoulder abduction-adduction char array
        char hex6_array[5];
        //Hexadecimal for shoulder external-internal rotation char array
        char hex7_array[5];

        //Conversion from string to char array
        strcpy_s(hex1_array, sizeof(hex1_array), hex1.c_str());
        strcpy_s(hex2_array, sizeof(hex2_array), hex2.c_str());
        strcpy_s(hex3_array, sizeof(hex3_array), hex3.c_str());
        strcpy_s(hex4_array, sizeof(hex4_array), hex4.c_str());
        strcpy_s(hex5_array, sizeof(hex5_array), hex5.c_str());
        strcpy_s(hex6_array, sizeof(hex6_array), hex6.c_str());
        strcpy_s(hex7_array, sizeof(hex7_array), hex7.c_str());

        //If the input data type is wrong
        if (!validateInput(hex1_array))
        {
            //Wrong input value
            std::cout << "Error\n";
        }
        //If the input data is correct
        else
        {
            //Conversion from hexadecimal to decimal
            wrist_flex_ext = hexadecimalToDecimal(hex1_array);
        }

        //If the input data type is wrong
        if (!validateInput(hex2_array))
        {
            //Wrong input value
            std::cout << "Error\n";
        }
        //If the input data is correct
        else
        {
            //Conversion from hexadecimal to decimal
            wrist_abd_add = hexadecimalToDecimal(hex2_array);
        }

        //If the input data type is wrong
        if (!validateInput(hex3_array))
        {
            //Wrong input value
            std::cout << "Error\n";
        }
        //If the input data is correct
        else
        {
            //Conversion from hexadecimal to decimal
            elbow_flex_ext = hexadecimalToDecimal(hex3_array);
        }

        //If the input data type is wrong
        if (!validateInput(hex4_array))
        {
            //Wrong input value
            std::cout << "Error\n";
        }
        //If the input data is correct
        else
        {
            //Conversion from hexadecimal to decimal
            elbow_pron_sup = hexadecimalToDecimal(hex4_array);
        }

        //If the input data type is wrong
        if (!validateInput(hex5_array))
        {
            //Wrong input value
            std::cout << "Error\n";
        }
        //If the input data is correct
        else
        {
            //Conversion from hexadecimal to decimal
            shoulder_flex_ext = hexadecimalToDecimal(hex5_array);
        }

        //If the input data type is wrong
        if (!validateInput(hex6_array))
        {
            //Wrong input value
            std::cout << "Error\n";
        }
        //If the input data is correct
        else
        {
            //Conversion from hexadecimal to decimal
            shoulder_abd_add = hexadecimalToDecimal(hex6_array);
        }

        //If the input data type is wrong
        if (!validateInput(hex7_array))
        {
            //Wrong input value
            std::cout << "Error\n";
        }
        //If the input data is correct
        else
        {
            //Conversion from hexadecimal to decimal
            shoulder_ext_int_rot = hexadecimalToDecimal(hex7_array);
        }


        //If wrist flexion
        if (wrist_flex_ext >= 180 && wrist_flex_ext <= 360)
        {
            //cout << "Wrist flexion: " << 360 - wrist_flex_ext << endl;
            wrist_flex_ext = 360 - wrist_flex_ext;
            wrist_flex_ext = - wrist_flex_ext;
        }
        //If wrist extension
        else
        {
            //cout << "Wrist extension: " << wrist_flex_ext << endl;
        }

        //If wrist adduction
        if (wrist_abd_add >= 180 && wrist_abd_add <= 360)
        {
            //cout << "Wrist adduction: " << 360 - wrist_abd_add << endl;
            wrist_abd_add = 360 - wrist_abd_add;
        }
        //If wrist abduction
        else
        {
            wrist_abd_add = - wrist_abd_add;
            //cout << "Wrist abduction: " << wrist_abd_add << endl;
        }

        //If elbow extension
        if (elbow_flex_ext >= 180 && elbow_flex_ext <= 360)
        {
            //cout << "Elbow extension: " << 360 - elbow_flex_ext << endl;
            elbow_flex_ext = 360 - elbow_flex_ext;
            elbow_flex_ext = - elbow_flex_ext;
        }
        //If elbow flexion
        else
        {
            //cout << "Elbow flexion: " << elbow_flex_ext << endl;
        }

        //If elbow supination (left arm)
        if (elbow_pron_sup >= 180 && elbow_pron_sup <= 360)
        {
            //cout << "Elbow supination: " << 360 - elbow_pron_sup << endl;
            elbow_pron_sup = 360 - elbow_pron_sup;
            elbow_pron_sup = - elbow_pron_sup;
        }
        //If elbow pronation (left arm)
        else
        {
            //cout << "Elbow pronation: " << elbow_pron_sup << endl;
        }

        //If shoulder extension
        if (shoulder_flex_ext >= 180 && shoulder_flex_ext <= 360)
        {
            //cout << "Shoulder extension: " << 360 - shoulder_flex_ext << endl;
            shoulder_flex_ext = 360 - shoulder_flex_ext;
            shoulder_flex_ext = - shoulder_flex_ext;
        }
        //If shoulder flexion
        else
        {
            //cout << "Shoulder flexion: " << shoulder_flex_ext << endl;
        }

        //If shoulder adduction (left arm)
        if (shoulder_abd_add >= 180 && shoulder_abd_add <= 360)
        {
            //cout << "Shoulder adduction: " << 360 - shoulder_abd_add << endl;
            shoulder_abd_add = 360 - shoulder_abd_add;
        }
        //If shoulder abduction
        else
        {
            shoulder_abd_add = - shoulder_abd_add;
            //cout << "Shoulder abduction: " << shoulder_abd_add << endl;
        }

        //If shoulder internal rotation (left arm)
        if (shoulder_ext_int_rot >= 180 && shoulder_ext_int_rot <= 360)
        {
            //cout << "Shoulder internal rotation: " << 360 - shoulder_flex_ext << endl;
            shoulder_ext_int_rot = 360 - shoulder_ext_int_rot;
        }
        //If shoulder external rotation (left arm)
        else
        {
            shoulder_ext_int_rot = - shoulder_ext_int_rot;
            //cout << "Shoulder external rotation: " << shoulder_flex_ext << endl;
        }

        //Store into sensor data vector
        SensorDataVector[0] = wrist_flex_ext;
        SensorDataVector[1] = wrist_abd_add;
        SensorDataVector[2] = elbow_flex_ext;
        SensorDataVector[3] = elbow_pron_sup;
        SensorDataVector[4] = shoulder_flex_ext;
        SensorDataVector[5] = shoulder_abd_add;
        SensorDataVector[6] = shoulder_ext_int_rot;
    }
}

//Function to validate input data
int UDPReceiver::validateInput(char hexVal[])
{
    int result = 0;
    int len = strlen(hexVal);

    if (len < 1)
    {
        std::cout << "Empty input?\n";
        result = 0;
    }
    else
    {
        result = 1;
    }

    //Now validate every symbol of the line
    for(int i=0; i<len; i++)
    {
        //cout << hexVal[i];

        if(!(((floor(hexVal[i]) == ceil(hexVal[i])) /* check if is integer */
                && (hexVal[i] >= '0' && hexVal[i] <= '9')) /* if is integer, check value */
            || ((isalpha(hexVal[i])) /* if is a latin letter  */
                && (hexVal[i] >= 'A' && hexVal[i] <= 'F')) )) { /* check that it is A-F */
            std::cout << hexVal[i] << " is an invalid character for a hexademical number!\n";
                    result = 0;
                    break;
            }
    }

    return result;
}

// Function to convert hexadecimal to decimal
int UDPReceiver::hexadecimalToDecimal(char hexVal[])
{
    int len = strlen(hexVal);

    // Initializing base value to 1, i.e 16^0
    int base = 1;
    int dec_val = 0;

    // Extracting characters as digits from last character
    for (int i=len-1; i>=0; i--)
    {
        // if character lies in '0'-'9', converting
        // it to integral 0-9 by subtracting 48 from
        // ASCII value.
        if (hexVal[i]>='0' && hexVal[i]<='9')
        {
            dec_val += (hexVal[i] - 48)*base;

            // incrementing base by power
            base = base * 16;
        }
        // if character lies in 'A'-'F' , converting
        // it to integral 10 - 15 by subtracting 55
        // from ASCII value
        else if (hexVal[i]>='A' && hexVal[i]<='F')
        {
            dec_val += (hexVal[i] - 55)*base;

            // incrementing base by power
            base = base*16;
        }
    }

    return dec_val;
}

//function that gets the normalize data to store it in a CSV file
std::vector<double> UDPReceiver::saveData()
{
    //normalizeGaze();
    return SensorDataVector;
}


//PARTE DE LOS SENSORES DE FUERZA DE LA ESFERA
void UDPReceiver::initForceSensors()
{
    //Create a UDP socket for the sensors
    socket2 = new QUdpSocket(this);
    //Socket bind to Host Address and IP Port
    bool result =  socket2->bind(QHostAddress::LocalHost, 25000);

    //If it binds to the Host address and correct port
    if(result)
    {
        //Write debugging information
        qDebug() << "Socket Force PASS";
    }
    //If it doesn't bind to the Host address and correct port
    else
    {
        //Write debugging information
        qDebug() << "Socket Force FAIL";
    }

    //Process pending datagrams
    processPendingDatagramsForce();
    //Connects socket
    connect(socket2, &QUdpSocket::readyRead, this, &UDPReceiver::processPendingDatagramsForce);
}

void UDPReceiver::processPendingDatagramsForce()
{
    //Sender IP Address
    QHostAddress sender;
    //IP port
    quint16 port;
    //Datagram string
    std::string full_datagram;

    // Variables de los sensores de fuerza
    int sensor1 = 0;
    int sensor2 = 0;
    int sensor3 = 0;
    int sensor4 = 0;

    // Mientras haya al menos un datagrama esperando ser leído
    while (socket2->hasPendingDatagrams())
    {
        //Byte array
        QByteArray datagram_array;
        //Resize byte array with the size of the first pending UDP datagram
        datagram_array.resize(socket2->pendingDatagramSize());
        //Receives the datagram and stores it in byte array data
        socket2->readDatagram(datagram_array.data(), datagram_array.size(), &sender, &port);

        /*//Write debugging information
        qDebug() <<"Message From :: " << sender.toString();
        qDebug() <<"Port From :: "<< port;
        qDebug() <<"Message :: " << datagram_array.toHex();
        qDebug() <<"Message :: " << datagram_array.size();*/

        //Conversion from QByteArray to QString
        QString data_qstring = QString(datagram_array.toHex());

        //Conversion from QString to string
        std::string data_string = data_qstring.toStdString();
        //Datagram
        full_datagram = data_string;
    }

    // Si hay un datagram
    if (full_datagram != "")
    {
        // Extraer los bytes significativos bajos y altos para cada sensor
        std::string lsb1 = full_datagram.substr(0, 2);
        std::string msb1 = full_datagram.substr(2, 2);
        std::string lsb2 = full_datagram.substr(4, 2);
        std::string msb2 = full_datagram.substr(6, 2);
        std::string lsb3 = full_datagram.substr(8, 2);
        std::string msb3 = full_datagram.substr(10, 2);
        std::string lsb4 = full_datagram.substr(12, 2);
        std::string msb4 = full_datagram.substr(14, 2);


        // Construir los valores hexadecimales big-endian para cada sensor
        std::string hex1;
        hex1.append(msb1).append(lsb1);
        std::string hex2;
        hex2.append(msb2).append(lsb2);
        std::string hex3;
        hex3.append(msb3).append(lsb3);
        std::string hex4;
        hex4.append(msb4).append(lsb4);

        // Transformar los strings a mayúsculas
        //std::transform(hex1.begin(), hex1.end(), hex1.begin(), ::toupper);
        //We transform the string of the first data given in lowercase to uppercase for the conversion from hexadecimal (char[]) to decimal (int)
       transform(hex1.begin(), hex1.end(), hex1.begin(), [](char const &c)
        {
            //We transform each character of the string to uppercase
            return toupper(c);
        });

        //std::transform(hex2.begin(), hex2.end(), hex2.begin(), ::toupper);
        //We transform the string of the first data given in lowercase to uppercase for the conversion from hexadecimal (char[]) to decimal (int)
        transform(hex2.begin(), hex2.end(), hex2.begin(), [](char const &c)
        {
            //We transform each character of the string to uppercase
            return toupper(c);
        });
        //std::transform(hex3.begin(), hex3.end(), hex3.begin(), ::toupper);
        //We transform the string of the first data given in lowercase to uppercase for the conversion from hexadecimal (char[]) to decimal (int)
        transform(hex3.begin(), hex3.end(), hex3.begin(), [](char const &c)
        {
            //We transform each character of the string to uppercase
            return toupper(c);
        });
        //std::transform(hex4.begin(), hex4.end(), hex4.begin(), ::toupper);
        //We transform the string of the first data given in lowercase to uppercase for the conversion from hexadecimal (char[]) to decimal (int)
        transform(hex4.begin(), hex4.end(), hex4.begin(), [](char const &c)
        {
            //We transform each character of the string to uppercase
            return toupper(c);
        });

        // Convertir los strings a arreglos de char
        char hex1_array[5], hex2_array[5], hex3_array[5], hex4_array[5];
        strcpy_s(hex1_array, sizeof(hex1_array), hex1.c_str());
        strcpy_s(hex2_array, sizeof(hex2_array), hex2.c_str());
        strcpy_s(hex3_array, sizeof(hex3_array), hex3.c_str());
        strcpy_s(hex4_array, sizeof(hex4_array), hex4.c_str());

        // Validar y convertir los datos de los sensores
        if (validateInput(hex1_array)){
            sensor1 = hexadecimalToDecimal(hex1_array);
        }
        else
            std::cout << "Error en Sensor 1\n";

        if (validateInput(hex2_array)){
            sensor2 = hexadecimalToDecimal(hex2_array);
        }
        else
            std::cout << "Error en Sensor 2\n";

        if (validateInput(hex3_array)){
            sensor3 = hexadecimalToDecimal(hex3_array);
        }
        else
            std::cout << "Error en Sensor 3\n";

        if (validateInput(hex4_array)){
            sensor4 = hexadecimalToDecimal(hex4_array);
        }
        else
            std::cout << "Error en Sensor 4\n";

        // Almacenar los datos en el vector de datos de los sensores de fuerza
        SensorForceDataVector[0] = sensor1;
        SensorForceDataVector[1] = sensor2;
        SensorForceDataVector[2] = sensor3;
        SensorForceDataVector[3] = sensor4;
    }
}
// Función que obtiene los datos normalizados para almacenarlos en un archivo CSV
std::vector<double> UDPReceiver::saveDataForce()
{
    return SensorForceDataVector;
}


