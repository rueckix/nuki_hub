#pragma once

#include "Network.h"
#include "NukiConstants.h"
#include "NukiDataTypes.h"
#include "BleScanner.h"
#include "NukiLock.h"

class NukiWrapper : public Nuki::SmartlockEventHandler
{
public:
    NukiWrapper(const std::string& deviceName, uint32_t id, BleScanner::Scanner* scanner, Network* network, Preferences* preferences);
    virtual ~NukiWrapper();

    void initialize();
    void update();

    void setPin(const uint16_t pin);

    void unpair();

    const NukiLock::KeyTurnerState& keyTurnerState();
    const bool isPaired();

    void notify(Nuki::EventType eventType) override;

private:
    static bool onLockActionReceivedCallback(const char* value);
    static void onConfigUpdateReceivedCallback(const char* topic, const char* value);
    void onConfigUpdateReceived(const char* topic, const char* value);

    void updateKeyTurnerState();
    void updateBatteryState();
    void updateConfig();
    void updateAuthData();

    void readConfig();
    void readAdvancedConfig();

    NukiLock::LockAction lockActionToEnum(const char* str); // char array at least 14 characters

    std::string _deviceName;
    NukiLock::NukiLock _nukiLock;
    BleScanner::Scanner* _bleScanner;
    Network* _network;
    Preferences* _preferences;
    int _intervalLockstate = 0; // seconds
    int _intervalBattery = 0; // seconds
    int _intervalConfig = 60 * 60; // seconds
    bool _publishAuthData = false;
    bool _clearAuthData = false;

    NukiLock::KeyTurnerState _lastKeyTurnerState;
    NukiLock::KeyTurnerState _keyTurnerState;

    uint32_t _lastAuthId = 0xffff;

    NukiLock::BatteryReport _batteryReport;
    NukiLock::BatteryReport _lastBatteryReport;

    NukiLock::Config _nukiConfig = {0};
    NukiLock::AdvancedConfig _nukiAdvancedConfig = {0};
    bool _nukiConfigValid = false;
    bool _nukiAdvancedConfigValid = false;

    bool _paired = false;
    bool _statusUpdated = false;
    unsigned long _nextLockStateUpdateTs = 0;
    unsigned long _nextBatteryReportTs = 0;
    unsigned long _nextConfigUpdateTs = 0;
    unsigned long _nextPairTs = 0;
    NukiLock::LockAction _nextLockAction = (NukiLock::LockAction)0xff;
};
