//HapticTraceData.h
#ifndef HAPTIC_TRACE_DATA_H
#define HAPTIC_TRACE_DATA_H

//Include standard files
//Header providing file stream classes
#include <fstream>
//Defines the standard input/output stream objects
#include <iostream>
//Provides string stream classes
#include <sstream>
//Provides parametric manipulators
#include <iomanip>
//Introduces random number generation facilities
#include <random>
//Defines a collection of functions especially designed to be used on ranges of elements
#include <algorithm>
//Defines the vector container class
#include <vector>
//Introduces string types, character traits and a set of converting functions
#include <string>

//3rd party libraries
#include <yaml-cpp/yaml.h>

//Standard namespace
using namespace std;

//Haptic Trace Event structure
struct HapticTraceEvent
{
    //Haptic trace event default Constructor
    HapticTraceEvent(): x(0), y(0), z(0), time(0)
    {
    }

    //Haptic trace event parameterized Constructor
    HapticTraceEvent(double X, double Y, double Z, double Time): x(X), y(Y), z(Z), time(Time)
    {
    }

    //Operator "-" overloaded
    HapticTraceEvent operator-(const HapticTraceEvent& rhs) const
    {
        return HapticTraceEvent(x-rhs.x, y-rhs.y, z-rhs.z, time-rhs.time);
    }

    //Operator "+" overloaded
    HapticTraceEvent operator+(const HapticTraceEvent& rhs) const
    {
        return HapticTraceEvent(x+rhs.x, y+rhs.y, z+rhs.z, time+rhs.time);
    }

    //Operator "*" overloaded
    HapticTraceEvent operator*(const double rhs) const
    {
        return HapticTraceEvent(x*rhs,y*rhs,z*rhs,time*rhs);
    }

    //Operator "+=" overloaded
    HapticTraceEvent& operator+=(const HapticTraceEvent& rhs)
    {
        x = x + rhs.x;
        y = y + rhs.y;
        z = z + rhs.z;
        time = time + rhs.time;

        return *this;
    }

    //Event data
    double x, y, z, time;
};

//Haptic Trace Data class
class HapticTraceData
{
    //Public access members (members are accesible from outside the class)
    public:

        //Haptic trace data default Constructor
        HapticTraceData();
        //Haptic trace data parameterized Constructor
        HapticTraceData(const string& traceFile);

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

        //Vector methods
        //Returns a reference to the element at the position specified in the vector
        HapticTraceEvent& at(size_t pos);
        const HapticTraceEvent& at(size_t pos) const;
        //Returns a reference to the element at the position specified in the vector container
        HapticTraceEvent& operator[](size_t pos);
        const HapticTraceEvent& operator[](size_t pos) const;
        //Adds a new element (value) at the end of the vector, after its current last element
        void push_back(const HapticTraceEvent& value);
        //Removes the last element in the vector, effectively reducing the container size by one
        void pop_back();
        //Returns whether the vector is empty (size = 0)
        bool empty() const;
        //Returns the number of elements in the vector
        size_t size() const;
        //Returns an iterator pointing to the first element in the vector
        vector<HapticTraceEvent>::iterator begin();
        vector<HapticTraceEvent>::const_iterator begin() const;
        //Returns an iterator referring to the past-the-end element in the vector container
        vector<HapticTraceEvent>::iterator end();
        vector<HapticTraceEvent>::const_iterator end() const;
        //Removes all the elements from the vector, leaving the container with a size of 0
        void clear();

    //Private access members (members are not accesible (or viewed) from outside the class)
    private:

        //Find the minimun and maximum bounds
        void findMinMaxBounds();

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

        //Vector with trace events (Haptic x, y, z and sampling time)
        vector<HapticTraceEvent> events;
        HapticTraceEvent maxBound;
        HapticTraceEvent minBound;
};

//Inline methods (more efficient at runtime)

//Access methods
inline string HapticTraceData::getPatientID() const
{
    return patientID;
}

inline string HapticTraceData::getDate() const
{
    return date;
}

inline string HapticTraceData::getLocation() const
{
    return location;
}

inline string HapticTraceData::getPatternType() const
{
    return patternType;
}

inline int HapticTraceData::getPatternLevel() const
{
    return patternLevel;
}

inline int HapticTraceData::getClassificationGroup() const
{
    return classificationGroup;
}

inline string HapticTraceData::getWorkspace() const
{
    return workspace;
}

inline string HapticTraceData::getCoordinateSpace() const
{
    return coordinateSpace;
}

inline double HapticTraceData::getTotalTime() const
{
    return totalTime;
}

//Modifier methods
inline void HapticTraceData::setPatientID(string patientID_str)
{
    patientID = patientID_str;
}

inline void HapticTraceData::setDate(string date_str)
{
    date = date_str;
}

inline void HapticTraceData::setLocation(string location_str)
{
    location = location_str;
}

inline void HapticTraceData::setPatternType(string patternType_str)
{
    patternType = patternType_str;
}

inline void HapticTraceData::setPatternLevel(int patternLevel_int)
{
    patternLevel = patternLevel_int;
}

inline void HapticTraceData::setClassificationGroup(int classificationGroup_int)
{
    classificationGroup = classificationGroup_int;
}

inline void HapticTraceData::setWorkspace(string workspace_str)
{
    workspace = workspace_str;
}

inline void HapticTraceData::setCoordinateSpace(string coordinateSpace_str)
{
    coordinateSpace = coordinateSpace_str;
}

inline void HapticTraceData::setTotalTime(double totalTime_dbl)
{
    totalTime = totalTime_dbl;
}

//Vector methods
inline HapticTraceEvent& HapticTraceData::at(size_t pos)
{
    return events.at(pos);
}

inline const HapticTraceEvent& HapticTraceData::at(size_t pos) const
{
    return events.at(pos);
}

inline HapticTraceEvent& HapticTraceData::operator[](size_t pos)
{
    return events[pos];
}

inline const HapticTraceEvent& HapticTraceData::operator[](size_t pos) const
{
    return events[pos];
}

inline void HapticTraceData::push_back(const HapticTraceEvent& value)
{
    events.push_back(value);
}

inline void HapticTraceData::pop_back()
{
    events.pop_back();
}

inline bool HapticTraceData::empty() const
{
    return events.empty();
}

inline size_t HapticTraceData::size() const
{
    return events.size();
}

inline vector<HapticTraceEvent>::iterator HapticTraceData::begin()
{
    return events.begin();
}

inline vector<HapticTraceEvent>::const_iterator HapticTraceData::begin() const
{
    return events.begin();
}

inline vector<HapticTraceEvent>::iterator HapticTraceData::end()
{
    return events.end();
}

inline vector<HapticTraceEvent>::const_iterator HapticTraceData::end() const
{
    return events.end();
}

inline void HapticTraceData::clear()
{
    events.clear();
}

//Define YAML namespace to provide a method for preventing name conflicts
namespace YAML
{
    //Template
    template<>

    //Haptic Trace Event conversion structure
    struct convert<HapticTraceEvent>
    {
        //x, y, z, time encoded as [x, y, z, time] in YAML
        static Node encode(const HapticTraceEvent& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.time);
            return node;
        }

        static bool decode(const Node& node, HapticTraceEvent& rhs)
        {
            if(!node.IsSequence() || node.size() != 4)
            {
                return false;
            }

            rhs.x = node[0].as<double>();
            rhs.y = node[1].as<double>();
            rhs.z = node[2].as<double>();
            rhs.time = node[3].as<double>();
            return true;
        }
    };
}
#endif //TRACE_DATA_H
