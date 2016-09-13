//------------------------------ tabstop = 4 ----------------------------------
// 
// Copyright (C) 2015 iControl Networks, Inc.
//
// All rights reserved.
//
// This software is protected by copyright laws of the United States
// and of foreign countries. This material may also be protected by
// patent laws of the United States and of foreign countries.
//
// This software is furnished under a license agreement and/or a
// nondisclosure agreement and may only be used or copied in accordance
// with the terms of those agreements.
//
// The mere transfer of this software does not imply any licenses of trade
// secrets, proprietary technology, copyrights, patents, trademarks, or
// any other form of intellectual property whatsoever.
//
// iControl Networks retains all ownership rights.
//
//------------------------------ tabstop = 4 ----------------------------------
/*
 * Interface to XBB (Xfinity Battery Backup).
 *
 * Based on Comcast-SP-XBB-ZigBee-SW-D03-160613 draft June 13, 2016
 *
 * Created by Thomas Lea on 7/18/16.
 */

#ifndef FLEXCORE_XBB_H
#define FLEXCORE_XBB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define MAX_MANUFACTURER_LEN 64
#define MAX_MODEL_LEN 64
#define MAX_SERIAL_NUMBER_LEN 32

typedef enum
{
    batteryStatusUnknown,
    batteryStatusNormal,
    batteryStatusLow,
    batteryStatusDepleted,
} BatteryStatus;

typedef enum
{
    batteryHealthGood,    /* 80-100% Capacity + Impedance low */
    batteryHealthFair,    /* 50% to 79% Capacity + Impedance moderately low */
    batteryHealthPoor,    /* <= 50% Capacity or Impedance moderately high */
    batteryHealthFailure, /* = Battery is Dead, Impedance High, or Turned Off for Safety Issue. */
} BatteryHealth;

typedef enum
{
    batteryTestingStateNotDischarging,
    batteryTestingStateDischarging,
    batteryTestingStateCharging,
} BatteryTestingState;

typedef enum
{
    batteryChargingSystemHealthGood,
    batteryChargingSystemHealthVoltageHigh,
    batteryChargingSystemHealthCurrentHigh,
    batteryChargingSystemHealthCurrentLow,
    batteryChargingSystemHealthDischargingOrTestCurrentFailure, //TODO: this entry may need renaming if its use becomes clear
} BatteryChargingSystemHealth;

typedef struct
{
    char                        manufacturer[MAX_MANUFACTURER_LEN+1];
    char                        model[MAX_MODEL_LEN+1];
    uint64_t                    firmwareVersion;
    uint8_t                     hardwareVersion;
    char                        serialNumber[MAX_SERIAL_NUMBER_LEN + 1];
    BatteryStatus               batteryStatus;
    BatteryHealth               batteryHealth;
    bool                        isCharging;
    bool                        isUnderTest;
    BatteryTestingState         testingState;
    BatteryChargingSystemHealth chargingSystemHealth;
    uint32_t                    secondsOnBattery;
    uint32_t                    estimatedMinutesRemaining;
    uint8_t                     estimatedChargeRemainingPercent;
    int16_t                     currentTemperature; /* degrees Celsius */
    int16_t                     minTempExperienced; /* degrees Celsius */
    int16_t                     maxTempExperienced; /* degrees Celsius */
    bool                        hasAlarms;
} XbbStatus;

typedef struct
{
    uint32_t                poweredDeviceIdlePower1;
    uint32_t                poweredDeviceIdlePower2; /* ignored on XBB1 */
    uint32_t                configLowBatteryMinutes;
    int16_t                 lowTempThreshold; /* degrees Celcius */
    int16_t                 highTempThreshold; /* degrees Celcius */
    uint32_t                lowTempDwellTripPointSeconds;
    uint32_t                highTempDwellTripPointSeconds;
} XbbConfiguration;

typedef enum
{
    alarmTypeUnknown,
    alarmTypeLowTemp,
    alarmTypeHighTemp,
    alarmTypeBatteryBad,
    alarmTypeBatteryLow,
    alarmTypeChargingSystemBad,
    alarmTypeBatteryMissing,
} AlarmType;

typedef struct
{
    AlarmType type;
    uint32_t timestamp; //POSIX timestamp
} XbbAlarmInfo;

/**
 * Retrieve the current status information from the XBB.
 *
 * @param status a pointer to an allocated XbbStatus structure that will be populated upon success
 *
 * @return true on success
 */
bool xbbGetStatus(XbbStatus *status);

/**
 * Retrieve the current XBB configuration.
 *
 * @param config a pointer to an allocated XbbConfiguration structure that will be populated upon success
 *
 * @return true on success
 */
bool xbbGetConfig(XbbConfiguration *config);

/**
 * Set the configuration options on the XBB.
 *
 * @param config a pointer to a populated XbbConfiguration structure
 *
 * @return true on success
 */
bool xbbSetConfig(XbbConfiguration *config);

/**
 * Attempt to locate the XBB.  This will discard any currently associated XBB.  XBB needs to also be attempting to pair.
 *
 * @param timeoutSeconds the number of seconds for the locate operation before giving up
 *
 * @return true if the XBB is successfully located
 */
bool xbbDiscover(uint16_t timeoutSeconds);

/**
 * Retrieve the current list of alarms (if any).
 *
 * @param alarmInfos will receive the list of XbbAlarmInfos currently active in the XBB.  Will be set to NULL if no alarms.
 * @param numAlarmInfos will receive the number of alarms returned in alarmInfos.  Will be set to 0 if no alarms.
 *
 * @return true on success
 */
bool xbbGetAlarms(XbbAlarmInfo **alarmInfos, uint16_t *numAlarmInfos);

#ifdef __cplusplus
}
#endif

#endif //FLEXCORE_XBB_H
