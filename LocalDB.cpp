#include "LocalDB.h"

    LocalDB::LocalDB()
    {

    }
    LocalDB::~LocalDB()
    {

    }
    void LocalDB::spiffs_reading()
    {
        if (SPIFFS.begin()) 
        {
            // Serial.println("reading file system");
            if (SPIFFS.exists("/config.json")) 
            {
                //file exists, reading and loading
                // Serial.println("reading config file");
                File configFile = SPIFFS.open("/config.json", "r");
                if (configFile) {
                    // Serial.println("opened config file");
                    size_t size = configFile.size();
                    // Allocate a buffer to store contents of the file.
                    std::unique_ptr<char[]> buf(new char[size]);

                    configFile.readBytes(buf.get(), size);
                    DynamicJsonBuffer jsonBuffer;
                    JsonObject& json = jsonBuffer.parseObject(buf.get());
                    json.printTo(Serial);
                    
                    if (json.success()) 
                    {
                        Serial.println("\nparsed json");
                        strcpy(mOn_angle, json["on_angle"]);
                        strcpy(mOff_angle, json["off_angle"]);
                        strcpy(mAlarm_time, json["alarm_time"]);
                    } 
                    else {
                        Serial.println("failed to load json config");
                    }
                    configFile.close();
                }
            }
        }        
    }

    void LocalDB::spiffs_writing(String id, String val)
    {
        if (SPIFFS.begin()) 
        {
            File configFile = SPIFFS.open("/config.json", "r");
            if (configFile) {
                // Serial.println("opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.parseObject(buf.get());
                configFile.close();


                json[id.c_str()] = val.c_str();
                configFile = SPIFFS.open("/config.json", "w");
                if (!configFile) {
                    Serial.println("failed to open config file for writing");
                }
                // Serial.println("writing file system");
                json.printTo(Serial);
                json.printTo(configFile);
                configFile.close();


            }            
           

        }
    }

    void LocalDB::spiffs_init()
    {
        if (SPIFFS.begin()) 
        {
            DynamicJsonBuffer jsonBuffer;
            JsonObject& json = jsonBuffer.createObject();
            json["on_angle"] = "0";
            json["off_angle"] = "60";
            json["alarm_time"] = "n0000";

            File configFile = SPIFFS.open("/config.json", "w");
            if (!configFile) {
                Serial.println("failed to open config file for writing");
            }
            Serial.println("writing file system");
            json.printTo(Serial);
            json.printTo(configFile);
            configFile.close();
        }
    }