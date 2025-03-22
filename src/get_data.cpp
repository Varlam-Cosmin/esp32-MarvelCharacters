#include "get_data.h"

void get_data()
{
    // Define the HTTP client
    HTTPClient http;

    // URL
    String url = "http://proiectia.bogdanflorea.ro/api/marvel-api/characters";

    // Define the connection parameters and, optionally,
    // set the connection timeout, especially if accessing
    // a server with a longer response time (such as a
    // free API server)
    http.begin(url);
    http.setConnectTimeout(30000); // connect timeout
    http.setTimeout(30000);        // response timeout

    // Send HTTP request
    int httpResponseCode = http.GET();

    // Check response code
    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        // Get response data
        String payload = http.getString();

        // no need for new dynamicjsondocument, use the existing one used for data received from pia app
        // apperently, JSONDocument.clear() is called implicitly by deserializejson
        DeserializationError error = deserializeJson(JSONDocument, payload.c_str());

        if (error)
        {
            Serial.println(error.c_str());
        }
        else
        {
            // Define a JsonArray from the JSONDocument, since the JSONString is an array of objects
            JsonArray list = JSONDocument.as<JsonArray>();

            // Iterate the JsonArray array
            for (JsonVariant value : list)
            {
                // Get the current item in the iterated list as a JsonObject
                JsonObject listItem = value.as<JsonObject>();

                // Create JSON Document response
                DynamicJsonDocument JSONDocument_response(4096);

                JSONDocument_response["id"] = listItem["id"];
                JSONDocument_response["name"] = listItem["name"];
                String thumbnail = listItem["thumbnail"];
                JSONDocument_response["image"] = thumbnail.c_str();
                JSONDocument_response["teamId"] = teamID;

                // Serialize json response document & send to pia app
                String get_data_json = "";
                serializeJson(JSONDocument_response, get_data_json);
                Serial.println(get_data_json);
                SerialBT.println(get_data_json);
                //delay(500);
            }
        }
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources - terminate the http object
    http.end();
}