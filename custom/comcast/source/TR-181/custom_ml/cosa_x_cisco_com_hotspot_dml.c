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

#ifdef CONFIG_CISCO_HOTSPOT
#include "cosa_x_cisco_com_hotspot_dml.h"
#include "cosa_x_cisco_com_hotspot_internal.h"

ULONG
HsSsid_GetEntryCount
    (
        ANSC_HANDLE hInsContext
    )
{
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
    COSA_DATAMODEL_HOTSPOT          *pMyObject = (COSA_DATAMODEL_HOTSPOT *)g_pCosaBEManager->hHotspot;

    if (nIndex >= MAX_HS_SSIDS || nIndex >= pMyObject->SsidCnt)
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

    CosaDml_HsSsidGetCfg(hsSsid->InstanceNumber, hsSsid);

    if (AnscEqualString(ParamName, "Alias", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(hsSsid->Alias))
        {
            *pUlSize = AnscSizeOfString(hsSsid->Alias) + 1;
            return 1;
        }

        AnscCopyString(pValue, hsSsid->Alias);
        return 0;
    }
    if (AnscEqualString(ParamName, "SSID", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(hsSsid->SSID))
        {
            *pUlSize = AnscSizeOfString(hsSsid->SSID) + 1;
            return 1;
        }

        AnscCopyString(pValue, hsSsid->SSID);
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

    if (nIndex >= MAX_HS_CLIS || nIndex >= hsSsid->DevCnt)
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

    CosaDml_HsSsidAssoDevGetCfg(assoDev->SsidIns, assoDev->InstanceNumber, assoDev);

    if (AnscEqualString(ParamName, "Alias", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->Alias))
        {
            *pUlSize = AnscSizeOfString(assoDev->Alias) + 1;
            return 1;
        }

        AnscCopyString(pValue, assoDev->Alias);
        return 0;
    }
    if (AnscEqualString(ParamName, "MACAddress", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->MACAddress))
        {
            *pUlSize = AnscSizeOfString(assoDev->MACAddress) + 1;
            return 1;
        }

        AnscCopyString(pValue, assoDev->MACAddress);
        return 0;
    }
    if (AnscEqualString(ParamName, "Hostname", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->Hostname))
        {
            *pUlSize = AnscSizeOfString(assoDev->Hostname) + 1;
            return 1;
        }

        AnscCopyString(pValue, assoDev->Hostname);
        return 0;
    }
    if (AnscEqualString(ParamName, "IPv4Address", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->IPv4Address))
        {
            *pUlSize = AnscSizeOfString(assoDev->IPv4Address) + 1;
            return 1;
        }

        AnscCopyString(pValue, assoDev->IPv4Address);
        return 0;
    }
    if (AnscEqualString(ParamName, "DHCPv4Status", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->DHCPv4Status))
        {
            *pUlSize = AnscSizeOfString(assoDev->DHCPv4Status) + 1;
            return 1;
        }

        AnscCopyString(pValue, assoDev->DHCPv4Status);
        return 0;
    }
    if (AnscEqualString(ParamName, "IPv6Address", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->IPv6Address))
        {
            *pUlSize = AnscSizeOfString(assoDev->IPv6Address) + 1;
            return 1;
        }

        AnscCopyString(pValue, assoDev->IPv6Address);
        return 0;
    }
    if (AnscEqualString(ParamName, "IPv6Prefix", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->IPv6Prefix))
        {
            *pUlSize = AnscSizeOfString(assoDev->IPv6Prefix) + 1;
            return 1;
        }

        AnscCopyString(pValue, assoDev->IPv6Prefix);
        return 0;
    }
    if (AnscEqualString(ParamName, "DHCPv6Status", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->DHCPv6Status))
        {
            *pUlSize = AnscSizeOfString(assoDev->DHCPv6Status) + 1;
            return 1;
        }

        AnscCopyString(pValue, assoDev->DHCPv6Status);
        return 0;
    }
    if (AnscEqualString(ParamName, "IPv6LinkLocalAddress", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(assoDev->IPv6LinkLocalAddress))
        {
            *pUlSize = AnscSizeOfString(assoDev->IPv6LinkLocalAddress) + 1;
            return 1;
        }

        AnscCopyString(pValue, assoDev->IPv6LinkLocalAddress);
        return 0;
    }

    return -1;
}

BOOL
HsAssoDev_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    )
{
    COSA_DML_HOTSPOT_ASSODEV        *assoDev = (COSA_DML_HOTSPOT_ASSODEV *)hInsContext;

    CosaDml_HsSsidAssoDevGetCfg(assoDev->SsidIns, assoDev->InstanceNumber, assoDev);

    if (AnscEqualString(ParamName, "RSSILevel", TRUE))
    {
        *pUlong = assoDev->RSSILevel;
        return TRUE;
    }

    return FALSE;
}
#endif
