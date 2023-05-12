#include <WiFi.h>


bool wifiBegan = false;

void initWifi() {
    WiFi.mode(WIFI_AP_STA);
    delay(100);

    if (strlen(gStore.wifiConfig.ssid) > 0)
    {
        WifiConnect();
    }
    else
    {
        SERIAL_ln("Wifi not configured yet!");
    }
}

bool WifiConnect()
{
    SERIAL_("Connecting to...");
    SERIAL_ln(gStore.wifiConfig.ssid);

    WiFi.begin(gStore.wifiConfig.ssid, gStore.wifiConfig.pwd);
    wifiBegan = true;
    int c = 0;
    while (c < 10 && WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        c++;
        SERIAL_(".");
    }
    SERIAL_ln("");
    if (WiFi.status() == WL_CONNECTED)
    {
        SERIAL_f("%s connected successfully\n", WiFi.SSID().c_str());
        SERIAL_("IP address:\t");
        SERIAL_(WiFi.localIP());
        return true;
    }
    else
    {
        SERIAL_f("Failed to conn '%s' (%d)\n", gStore.wifiConfig.ssid, WiFi.status());
        return false;
    }
}