#include "ArduinoJson/ArduinoJson.h"
#include <iostream>

using namespace std;

int jsonrpc_debug(char * jsonrpc) {

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(jsonrpc);

    string jsonrcp_version = root["jsonrcp"];
    string method = root["method"];
    JsonVariant params = root["params"];
    long id = root["id"];


    if (!root.success()) {
        cout << "Not a valid JSON object" << endl;
        return -1;
    }

    if (jsonrcp_version != "2.0"  || !method.length() ) {
        cout << "Not a valid JSONRPC object" << endl;
        return -1;
    }

    cout << "----------JSONRPC-----------" << endl;
    cout << "Version: " << jsonrcp_version << endl;

    if (id) {
        cout << "ID: " << id << endl;
    } else {
        cout << "Notification" << endl;
    }

    cout << "Method: " << method << endl;
    if (params.is<JsonArray>()) {
        cout << "Parameter Array" << endl;
        int i = 0;
        for( const auto& kv : params.as<JsonObject>() ) {
            cout << "[" << kv.key << "] " << kv.value.as<char*>() << endl;
        }
    }
    if (params.is<JsonObject>()) {
        cout << "Parameter Object" << endl;
        for( const auto& kv : params.as<JsonObject>() ) {
            cout << kv.key << " -> " << kv.value.as<char*>() << endl;
        }
    }


    cout << "-------ENDJSONRPC-----------" << endl;
    return 0;
}

string jsonrpc_handler(string jsonrpc) {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(jsonrpc);

    string jsonrcp_version = root["jsonrcp"];
    string method = root["method"];
    JsonVariant params = root["params"];
    long id = root["id"];


    if (!root.success()) {
        cout << "Not a valid JSON object" << endl;
        return -1;
    }
    
    if (jsonrcp_version != "2.0"  || !method.length() ) {
        cout << "Not a valid JSONRPC object" << endl;
        return -1;
    }
    
    if(method == "getTemp") {
        cout << "Temp was requested" << endl;
        root["result"] = 22.52;
    }
    
    string output;
    root.printTo<string>(output);
    return output;
}