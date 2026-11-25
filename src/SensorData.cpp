//SensorData.cpp

//Include the user-defined needed files
//Sensor data header
#include "SensorData.h"

//Sensor data default Constructor
SensorData::SensorData(): totalTime(0)
{
}

//New Sensor data (parameterized Constructor)
SensorData::SensorData(const string& traceFile)
{
    //Load YAML file
    loadYamlFile(traceFile);
}

//Load YAML file
void SensorData::loadYamlFile(const string& traceFile)
{
    //Create file with single native data structure
    YAML::Node doc = YAML::LoadFile(traceFile);

    //Save Sensor filename for reference
    filename = traceFile;

    //Clear sensor vector events (wrist_flex_ext, wrist_abd_add, elbow_flex_ext, elbow_pron_sup, shoulder_flex_ext, shoulder_abd_add, shoulder_ext_int_rot, and sampling time)
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
        events.push_back(eventsNode[i].as<SensorEvent>());
    }
}

//Export YAML File with Sensor data
void SensorData::exportYamlFile(const string& traceFile) const
{
    //Output Sensor file stream
    ofstream file(traceFile);

    if(getArm() == 0)
    {
        //Additional test data
        file << "%YAML 1.2" << ";" << "Right arm\n" << exportYamlString() << endl;
    }
    else
    {
        //Additional test data
        file << "%YAML 1.2" << ";" << "Left arm\n" << exportYamlString() << endl;
    }

    //Sensor Test data
    file << "WRIST_FLEX_EXT" << ";" << "WRIST_ABD_ADD" << ";" << "ELBOW_FLEX_EXT" << ";" << "ELBOW_PRON_SUP" << ";" << "SHOULDER_FLEX_EXT" << ";" << "SHOULDER_ABD_ADD" << ";" << "SHOULDER_EXT_INT_ROT" << ";" << "SAMPLING_TIME" << endl;
    //Loop to save values to file
    for(vector<SensorEvent>::const_iterator it = events.begin(); it != events.end(); it++)
    {
        //Values saved with 4 decimal precision
        file << fixed << setprecision(4) << it -> wrist_flex_ext << ";" << it -> wrist_abd_add << ";" << it -> elbow_flex_ext << ";" << it -> elbow_pron_sup << ";" << it -> shoulder_flex_ext << ";" << it -> shoulder_abd_add << ";" << it -> shoulder_ext_int_rot << ";" << it -> time << endl;
    }

    //Close file
    file.close();
}

//Export Sensor additional data
string SensorData::exportYamlString() const
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
