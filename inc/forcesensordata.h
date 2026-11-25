#ifndef FORCESENSORDATA_H
#define FORCESENSORDATA_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <yaml-cpp/yaml.h>

using namespace std;

//Force Sensor Event structure
struct ForceSensorEvent
{
    //Force Sensor event default Constructor
    ForceSensorEvent(): sensor1(0), sensor2(0), sensor3(0), sensor4(0), time(0)
    {
    }

    //Sensor event parameterized Constructor
    ForceSensorEvent(double SENSOR1, double SENSOR2, double SENSOR3, double SENSOR4, double Time): sensor1(SENSOR1), sensor2(SENSOR2), sensor3(SENSOR3), sensor4(SENSOR4), time(Time)
    {
    }

    //Event data
    double sensor1, sensor2, sensor3, sensor4, time;
};

//Force Sensor Data class
class ForceSensorData
{
    //Public access members (members are accesible from outside the class)
    public:
        //Sensor data default Constructor
        ForceSensorData();
        //Sensor data parameterized Constructor
        ForceSensorData(const string& traceFile);

        //Input/Output functions
        void loadYamlFile(const string& traceFile);
        void exportYamlFile(const string& traceFile) const;
        string exportYamlString() const;

        //Access methods
        string getPatientID() const;
        string getDate() const;
        string getLocation() const;
        string getPatternType() const;
        int getPatternLevel() const;
        int getClassificationGroup() const;
        string getWorkspace() const;
        string getCoordinateSpace() const;
        double getTotalTime() const;
        bool getArm() const;

        //Modifier methods
        void setPatientID(string patientID_str);
        void setDate(string date_str);
        void setLocation(string location_str);
        void setPatternType(string patternType_str);
        void setPatternLevel(int patternLevel_int);
        void setClassificationGroup(int classificationGroup_int);
        void setWorkspace(string workspace_str);
        void setCoordinateSpace(string coordinateSpace_str);
        void setTotalTime(double totalTime_dbl);
        void setArm(bool arm_bool);

        //Vector methods
        //Returns a reference to the element at the position specified in the vector
        ForceSensorEvent& at(size_t pos);
        const ForceSensorEvent& at(size_t pos) const;
        //Returns a reference to the element at the position specified in the vector container
        ForceSensorEvent& operator[](size_t pos);
        const ForceSensorEvent& operator[](size_t pos) const;
        //Adds a new element (value) at the end of the vector, after its current last element
        void push_back(const ForceSensorEvent& value);
        //Removes the last element in the vector, effectively reducing the container size by one
        void pop_back();
        //Returns whether the vector is empty (size = 0)
        bool empty() const;
        //Returns the number of elements in the vector
        size_t size() const;
        //Returns an iterator pointing to the first element in the vector
        vector<ForceSensorEvent>::iterator begin();
        vector<ForceSensorEvent>::const_iterator begin() const;
        //Returns an iterator referring to the past-the-end element in the vector container
        vector<ForceSensorEvent>::iterator end();
        vector<ForceSensorEvent>::const_iterator end() const;
        //Removes all the elements from the vector, leaving the container with a size of 0
        void clear();

    //Private access members (members are not accesible (or viewed) from outside the class)
    private:
        //Private attributes
        string filename;
        string patientID;
        string date;
        string location;
        string patternType;
        int patternLevel;
        int classificationGroup;
        string workspace;
        string coordinateSpace;
        double totalTime;
        //Select the arm
        bool arm = false;


        // Vector con eventos de sensores de fuerza
        vector<ForceSensorEvent> events;
};

//Inline methods (more efficient at runtime)

