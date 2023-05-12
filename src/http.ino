#include <WiFi.h>
#include <HTTPClient.h>
#include "sessionTypes.h"
//#include "wemosD1Mini.h"
//#include <HTTPSRedirect.h>
#include "config.h"

bool getUrl(const char *url, String &result)
{
    SERIAL_("GET:");
    SERIAL_(host);
    SERIAL_ln(url);

    if (WiFi.status() != WL_CONNECTED)
    {
        if (!WifiConnect())
        {
            SERIAL_f("Wifi not conn. %d\n", WiFi.status());
            return false;
        }
    }

    HTTPClient http;

    // Replace with the URL of the website you want to request
    String fullUrl = "https://";
    fullUrl += host;
    fullUrl += url;
    
    http.begin(fullUrl);

    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    if (httpCode > 0)
    {
        result = http.getString();
        Serial.println("Received payload:");
        Serial.println(result);
    }
    else
    {
        Serial.printf("HTTP error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

    return httpCode > 0;
}

/*

bool getUrl(const char *url, String &result)
{
    SERIAL_("GET:");
    SERIAL_(host);
    //SERIAL_("/");
    SERIAL_ln(url);

    if (WiFi.status() != WL_CONNECTED)
    {
        if (!WifiConnect())
        {
            SERIAL_f("Wifi not conn. %d\n", WiFi.status());
            return false;
        }
    }

    HTTPSRedirect client;

    client.setInsecure();

    // Try to connect for a maximum of 5 times
    int res = -1;
    for (int i = 0; i < 5; i++)
    {
        res = client.connect(host, 443);

        if (res == 0)
        {
            break;
        }
        else
        {
            SERIAL_ln("Connection await...");
            delay(100);
        }
    }
    if (!res)
    {
        SERIAL_("Connect error:");
        SERIAL_ln(res);
        return false;
    }
    SERIAL_ln("Connected!");

    if (!client.GET(String(url), host))
    {
        SERIAL_("HTTP error:");
        SERIAL_(client.getStatusCode());
        SERIAL_(",");
        SERIAL_ln(client.getReasonPhrase());
        return false;
    }

    result = client.getResponseBody();
    SERIAL_ln("");
    SERIAL_("sent: ");
    SERIAL_ln(url);
    SERIAL_ln(result);

    return true;
}
*/