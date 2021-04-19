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

/*
 *  Where is the file header section? Taking shortcut?...
 */

#include "dml_tr181_custom_cfg.h"
#include "safec_lib_common.h"

#ifdef CONFIG_CISCO_HOTSPOT
#include "cosa_x_cisco_com_hotspot_dml.h"
#include "cosa_x_cisco_com_hotspot_internal.h"

ULONG
HsSsid_GetEntryCount
    (
        ANSC_HANDLE hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return CosaDml_HsSsidGetNumberOfEntries();
}

ANSC_HANDLE
HsSsid_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_HOTSPOT          *pMyObject = (COSA_DATAMODEL_HOTSPOT *)g_pCosaBEManager->hHotspot;

    if (nIndex >= MAX_HS_SSIDS || (int)nIndex >= pMyObject->SsidCnt)
        return NULL;

    *pInsNumber = pMyObject->HsSsids[nIndex].InstanceNumber;
    return (ANSC_HANDLE)&pMyObject->HsSsids[nIndex];
}

ULONG
HsSsid_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    COSA_DML_HOTSPOT_SSID           *hsSsid = (COSA_DML_HOTSPOT_SSID *)hInsContext;
    errno_t                         rc      = -1;

    CosaDml_HsSsidGetCfg(hsSsid->InstanceNumber, hsSsid);

    if (strcmp(ParamName, "Alias") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(hsSsid->Alias))
        {
            *pUlSize = AnscSizeOfString(hsSsid->Alias) + 1;
            return 1;
        }

        rc = strcpy_s(pValue, *pUlSize, hsSsid->Alias);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "SSID") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(hsSsid->SSID))
        {
            *pUlSize = AnscSizeOfString(hsSsid->SSID) + 1;
            return 1;
        }

        rc = strcpy_s(pValue, *pUlSize, hsSsid->SSID);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    return -1;
}

ULONG
HsAssoDev_GetEntryCount
    (
        ANSC_HANDLE hInsContext
    )
{
    COSA_DML_HOTSPOT_SSID           *hsSsid = (COSA_DML_HOTSPOT_SSID *)hInsContext;

    return hsSsid->DevCnt;
}

ANSC_HANDLE
HsAssoDev_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    COSA_DML_HOTSPOT_SSID           *hsSsid = (COSA_DML_HOTSPOT_SSID *)hInsContext;

    if (nIndex >= MAX_HS_CLIS || (int)nIndex >= hsSsid->DevCnt)
        return NULL;

    *pInsNumber = hsSsid->AssoDevs[nIndex].InstanceNumber;
    return (ANSC_HANDLE)&hsSsid->AssoDevs[nIndex];
}


BOOL
HsAssoDev_IsUpdated
    (
     ANSC_HANDLE hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return TRUE;
}

ULONG
HsAssoDev_Synchronize
    (
     ANSC_HANDLE hInsContext
    )
{
    COSA_DML_HOTSPOT_SSID           *hsSsid = (COSA_DML_HOTSPOT_SSID *)hInsContext;
    int i;

    hsSsid->DevCnt = CosaDml_HsSsidAssoDevGetNumberOfEntries(hsSsid->InstanceNumber);
    if (hsSsid->DevCnt > MAX_HS_CLIS)
        hsSsid->DevCnt = MAX_HS_CLIS;

    for (i = 0; i < hsSsid->DevCnt; i++) {
        CosaDml_HsSsidAssoDevGetEntryByIndex(hsSsid->InstanceNumber, i, &hsSsid->AssoDevs[i]);
        hsSsid->AssoDevs[i].InstanceNumber = i + 1;
    }

    return 0;
}

ULONG
HsAssoDev_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    COSA_DML_HOTSPOT_ASSODEV        *assoDev = (COSA_DML_HOTSPOT_ASSODEV *)hInsContext;
    errno_t                         rc       = -1;

   // CosaDml_HsSsidAssoDevGetCfg(assoDev->SsidIns, assoDev->InstanceNumber, assoDev);

    if (strcmp(ParamName, "Alias") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->Alias))
        {
            *pUlSize = AnscSizeOfString(assoDev->Alias) + 1;
            return 1;
        }

        rc = strcpy_s(pValue, *pUlSize, assoDev->Alias);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "MACAddress") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->MACAddress))
        {
            *pUlSize = AnscSizeOfString(assoDev->MACAddress) + 1;
            return 1;
        }

        rc = strcpy_s(pValue, *pUlSize, assoDev->MACAddress);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "Hostname") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->Hostname))
        {
            *pUlSize = AnscSizeOfString(assoDev->Hostname) + 1;
            return 1;
        }

        rc = strcpy_s(pValue, *pUlSize, assoDev->Hostname);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "IPv4Address") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->IPv4Address))
        {
            *pUlSize = AnscSizeOfString(assoDev->IPv4Address) + 1;
            return 1;
        }

        rc = strcpy_s(pValue, *pUlSize, assoDev->IPv4Address);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "DHCPv4Status") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->DHCPv4Status))
        {
            *pUlSize = AnscSizeOfString(assoDev->DHCPv4Status) + 1;
            return 1;
        }

        rc = strcpy_s(pValue, *pUlSize, assoDev->DHCPv4Status);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "IPv6Address") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->IPv6Address))
        {
            *pUlSize = AnscSizeOfString(assoDev->IPv6Address) + 1;
            return 1;
        }

        rc = strcpy_s(pValue, *pUlSize, assoDev->IPv6Address);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "IPv6Prefix") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->IPv6Prefix))
        {
            *pUlSize = AnscSizeOfString(assoDev->IPv6Prefix) + 1;
            return 1;
        }

        rc = strcpy_s(pValue, *pUlSize, assoDev->IPv6Prefix);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "DHCPv6Status") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->DHCPv6Status))
        {
            *pUlSize = AnscSizeOfString(assoDev->DHCPv6Status) + 1;
            return 1;
        }

        rc = strcpy_s(pValue, *pUlSize, assoDev->DHCPv6Status);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "IPv6LinkLocalAddress") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->IPv6LinkLocalAddress))
        {
            *pUlSize = AnscSizeOfString(assoDev->IPv6LinkLocalAddress) + 1;
            return 1;
        }

        rc = strcpy_s(pValue, *pUlSize, assoDev->IPv6LinkLocalAddress);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    return -1;
}

BOOL
HsAssoDev_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    COSA_DML_HOTSPOT_ASSODEV        *assoDev = (COSA_DML_HOTSPOT_ASSODEV *)hInsContext;

    CosaDml_HsSsidAssoDevGetCfg(assoDev->SsidIns, assoDev->InstanceNumber, assoDev);

    if (strcmp(ParamName, "RSSILevel") == 0)
    {
        *pInt = assoDev->RSSILevel;
        return TRUE;
    }

    return FALSE;
}
#endif
