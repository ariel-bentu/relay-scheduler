#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "sessionTypes.h"

ESP8266WebServer serverAP(80);
// const byte DNS_PORT = 53;
IPAddress apIP(10,25,1,1);
// DNSServer dnsServer;

void reconnectAP() {
    WiFi.softAPdisconnect();
    WiFi.softAP(gStore.wifiConfig.ssidAP, gStore.wifiConfig.pwdAP);
}

void initServerAP() {
    WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
    WiFi.softAP(gStore.wifiConfig.ssidAP, gStore.wifiConfig.pwdAP);

    serverAP.on("/", handleRoot);
    serverAP.onNotFound([]() {
        serverAP.send(404, "text/html", "Not Found!");
    });
    serverAP.on("/on", handleOn);
    serverAP.on("/refresh-plan", handleRefreshPlan);
    serverAP.on("/off", handleOff);
    serverAP.on("/setup-wifi", HTTP_GET, handleWifiSetupHTML);
    serverAP.on("/setup-wifi", HTTP_POST, handleSetWiFiConfig);
    serverAP.on("/setup-cal", HTTP_GET, handleCelendarSetupHTML);
    serverAP.on("/setup-cal", HTTP_POST, handleSetCalConfig);
    serverAP.on("/disable", HTTP_GET, handleGetDisable);
    serverAP.on("/disable", HTTP_POST, handlePostDisable);
    serverAP.begin();
}

void loopAP() {
    serverAP.handleClient();
}

void handleRoot() {
    
    //<a href='/on?relay=1'>On relay 1</a><br/> \
    //<a href='/off?relay=1'>Off relay 1</a><br/>");
    int min = getMinFromWeekStart();
    String relays = "<h2>Relays:</h2>\n";
    for (int i = 0; i< MAX_RELAYS ;i++) {
        relays += "Relay ";
        relays += (i+1);
        relays += ": <a href='/on?relay=";
        relays += (i+1);
        relays += "'>On</a>";
        relays += "   <a href='/off?relay=";
        relays += (i+1);
        relays += "'>Off</a>";
        if (gOveride[i].until > 0) {
            relays += " - Override ";
            relays += (gOveride[i].on ? "on " : "off ");
            relays.concat(min - gOveride[i].until);
            relays += " more minutes";
        }
        relays += "<br/>\n";
    }

    String status = "<h2>Status</h2>\n";
    if (WiFi.status() == WL_CONNECTED) {
        status += "Connected to " + WiFi.SSID();
        status += "\n<br/>";
    } else if (strlen(gStore.wifiConfig.ssid) == 0) {
        status += "WiFi not set.\n<br/>";
    }

    const char head[] =  "<h1>Automation Site</h1>\n";


    const char menu[] =  "<h2>Actions</h2>\n" \
    "<a href='/setup-wifi'>Setup WIFI and AccessPoint ...</a><br/>" \
    "<a href='/setup-cal'>Setup Google Calendar ...</a><br/>" \
    "<a href='/refresh-plan'>Reload plan from calendar</a><br/>" \
    "<a href='/disable'>Disable system temporarily...</a><br/>";
    serverAP.setContentLength(strlen(menu) + strlen(head) + relays.length() + status.length());
    serverAP.send(200, "text/html", head);
    serverAP.sendContent(status);
    serverAP.sendContent(menu);
    serverAP.sendContent(relays);
}

void handleWifiSetupHTML() {
    String setup = "\n<h1>Setup WIFI</h1><br/>" \
    "<form action='/setup-wifi' method='POST'>" \
    "Wifi SSID: <input name='ssid' type='text' value='";
    setup += gStore.wifiConfig.ssid;
    setup += "' /><br/>" \
    "Wifi Password: <input name='pwd' type='password' value='";
    setup += gStore.wifiConfig.pwd;
    setup += "' /><br/><br/>" \
    "Device Access Point (AP):<br/>" \
    "Device AP SSID: <input name='ssidAP' type='text' value='";
    setup += gStore.wifiConfig.ssidAP;
    setup += "' /><br/>" \
    "Device AP Password: <input name='pwdAP' type='password' value='";
    setup += gStore.wifiConfig.pwdAP;
    setup += "' /><br/><br/>" \
    "<input type='submit' value='Update' />" \
    "</form>";
    
    serverAP.send(200, "text/html", setup);
}

