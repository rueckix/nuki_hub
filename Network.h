#pragma once

#include <PubSubClient.h>
#include "networkDevices/NetworkDevice.h"
#include "networkDevices/WifiDevice.h"
#include "networkDevices/W5500Device.h"
#include <Preferences.h>
#include <vector>
#include "NukiConstants.h"
#include "SpiffsCookie.h"
#include "NukiLockConstants.h"

enum class NetworkDeviceType
{
    WiFi,
    W5500
};

class Network
{
public:
    explicit Network(const NetworkDeviceType networkDevice, Preferences* preferences);
    virtual ~Network();

    void initialize();
    void update();
    void setupDevice(const NetworkDeviceType hardware);
    void initializeW5500();

    bool isMqttConnected();

    void publishKeyTurnerState(const NukiLock::KeyTurnerState& keyTurnerState, const NukiLock::KeyTurnerState& lastKeyTurnerState);
    void publishAuthorizationInfo(const uint32_t authId, const char* authName);
    void publishCommandResult(const char* resultStr);
    void publishBatteryReport(const NukiLock::BatteryReport& batteryReport);
    void publishConfig(const NukiLock::Config& config);
    void publishAdvancedConfig(const NukiLock::AdvancedConfig& config);
    void publishPresenceDetection(char* csv);

    void setLockActionReceivedCallback(bool (*lockActionReceivedCallback)(const char* value));
    void setConfigUpdateReceivedCallback(void (*configUpdateReceivedCallback)(const char* path, const char* value));
    void setMqttDataReceivedForwardCallback(void (*callback)(char*, uint8_t*, unsigned int));

    void restartAndConfigureWifi();

    NetworkDevice* device();

private:
    static void onMqttDataReceivedCallback(char* topic, byte* payload, unsigned int length);
    void onMqttDataReceived(char*& topic, byte*& payload, unsigned int& length);
    bool comparePrefixedPath(const char* fullPath, const char* subPath);

    void publishFloat(const char* topic, const float value, const uint8_t precision = 2);
    void publishInt(const char* topic, const int value);
    void publishUInt(const char* topic, const unsigned int value);
    void publishBool(const char* topic, const bool value);
    bool publishString(const char* topic, const char* value);

    void buildMqttPath(const char* path, char* outPath);
    void subscribe(const char* path);

    bool reconnect();

    NetworkDevice* _device = nullptr;
    Preferences* _preferences;
    String _hostname;

    bool _mqttConnected = false;

    unsigned long _nextReconnect = 0;
    char _mqttBrokerAddr[101] = {0};
    char _mqttPath[181] = {0};
    char _mqttUser[31] = {0};
    char _mqttPass[31] = {0};
    int _networkTimeout = 0;

    unsigned long _lastConnectedTs = 0;

    char* _presenceCsv = nullptr;

    std::vector<char*> _configTopics;

    bool _firstTunerStatePublish = true;

    bool (*_lockActionReceivedCallback)(const char* value) = nullptr;
    void (*_configUpdateReceivedCallback)(const char* path, const char* value) = nullptr;
    void (*_mqttTopicReceivedForwardCallback)(char*, uint8_t*, unsigned int)  = nullptr;
};
