#include "get_details.h"

void get_details()
{
    // get url
    String id = JSONDocument["id"];
    String url = "http://proiectia.bogdanflorea.ro/api/marvel-api/character?id=" + id;
    Serial.println(url);

    // Define the HTTP client
    HTTPClient http;

    // Define the connection parameters and, optionally,
    // set the connection timeout, especially if accessing
    // a server with a longer response time (such as a
    // free API server)
    http.begin(url);
    http.setConnectTimeout(30000); // connect timeout
    http.setTimeout(30000);        // response timeout

    // Send HTTP request
    // In this exemple, the GET method is used, according to
    // the API docs
    int httpResponseCode = http.GET();

    // Check response code
    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        // Get response data
        String payload = http.getString();

        // no need for new dynamicjsondocument, use the existing one used for data received from pia app
        DeserializationError error = deserializeJson(JSONDocument, payload.c_str());

        if (error)
        {
            Serial.println(error.c_str());
        }
        else
        {
            String id = JSONDocument["id"];
            String name = JSONDocument["name"];
            String thumbnail = JSONDocument["thumbnail"];

            JsonObject comics = JSONDocument["comics"];
            String comicsAvailable = comics["available"];
            String comicsCollectionURI = comics["collectionURI"];
            JsonObject items = comics["items"];

            String comicsLista = "";
            for(auto item : comics["items"].as<JsonArray>()) {
                String itemName = item["name"];
                comicsLista += "name: " + itemName + "\n";
                String itemResourceURI = item["resourceURI"];
                comicsLista += "resourceURI: " + itemResourceURI + "\n";
            }
            

            JsonObject series = JSONDocument["series"];
            String seriesAvailable = series["available"];
            String seriesCollectionURI = series["collectionURI"];
            String seriesLista = "";
            for(auto item : series["items"].as<JsonArray>()) {
                String itemName = item["name"];
                seriesLista += "name: " + itemName + "\n";
                String itemResourceURI = item["resourceURI"];
                seriesLista += "resourceURI: " + itemResourceURI + "\n";
            }

            JsonObject stories = JSONDocument["stories"];
            String storiesAvailable = stories["available"];
            String storiesCollectionURI = stories["collectionURI"];
            String storiesLista = "";
            for(auto item : stories["items"].as<JsonArray>()) {
                String itemName = item["name"];
                storiesLista += "name: " + itemName + "\n";
                String itemResourceURI = item["resourceURI"];
                storiesLista += "resourceURI: " + itemResourceURI + "\n";
                String itemType = item["type"];
                storiesLista += "type: " + itemType + "\n";
            }

            JsonObject events = JSONDocument["events"];
            String eventsAvailable = events["available"];
            String eventsCollectionURI = events["collectionURI"];
            String eventsLista = "";
            for(auto item : events["items"].as<JsonArray>()) {
                String itemName = item["name"];
                eventsLista += "name: " + itemName + "\n";
                String itemResourceURI = item["resourceURI"];
                eventsLista += "resourceURI: " + itemResourceURI + "\n";
            }

            DynamicJsonDocument JSONDocument_response(8192);

            JSONDocument_response["id"] = id;
            JSONDocument_response["name"] = name;
            JSONDocument_response["image"] = thumbnail.c_str();
            
            JSONDocument_response["description"] = "comics:\n" + comicsLista + "\n" +
                "series:\n" + seriesLista + "\n" +
                "stories:\n" + storiesLista + "\n" +
                "events:\n" + eventsLista + "\n";

            JSONDocument_response["teamId"] = teamID;

            String get_details_response = "";
            serializeJson(JSONDocument_response, get_details_response);

            // delay(500);

            SerialBT.println(get_details_response);
            Serial.println(get_details_response);

            // delay(500);
        }
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}