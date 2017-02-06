#include "user_base.h"
#include "ansc_platform.h"
#include "ccsp_trace.h"
#include "xbb1.h"

#include <inttypes.h>
#include <string.h>

#define XBB1_CHECK_NULL(NAME) if (!NAME) return FALSE

#if 1
#define XBB1_DEBUG_ENTER(NAME) CcspTraceInfo(("ENTER %s(%s)\n", __FUNCTION__, NAME));
#else
#define XBB1_DEBUG_ENTER(NAME)
#endif

#define XBB1_COPY_PARAM(TYPE, NAME, FIELD) \
  if (XBB1_StringEquals(name, NAME)) \
  { \
    *val = (TYPE) FIELD; \
    return TRUE; \
  }

#define XBB1_COPY_STRING(NAME, FIELD) \
  if (XBB1_StringEquals(name, NAME)) \
  {  \
    return XBB1_StringCopy(val, FIELD, size); \
  }

static XbbStatus        xbbStatus;
static XbbConfiguration xbbConfig;
static XbbAlarmInfo*    xbbAlarms = NULL;
static int const        kStatusCacheUpdateSeconds = 10;
static int const        kConfigCacheUpdateSeconds = 10;
static int const        kAlarmCacheUpdateSeconds = 10;
static time_t           lastStatusUpdateTime = 0;
static time_t           lastConfigUpdateTime = 0;
static time_t           lastAlarmUpdateTime = 0;
static int              alarmsDirty = 1;
static uint16_t         numAlarms = 0;
static int              alarmIndex = 0;

static void xbbUpdateAlarms()
{
  XBB1_DEBUG_ENTER("xbbUpdateAlarms");
  if (alarmsDirty)
  {
    time_t now = time(0);
    if(now > (kAlarmCacheUpdateSeconds + lastAlarmUpdateTime))
    {
        if(xbbAlarms)
        {
        	free(xbbAlarms);
                xbbAlarms = NULL;
                numAlarms = 0;
                alarmIndex = 0;
        }

    	if (!xbbGetAlarms(&xbbAlarms, &numAlarms))
    	{
      		CcspTraceWarning(("error getting battery alarms list.\n"));
      		xbbAlarms = NULL;
    	}
        lastAlarmUpdateTime = now;	 
    }
  }
}

static void xbbUpdateStatus()
{
  if (lastStatusUpdateTime == 0)
  {
    memset(&xbbStatus, sizeof(XbbStatus), 0);
  }

  time_t now = time(0);
  if (now > (kStatusCacheUpdateSeconds + lastStatusUpdateTime))
  {
    CcspTraceInfo(("updating cached batery status\n"));
    if (!xbbGetStatus(&xbbStatus))
    {
      CcspTraceWarning(("error updating battery status. probably offline or needs to be re-paired\n"));
    }
    lastStatusUpdateTime = now;
  }
}

static void xbbUpdateConfig()
{
  if (lastConfigUpdateTime == 0)
  {
    memset(&xbbConfig, sizeof(xbbConfig), 0);
  }

  time_t now = time(0);
  if (now > (kConfigCacheUpdateSeconds + lastConfigUpdateTime))
  {
    CcspTraceInfo(("updating cached batter config\n"));
    if (!xbbGetConfig(&xbbConfig))
    {
      CcspTraceWarning(("error updating batter config.\n"));
    }
    lastConfigUpdateTime = now;
  }
}

static ULONG 
XBB1_StringCopy(char* dst, char const* src, ULONG* n)
{
  ULONG status;
  ULONG length;

  length = (ULONG) strlen(src);
  if (*n < length)
  {
    *n = (ULONG) length;
    status = 1;
  }
  else
  {
    memset(dst, *n, 0);
    strcpy(dst, src);
    status = 0;
  }

  return status;
}

BOOL
XBB1_StringEquals(char const* s, char const* t)
{
  return (strcmp(s, t) == 0) ? TRUE : FALSE;
}


BOOL
XBB1_GetParamBoolValue(ANSC_HANDLE h, char* name, BOOL* val)
{
  XBB1_DEBUG_ENTER(name);
  XBB1_CHECK_NULL(name);
  XBB1_CHECK_NULL(val);

  xbbUpdateStatus();

  XBB1_COPY_PARAM(BOOL, "chargingStatus", xbbStatus.isCharging);
  XBB1_COPY_PARAM(BOOL, "testingStatus", xbbStatus.isUnderTest);

  return FALSE;
}

BOOL
XBB1_SetParamBoolValue(ANSC_HANDLE h, char* name, BOOL  val)
{
  XBB1_DEBUG_ENTER(name);
  return FALSE;
}

