//HapticTraceData.cpp

//Include the user-defined needed files
//Haptic trace data header
#include "HapticTraceData.h"

//Haptic trace data default Constructor
HapticTraceData::HapticTraceData(): totalTime(0)
{
}

//New Haptic trace data (parameterized Constructor)
HapticTraceData::HapticTraceData(const string& traceFile)
{
    //Load YAML file
    loadYamlFile(traceFile);
}

//Load YAML file
void HapticTraceData::loadYamlFile(const string& traceFile)
{
    //Create file with single native data structure
    YAML::Node doc = YAML::LoadFile(traceFile);

    //Save Haptic filename for reference
    filename = traceFile;

    //Clear haptic vector trace events (haptic x, y, z and sampling time)
    events.clear();

    //Load metadata
    patientID = doc["Patient-id"].as<std::string>();
    date = doc["Date"].as<std::string>();
    location = doc["Location"].as<std::string>();

    if(doc["Pattern"])
    {
        patternType = doc["Pattern"]["Type"].as<std::string>();
        patternLevel = doc["Pattern"]["Level"].as<int>();
    }

    if(doc["Classification"])
    {
        classificationGroup = doc["Classification"]["Group"].as<int>();
    }
    else
    {
        classificationGroup = 0;
    }

    workspace = doc["Workspace"].as<std::string>();
    coordinateSpace = doc["Coordinate-space"].as<std::string>();
    totalTime = doc["Total-time"].as<double>();

    //Load events metadata
    const YAML::Node& eventsNode = doc["Events"];

    for(size_t i = 1; i < eventsNode.size(); i++)
    {
        events.push_back(eventsNode[i].as<HapticTraceEvent>());
    }

    //Find the minimun and maximum bounds
    findMinMaxBounds();
}

//Find the minimun and maximum bounds
void HapticTraceData::findMinMaxBounds()
{
    //First element in the haptic vector trace events
    maxBound = minBound = events.front();

    //Last sample time record
    maxBound.time = events.back().time;

    //Find min and max for each event spacial dimension
    for(vector<HapticTraceEvent>::const_iterator it = events.begin(); it != events.end(); it++)
    {
        //To find de maximun and minimun bounds on the Haptic X coordinate
        if(it->x > maxBound.x)
        {
            maxBound.x = it->x;
        }
        else if(it->x < minBound.x)
        {
            minBound.x = it->x;
        }

        //To find de maximun and minimun bounds on the Haptic Y coordinate
        if(it->y > maxBound.y)
        {
            maxBound.y = it->y;
        }
        else if(it->y < minBound.y)
        {
            minBound.y = it->y;
        }

        //To find de maximun and minimun bounds on the Haptic Z coordinate
        if(it->z > maxBound.z)
        {
            maxBound.z = it->z;
        }
        else if(it->z < minBound.z)
        {
            minBound.z = it->z;
        }
    }
}

//Export YAML File with Haptic trace data
void HapticTraceData::exportYamlFile(const string& traceFile) const
{
    //Output Haptic file stream
    ofstream file(traceFile);

    //Additional test data
    file << "%YAML 1.2\n" << exportYamlString() << endl;

    //Haptic Test data
    file << "HAPTIC_X" << ";" << "HAPTIC_Y" << ";" << "HAPTIC_Z" << ";" << "SAMPLING_TIME" << endl;
    //Loop to save values to file
    for(vector<HapticTraceEvent>::const_iterator it = events.begin(); it != events.end(); it++)
    {
        //Values saved with 4 decimal precision
        file << fixed << setprecision(4) << it -> x << ";" << it -> y << ";" << it -> z << ";" << it ->time << endl;
    }

    //Close file
    file.close();
}

//Export Haptic trace additional data
string HapticTraceData::exportYamlString() const
{
    //Output stream object
    YAML::Emitter out;

    out << YAML::BeginDoc
            << YAML::BeginMap
                << YAML::Key << "Patient-id" << YAML::Value << getPatientID()
                << YAML::Key << "Date" << YAML::Value << getDate()
                << YAML::Key << "Location" << YAML::Value << getLocation()

                << YAML::Key << "Pattern"
                << YAML::BeginMap
                    << YAML::Key << "Type" << YAML::Value << getPatternType()
                    << YAML::Key << "Level" << YAML::Value << getPatternLevel()
                << YAML::EndMap

                << YAML::Key << "Classification"
                << YAML::BeginMap
                    << YAML::Key << "Group" << YAML::Value << getClassificationGroup()
                << YAML::EndMap

                << YAML::Key << "Workspace" << YAML::Value << getWorkspace()
                << YAML::Key << "Coordinate-space" << YAML::Value << getCoordinateSpace()
                << YAML::Key << "Total-time" << YAML::Value << getTotalTime()

                << YAML::Key << "Events:";

    out << YAML::EndMap;

    return out.c_str();
}