void handleCelendarSetupHTML() {
    String setup = "\n<h1>Setup Calendar</h1><br/>" \
    "<form action='/setup-cal' method='POST'>" \
    "Calendar Name: <input name='name' type='text' value='";
    setup += gStore.calConfig.name;
    setup += "' /><br/>" \
    "Function Name: <input name='func'  value='";
    setup += gStore.calConfig.funcName;
    setup += "' /><br/><br/>" \
    "<input type='submit' value='Update'/></form>";
    
    serverAP.send(200, "text/html", setup);
}

void handleOn() {
    handleOnOff(true);
}

void handleOff() {
    handleOnOff(false);
}

void handleOnOff(bool on) {
    String argName("relay");

    if (serverAP.hasArg(argName)) {
        String val = serverAP.arg(argName);
        int ID = val.toInt();
        String message = "Relay ";
        message.concat(ID);
        message += " turned ";
        message += on? "ON!" : "OFF!";


        if (ID < 1 || ID > MAX_RELAYS) {
            message += " ID out of range";
        } else {
            bool doIt = true;
            if (gOveride[ID-1].until > 0) {
                if (gOveride[ID-1].on == on) {
                    message += " - Already in this state";
                    doIt = false;
                }
            }
            if (doIt) {
                gOveride[ID-1].on = on;
                gOveride[ID-1].until = getMinFromWeekStart() + DEFAULT_ADHOC_MIN;
            }

        }
        //turnOnOffRelay(ID, on);
        
        SERIAL_ln(message);
        serverAP.send(200, "text/plain", message); 
    }

}


void handleRefreshPlan() {
    String msg;
    bool success = readPlan(msg);
    serverAP.send(success?200:500, "text/plain", msg.c_str()); 
}

void handleSetWiFiConfig() {
    for (int i = 0; i < serverAP.args(); i++) {
        if (strcmp(serverAP.argName(i).c_str(), "ssid") == 0) {
            strcpy(gStore.wifiConfig.ssid, serverAP.arg(i).c_str());
        } else if (strcmp(serverAP.argName(i).c_str(), "pwd") == 0) {
            strcpy(gStore.wifiConfig.pwd, serverAP.arg(i).c_str());
        } else if (strcmp(serverAP.argName(i).c_str(), "ssidAP") == 0) {
            strcpy(gStore.wifiConfig.ssidAP, serverAP.arg(i).c_str());
        } else if (strcmp(serverAP.argName(i).c_str(), "pwdAP") == 0) {
            strcpy(gStore.wifiConfig.pwdAP, serverAP.arg(i).c_str());
        }
    }
    saveStore();
    WifiConnect();
    reconnectAP();

    sendOkBack();
}

void handleSetCalConfig() {
    for (int i = 0; i < serverAP.args(); i++) {
        if (strcmp(serverAP.argName(i).c_str(), "name") == 0) {
            strcpy(gStore.calConfig.name, serverAP.arg(i).c_str());
        } else if (strcmp(serverAP.argName(i).c_str(), "func") == 0) {
            strcpy(gStore.calConfig.funcName, serverAP.arg(i).c_str());
        }
    }
    saveStore();

    sendOkBack();
}

void handleGetDisable() {
    String setup = "\n<h1>Disable system</h1><br/>" \
    "<form action='/disable' method='POST'>" \
    "How long (days): <input name='days' type='text' value='1' />"\
    "<input type='submit' value='Update' />" \
    "</form>";
    
    serverAP.send(200, "text/html", setup);
}

void handlePostDisable() {
    for (int i = 0; i < serverAP.args(); i++) {
        if (strcmp(serverAP.argName(i).c_str(), "days") == 0) { 
            int days = atoi(serverAP.arg(i).c_str());
            int min = (days == 0 ? 0 : (getMinFromWeekStart() + days*24*60));
            for (int j = 0;j< MAX_RELAYS;j++) {
                gOveride[j].on = false;
                gOveride[j].until = min;
            }
        }
    }

    sendOkBack();
}

void sendOkBack() {
    serverAP.send(200, "text/html", "Successfully saved<br><a href='/'>Back...</a>");
}