//Access methods
inline string ForceSensorData::getPatientID() const
{
    return patientID;
}
inline string ForceSensorData::getDate() const
{
    return date;
}
inline string ForceSensorData::getLocation() const
{
    return location;
}
inline string ForceSensorData::getPatternType() const
{
    return patternType;
}
inline int ForceSensorData::getPatternLevel() const
{
    return patternLevel;
}
inline int ForceSensorData::getClassificationGroup() const
{
    return classificationGroup;
}
inline string ForceSensorData::getWorkspace() const
{
    return workspace;
}
inline string ForceSensorData::getCoordinateSpace() const
{
    return coordinateSpace;
}
inline double ForceSensorData::getTotalTime() const
{
    return totalTime;
}
inline bool ForceSensorData::getArm() const
{
    return arm;
}

//Modifier methods
inline void ForceSensorData::setPatientID(string patientID_str)
{
    patientID = patientID_str;
}
inline void ForceSensorData::setDate(string date_str)
{
    date = date_str;
}
inline void ForceSensorData::setLocation(string location_str)
{
    location = location_str;
}
inline void ForceSensorData::setPatternType(string patternType_str)
{
    patternType = patternType_str;
}
inline void ForceSensorData::setPatternLevel(int patternLevel_int)
{
    patternLevel = patternLevel_int;
}
inline void ForceSensorData::setClassificationGroup(int classificationGroup_int)
{
    classificationGroup = classificationGroup_int;
}
inline void ForceSensorData::setWorkspace(string workspace_str)
{
    workspace = workspace_str;
}
inline void ForceSensorData::setCoordinateSpace(string coordinateSpace_str)
{
    coordinateSpace = coordinateSpace_str;
}
inline void ForceSensorData::setTotalTime(double totalTime_dbl)
{
    totalTime = totalTime_dbl;
}
inline void ForceSensorData::setArm(bool arm_bool)
{
    arm = arm_bool;
}

//Vector methods
inline ForceSensorEvent& ForceSensorData::at(size_t pos)
{
    return events.at(pos);
}
inline const ForceSensorEvent& ForceSensorData::at(size_t pos) const
{
    return events.at(pos);
}
inline ForceSensorEvent& ForceSensorData::operator[](size_t pos)
{
    return events[pos];
}
inline const ForceSensorEvent& ForceSensorData::operator[](size_t pos) const
{
    return events[pos];
}
inline void ForceSensorData::push_back(const ForceSensorEvent& value)
{
    events.push_back(value);
}
inline void ForceSensorData::pop_back()
{
    events.pop_back();
}
inline bool ForceSensorData::empty() const
{
    return events.empty();
}
inline size_t ForceSensorData::size() const
{
    return events.size();
}
inline vector<ForceSensorEvent>::iterator ForceSensorData::begin()
{
    return events.begin();
}
inline vector<ForceSensorEvent>::const_iterator ForceSensorData::begin() const
{
    return events.begin();
}
inline vector<ForceSensorEvent>::iterator ForceSensorData::end()
{
    return events.end();
}
inline vector<ForceSensorEvent>::const_iterator ForceSensorData::end() const
{
    return events.end();
}
inline void ForceSensorData::clear()
{
    events.clear();
}

//Define YAML namespace to provide a method for preventing name conflicts
namespace YAML
{
    //Template
    template<>

    //Force Sensor Event conversion structur
    struct convert<ForceSensorEvent>
    {
        static Node encode(const ForceSensorEvent& rhs)
        {
            Node node;
            node.push_back(rhs.sensor1);
            node.push_back(rhs.sensor2);
            node.push_back(rhs.sensor3);
            node.push_back(rhs.sensor4);
            node.push_back(rhs.time);
            return node;
        }

        static bool decode(const Node& node, ForceSensorEvent& rhs)
        {
            if (!node.IsSequence() || node.size() != 5)
            {
                return false;
            }

            rhs.sensor1 = node[0].as<double>();
            rhs.sensor2 = node[1].as<double>();
            rhs.sensor3 = node[2].as<double>();
            rhs.sensor4 = node[3].as<double>();
            rhs.time = node[4].as<double>();
            return true;
        }
    };
}

#endif // FORCESENSORDATA_H
