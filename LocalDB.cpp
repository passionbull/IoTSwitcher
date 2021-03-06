#include "LocalDB.h"

    LocalDB::LocalDB()
    {
        Serial.println("created localDB");
        strcpy(mOn_angle, "0");
        strcpy(mOff_angle, "0");
        strcpy(mMiddle_angle, "0");
        strcpy(mAlarm_time_0, "000000000000");
        strcpy(mAlarm_time_1, "000000000000");
        strcpy(mAlarm_time_2, "000000000000");
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
                            int cntJson = 0;        
                            JsonObject::iterator it;
                            for(it = json.begin(); it !=json.end();++it){
                                // Serial.println(it->key);
                                // Serial.println(it->value);
                                cntJson++;
                            }
                            // check cntJson Size;
                            if(cntJson != 6){
                                configFile.close();
                                Serial.println("Loading is fail. Init restarts");
                                spiffs_init();
                                return;
                            }

                            Serial.println("\nparsed json");
                            strcpy(mOn_angle, json["on_angle"]);
                            strcpy(mOff_angle, json["off_angle"]);
                            strcpy(mMiddle_angle, json["middle_angle"]);
                            strcpy(mAlarm_time_0, json["alarm_0"]);
                            strcpy(mAlarm_time_1, json["alarm_1"]);
                            strcpy(mAlarm_time_2, json["alarm_2"]);
                    } 
                    else {
                        Serial.println("failed to load json config");
                    }
                    configFile.close();
                }
            }
            else {
                // if config.json is not exist, board should generate the file.
                spiffs_init();
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
            json["middle_angle"] = "30";
            json["alarm_0"] = "000000000000";//HHMMDDDDDDDX
            json["alarm_1"] = "000000000000";//HHMMDDDDDDDX
            json["alarm_2"] = "000000000000";//HHMMDDDDDDDX

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

    void LocalDB::printDBData() {
        Serial.print("mOn_angle: ");
        Serial.println(mOn_angle);
        Serial.print("mOff_angle: ");
        Serial.println(mOff_angle);
        Serial.print("mMiddle_angle: ");
        Serial.println(mMiddle_angle);
        Serial.print("mAlarm_time_0: ");
        Serial.println(mAlarm_time_0);
        Serial.print("mAlarm_time_1: ");
        Serial.println(mAlarm_time_1);
        Serial.print("mAlarm_time_2: ");
        Serial.println(mAlarm_time_2);
}
