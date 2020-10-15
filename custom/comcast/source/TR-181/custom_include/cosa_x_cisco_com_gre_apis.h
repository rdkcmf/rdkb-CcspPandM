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
#ifndef _COSA_X_CISCO_COM_GRE_APIS_H
#define _COSA_X_CISCO_COM_GRE_APIS_H

#define MAX_CGRE_IFS             1

typedef enum
_COSA_DML_CGRE_STATUS 
{
    COSA_DML_CGRE_STATUS_UP      = 1,
    COSA_DML_CGRE_STATUS_DOWN    = 2,
    COSA_DML_CGRE_STATUS_LOWERLAYERDOWN    = 3,
    COSA_DML_CGRE_STATUS_ERROR   = 4,
}
COSA_DML_CGRE_STATUS;

typedef enum
_COSA_DML_CGRE_MODE
{
    COSA_DML_CGRE_MODE_IP = 1,
    COSA_DML_CGRE_MODE_ETHERNET = 2,
}
COSA_DML_CGRE_MODE;

typedef enum
_COSA_DML_CGRE_KEYMODE
{
    COSA_DML_CGRE_KEYMODE_DISABLED     = 1,
    COSA_DML_CGRE_KEYMODE_MANUAL        = 2,
    COSA_DML_CGRE_KEYMODE_AUTO        = 3,
}
COSA_DML_CGRE_KEYMODE;

typedef enum
_COSA_DML_CGRE_TOSMODE
{
    COSA_DML_CGRE_TOSMODE_STATIC = 1,
    COSA_DML_CGRE_TOSMODE_INHERITED = 2,
    COSA_DML_CGRE_TOSMODE_AUTOMAPPED = 3,
}
COSA_DML_CGRE_TOSMODE;

enum {
    COSA_DML_CGRE_CF_ALIAS      = 0x1 << 0,
    COSA_DML_CGRE_CF_ENABLE     = 0x1 << 1,
    COSA_DML_CGRE_CF_NAME       = 0x1 << 2,
    COSA_DML_CGRE_CF_LOWLAY     = 0x1 << 3,
    COSA_DML_CGRE_CF_MODE       = 0x1 << 4,
    COSA_DML_CGRE_CF_EP         = 0x1 << 5,
    COSA_DML_CGRE_CF_KEYMOD     = 0x1 << 6,
    COSA_DML_CGRE_CF_KEY        = 0x1 << 7,
    COSA_DML_CGRE_CF_CSUMEN     = 0x1 << 8,
    COSA_DML_CGRE_CF_SEQEN      = 0x1 << 9,
    COSA_DML_CGRE_CF_TOSMOD     = 0x1 << 10,
    COSA_DML_CGRE_CF_TOS        = 0x1 << 11,
};

typedef struct 
_COSA_DML_CGRE_IF
{
    ULONG                       InstanceNumber;
    char                        Alias[65];

    BOOL                        Enable;
    //COSA_DML_CGRE_STATUS        Status;
    char                        Name[65];
    //ULONG                       LastChange;
    char                        LowerLayers[1025];
    COSA_DML_CGRE_MODE          Mode;
    char                        RemoteEndpoint[257];
    COSA_DML_CGRE_KEYMODE       KeyMode;
    char                        Key[9];
    BOOL                        ChecksumEnabled;
    BOOL                        SequenceNumberEnabled;
    COSA_DML_CGRE_TOSMODE       TOSMode;
    ULONG                       TOS;

    time_t                      LastSet;
    int                         ChangeFlag;

    COSA_DML_IF_STATS           Stats;
} 
COSA_DML_CGRE_IF;

ANSC_STATUS 
CosaDml_CGreInit(void);

ANSC_STATUS 
CosaDml_CGreFinalize(void);

ULONG 
CosaDml_CGreIfGetNumberOfEntries(void);

ANSC_STATUS
CosaDml_CGreIfGetEntryByIndex(ULONG idx, COSA_DML_CGRE_IF *greIf);

ANSC_STATUS
CosaDml_CGreIfSetIns(ULONG idx, ULONG ins);

ANSC_STATUS
CosaDml_CGreIfGetCfg(ULONG ins, COSA_DML_CGRE_IF *greIf);

ANSC_STATUS
CosaDml_CGreIfSetCfg(ULONG ins, COSA_DML_CGRE_IF *greIf);

ANSC_STATUS
CosaDml_CGreIfGetStats(ULONG ins, COSA_DML_IF_STATS *stats);

ANSC_STATUS
CosaDml_CGreIfGetStatus(const COSA_DML_CGRE_IF *greIf, 
        COSA_DML_CGRE_STATUS *status);

ANSC_STATUS
CosaDml_CGreIfGetLastChange(const COSA_DML_CGRE_IF *greIf, 
        ULONG *lastChange);

ANSC_STATUS
CosaDml_CGreIfSetInsAlias(ULONG idx, ULONG ins, const char *alias);        

#endif
#endif
