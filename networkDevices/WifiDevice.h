#pragma once

#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <Preferences.h>
#include "NetworkDevice.h"
#include "../SpiffsCookie.h"

class WifiDevice : public NetworkDevice
{
public:
    WifiDevice(const String& hostname, Preferences* _preferences);

    virtual void initialize();
    virtual void reconfigure();
    virtual bool reconnect();
    virtual void printError();

    virtual void update();

    virtual bool isConnected();

    virtual PubSubClient *mqttClient();

private:
    WiFiClient* _wifiClient = nullptr;
    WiFiClientSecure* _wifiClientSecure = nullptr;
    PubSubClient* _mqttClient = nullptr;
    SpiffsCookie _cookie;

    String _wifiSSID = "";
    String _wifiPSK = "";

    char _ca[TLS_CA_MAX_SIZE];
    char _cert[TLS_CERT_MAX_SIZE];
    char _key[TLS_KEY_MAX_SIZE];
};