BOOL
XBB1_GetParamIntValue(ANSC_HANDLE h, char* name, int* val)
{
  XBB1_DEBUG_ENTER(name);
  XBB1_CHECK_NULL(name);
  XBB1_CHECK_NULL(val);

  xbbUpdateStatus();
  xbbUpdateConfig();

  XBB1_COPY_PARAM(int, "currentTemperature", xbbStatus.currentTemperature);
  XBB1_COPY_PARAM(int, "maxTempExperienced", xbbStatus.maxTempExperienced);
  XBB1_COPY_PARAM(int, "minTempExperienced", xbbStatus.minTempExperienced);
  XBB1_COPY_PARAM(int, "hwVersion", xbbStatus.hardwareVersion);
  XBB1_COPY_PARAM(int, "lowTempThreshold", xbbConfig.lowTempThreshold);
  XBB1_COPY_PARAM(int, "highTempThreshold", xbbConfig.highTempThreshold);

  if (XBB1_StringEquals("discover", name))
  {
    *val = 0;
    return TRUE;
  }

  return FALSE;
}

BOOL
XBB1_SetParamIntValue(ANSC_HANDLE h, char* name, int val)
{
  XBB1_DEBUG_ENTER(name);
  XbbConfiguration config;

  if (XBB1_StringEquals("discover", name))
  {
    CcspTraceInfo(("enabling xbb discovery for: %ds\n", val));
    if (xbbDiscover((uint16_t) val))
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  if (XBB1_StringEquals("lowTempThreshold",name))
  {
    config.lowTempThreshold = val;
  }
  if (XBB1_StringEquals("highTempThreshold",name))
  {
    config.highTempThreshold = val;
  }

  xbbUpdateConfig();

  return FALSE;
}

BOOL
XBB1_GetParamUlongValue(ANSC_HANDLE h, char* name, ULONG* val)
{
  XBB1_DEBUG_ENTER(name);
  XBB1_CHECK_NULL(name);
  XBB1_CHECK_NULL(val);

  xbbUpdateStatus();
  xbbUpdateConfig();

  if (XBB1_StringEquals(name, "batteryCapacity"))
  {
    /* Where does this come from? */
    *val = 8;
    return TRUE;
  }

  if (XBB1_StringEquals("alarmNumberOfEntries", name))
  {
    xbbUpdateAlarms();
  }

  XBB1_COPY_PARAM(ULONG, "batteryHealth", xbbStatus.batteryHealth);
  XBB1_COPY_PARAM(ULONG, "batteryStatus", xbbStatus.batteryStatus);
  XBB1_COPY_PARAM(ULONG, "secondsOnBattery", xbbStatus.secondsOnBattery);
  XBB1_COPY_PARAM(ULONG, "healthOfChargingSystem", xbbStatus.chargingSystemHealth);

  XBB1_COPY_PARAM(ULONG, "poweredDeviceIdlePower1", xbbConfig.poweredDeviceIdlePower1);
  XBB1_COPY_PARAM(ULONG, "poweredDeviceIdlePower2", xbbConfig.poweredDeviceIdlePower2);

  XBB1_COPY_PARAM(ULONG, "estimatedChargeRemaining", xbbStatus.estimatedChargeRemainingPercent);
  XBB1_COPY_PARAM(ULONG, "estimatedMinutesRemaining", xbbStatus.estimatedMinutesRemaining);

  XBB1_COPY_PARAM(ULONG, "configLowBattTime", xbbConfig.configLowBatteryMinutes);
  XBB1_COPY_PARAM(ULONG, "testingState", xbbStatus.testingState);

  XBB1_COPY_PARAM(ULONG, "lowTempDwellTripPointSeconds", xbbConfig.lowTempDwellTripPointSeconds);
  XBB1_COPY_PARAM(ULONG, "highTempDwellTripPointSeconds", xbbConfig.highTempDwellTripPointSeconds);

  XBB1_COPY_PARAM(ULONG, "alarmNumberOfEntries", numAlarms);

  return FALSE;
}

BOOL
XBB1_SetParamUlongValue(ANSC_HANDLE h, char* name, ULONG val)
{
  int               dirty;
  XbbConfiguration  config;
  BOOL              paramSet;

  XBB1_DEBUG_ENTER(name);

  xbbUpdateConfig();

  dirty = 0;
  paramSet = FALSE;
  memcpy(&config, &xbbConfig, sizeof(XbbConfiguration));

  if (XBB1_StringEquals(name, "configLowBattTime"))
  {
    config.configLowBatteryMinutes = val;
    dirty = 1;
  }
  if (XBB1_StringEquals(name, "poweredDeviceIdlePower1"))
  {
    config.poweredDeviceIdlePower1 = val;
    dirty = 1;
  }
  if (XBB1_StringEquals(name, "poweredDeviceIdlePower2"))
  {
    config.poweredDeviceIdlePower2 = val;
    dirty = 1;
  }
  if (XBB1_StringEquals(name, "lowTempDwellTripPointSeconds"))
  {
    config.lowTempDwellTripPointSeconds = val;
    dirty = 1;
  }
  if (XBB1_StringEquals(name, "highTempDwellTripPointSeconds"))
  {
    config.highTempDwellTripPointSeconds = val;
    dirty = 1;
  }

  if (dirty)
  {
    CcspTraceInfo(("setting battery parameter %s == %lu\n", name, val));
    if (xbbSetConfig(&config))
    {
      lastConfigUpdateTime = 0;
      paramSet = TRUE;
    }
    else
    {
      CcspTraceWarning(("failed to set battery parameter %s to %lu\n", name, val));
    }
  }

  return paramSet;
}

/*
 * return: 0 (ok) 
 *         1 buffer not big enough, set size to required length
 *        -1 not supported
 */
ULONG
XBB1_GetParamStringValue(ANSC_HANDLE h, char* name, char* val, ULONG* size)
{
  XBB1_DEBUG_ENTER(name);
  XBB1_CHECK_NULL(name);
  XBB1_CHECK_NULL(val);
  XBB1_CHECK_NULL(size);

  xbbUpdateStatus();

  XBB1_COPY_STRING("manufacturerName", xbbStatus.manufacturer);
  XBB1_COPY_STRING("serialNumber", xbbStatus.serialNumber);
  XBB1_COPY_STRING("modelIdentifier", xbbStatus.model);

  if (XBB1_StringEquals(name, "softwareVersion"))
  {
    char buff[64];
    memset(buff, 0, sizeof(buff));
    snprintf(buff, sizeof(buff), "%" PRIx64, xbbStatus.firmwareVersion);

    return XBB1_StringCopy(val, buff, size);
  }

  return (ULONG) -1;
}

BOOL
XBB1_SetParamStringValue(ANSC_HANDLE h, char* name, char* val, ULONG* n)
{
  XBB1_DEBUG_ENTER(name);
  return FALSE;
}

ULONG
XBB1_Alarm_GetEntryCount(ANSC_HANDLE h)
{
  XBB1_DEBUG_ENTER("XBB1_Alarm_GetEntryCount");
  uint16_t n;
   
  xbbUpdateAlarms();

  n = numAlarms;
  if (n > 16)
    n = 16;
  CcspTraceInfo(("Number of Alarms: %d\n", n));
  return (ULONG) n;
}

ANSC_HANDLE
XBB1_Alarm_GetEntry(ANSC_HANDLE h, ULONG index, ULONG* inst)
{
  XBB1_DEBUG_ENTER("XBB1_Alarm_GetEntry");
  xbbUpdateAlarms();

  *inst = index + 1;

  if (index < numAlarms)
  {
    return (ANSC_HANDLE) &xbbAlarms[index];
  }
  return NULL;
}

BOOL
XBB1_Alarm_IsUpdated(ANSC_HANDLE h)
{
    XBB1_DEBUG_ENTER("XBB1_Alarm_IsUpdated");
    return TRUE;
}


ULONG
XBB1_Alarm_Synchronize(ANSC_HANDLE h)
{
    XBB1_DEBUG_ENTER("XBB1_Alarm_Synchronize");
    return 0;
}

BOOL
XBB1_Alarm_GetParamIntValue(ANSC_HANDLE h, char* name, int* val)
{
  XBB1_DEBUG_ENTER("XBB1_Alarm_GetParamIntValue");
  alarmIndex = ((XbbAlarmInfo *)h - xbbAlarms) + 1;
  XBB1_COPY_PARAM(int, "alarm", alarmIndex); 
  return FALSE; 
}

BOOL
XBB1_Alarm_GetParamUlongValue(ANSC_HANDLE h, char* name, int* val)
{
  XbbAlarmInfo* alarm = NULL;
  XBB1_DEBUG_ENTER("XBB1_Alarm_GetParamUlongValue");
  unsigned int alarmDesc[7] = {6, 4, 5, 0, 1, 2, 3};
  xbbUpdateAlarms();
  alarm = (XbbAlarmInfo *) h;
    
  if (alarm)
  {
    XBB1_COPY_PARAM(ULONG, "timeStamp", alarm->timestamp);
    XBB1_COPY_PARAM(ULONG, "alarmDescription", alarmDesc[alarm->type]);
  }
  return FALSE;
}
