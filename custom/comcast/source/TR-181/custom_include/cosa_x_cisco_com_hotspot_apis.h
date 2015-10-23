/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

#ifdef CONFIG_CISCO_HOTSPOT
#ifndef _COSA_X_CISCO_COM_HOTSPOT_APIS_H
#define _COSA_X_CISCO_COM_HOTSPOT_APIS_H

#define MAX_HS_SSIDS    16
#define MAX_HS_CLIS     64

struct _COSA_DML_HOTSPOT_ASSODEV;

typedef struct 
_COSA_DML_HOTSPOT_ASSODEV
{
    ULONG                       InstanceNumber;
    char                        Alias[65];

    ULONG                       SsidIns;

    char                        MACAddress[18];
    char                        Hostname[257];
    int                         RSSILevel;
    char                        IPv4Address[17];
    char                        DHCPv4Status[64];
    char                        IPv6Address[64];
    char                        IPv6Prefix[64];
    char                        DHCPv6Status[64];
    char                        IPv6LinkLocalAddress[64];
} COSA_DML_HOTSPOT_ASSODEV;

typedef struct {
    ULONG                       InstanceNumber;
    char                        Alias[65];

    char                        SSID[256];

    COSA_DML_HOTSPOT_ASSODEV    AssoDevs[MAX_HS_CLIS];
    int                         DevCnt;
} COSA_DML_HOTSPOT_SSID;

ANSC_STATUS
CosaDml_HotspotInit(void);

ANSC_STATUS
CosaDml_HotspotFinalize(void);

ULONG
CosaDml_HsSsidGetNumberOfEntries(void);

ANSC_STATUS
CosaDml_HsSsidGetEntryByIndex(ULONG idx, COSA_DML_HOTSPOT_SSID *entry);

ANSC_STATUS
CosaDml_HsSsidGetCfg(ULONG ins, COSA_DML_HOTSPOT_SSID *entry);

ULONG
CosaDml_HsSsidAssoDevGetNumberOfEntries(ULONG ssidIns);

ANSC_STATUS
CosaDml_HsSsidAssoDevGetEntryByIndex(ULONG ssidIns, ULONG idx, COSA_DML_HOTSPOT_ASSODEV *entry);

ANSC_STATUS
CosaDml_HsSsidAssoDevGetCfg(ULONG ssidIns, ULONG ins, COSA_DML_HOTSPOT_ASSODEV *entry);

#endif  /* _COSA_X_CISCO_COM_HOTSPOT_APIS_H */
#endif
