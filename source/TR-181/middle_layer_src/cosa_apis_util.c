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


/**********************************************************************

    module:	cosa_apis_util.h

        This is base file for all parameters H files.

    ---------------------------------------------------------------

    copyright:

        CISCO, Inc.,
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This file contains all utility functions for COSA DML API development.

    ---------------------------------------------------------------

    environment:

        COSA independent

    ---------------------------------------------------------------

    author:

        Roger Hu
        leichen2@cisco.com

    ---------------------------------------------------------------

    revision:

        01/30/2011    initial revision.
        06/15/2012    add IPv4 address utils
        06/15/2012    add vsystem(), chomp()

**********************************************************************/



#include "cosa_apis.h"
#include "plugin_main_apis.h"

#ifdef _ANSC_LINUX
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/types.h>
#endif
#include "ansc_platform.h"

//$HL 4/30/2013
#include "ccsp_psm_helper.h"

ANSC_STATUS
CosaUtilStringToHex
    (
        char          *str,
        unsigned char *hex_str,
        int           hex_sz 
    )
{
    INT   i, index, val = 0;
    CHAR  byte[3]       = {'\0'};

    while(str[i] != '\0')
    {
        byte[0] = str[i];
        byte[1] = str[i+1];
        byte[2] = '\0';
        if(_ansc_sscanf(byte, "%x", &val) != 1)
            break;
	hex_str[index] = val;
        i += 2;
        index++;
        if (str[i] == ':' || str[i] == '-'  || str[i] == '_')
            i++;
    }
    if(index != hex_sz)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaUtilGetIfAddr
    (
        char*       netdev
    )
{
    ANSC_IPV4_ADDRESS       ip4_addr = {0};

#ifdef _ANSC_LINUX

    struct ifreq            ifr;
    int                     fd = 0;

    strcpy(ifr.ifr_name, netdev);

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
    {
        if (!ioctl(fd, SIOCGIFADDR, &ifr))
           memcpy(&ip4_addr.Value, ifr.ifr_ifru.ifru_addr.sa_data + 2,4);
        else {
#ifdef _XF3_PRODUCT_REQ_
           /* CcspTraceWarning(("CosaUtilGetIfAddr IOCTL failure\n"));  Disable since it spams console when fiber is disconnected */
#else
           CcspTraceWarning(("CosaUtilGetIfAddr IOCTL failure\n"));
#endif
           CcspTraceWarning(("Cannot get ipv4 address of netdev:%s\n",netdev));
        }
        close(fd);
    }
    else
        perror("CosaUtilGetIfAddr failed to open socket.");

#else

    AnscGetLocalHostAddress(ip4_addr.Dot);

#endif

    return ip4_addr.Value;

}

ANSC_STATUS
CosaSListPushEntryByInsNum
    (
        PSLIST_HEADER               pListHead,
        PCOSA_CONTEXT_LINK_OBJECT   pCosaContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContextEntry = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    ULONG                           ulIndex           = 0;

    if ( pListHead->Depth == 0 )
    {
        AnscSListPushEntryAtBack(pListHead, &pCosaContext->Linkage);
    }
    else
    {
        pSLinkEntry = AnscSListGetFirstEntry(pListHead);

        for ( ulIndex = 0; ulIndex < pListHead->Depth; ulIndex++ )
        {
            pCosaContextEntry = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

            if ( pCosaContext->InstanceNumber < pCosaContextEntry->InstanceNumber )
            {
                AnscSListPushEntryByIndex(pListHead, &pCosaContext->Linkage, ulIndex);

                return ANSC_STATUS_SUCCESS;
            }
        }

        AnscSListPushEntryAtBack(pListHead, &pCosaContext->Linkage);
    }

    return ANSC_STATUS_SUCCESS;
}

PCOSA_CONTEXT_LINK_OBJECT
CosaSListGetEntryByInsNum
    (
        PSLIST_HEADER               pListHead,
        ULONG                       InstanceNumber
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContextEntry = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    ULONG                           ulIndex           = 0;

    if ( pListHead->Depth == 0 )
    {
        return NULL;
    }
    else
    {
        pSLinkEntry = AnscSListGetFirstEntry(pListHead);

        for ( ulIndex = 0; ulIndex < pListHead->Depth; ulIndex++ )
        {
            pCosaContextEntry = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

            if ( pCosaContextEntry->InstanceNumber == InstanceNumber )
            {
                return pCosaContextEntry;
            }
        }
    }

    return NULL;
}

PUCHAR
CosaUtilGetLowerLayers
    (
        PUCHAR                      pTableName,
        PUCHAR                      pKeyword
    )
{

    ULONG                           ulNumOfEntries              = 0;
    ULONG                           ulChildNumOfEntries         = 0;
    ULONG                           i                           = 0;
    ULONG                           j                           = 0;
    ULONG                           ulEntryNameLen              = 256;
    UCHAR                           ucEntryParamName[256]       = {0};
    UCHAR                           ucEntryNameValue[256]       = {0};
    UCHAR                           ucEntryFullPath[256]        = {0};
    UCHAR                           ucLowerEntryPath[256]       = {0};
    UCHAR                           ucLowerEntryName[256]       = {0};
    ULONG                           ulEntryInstanceNum          = 0;
    ULONG                           ulEntryPortNum              = 0;
    PUCHAR                          pMatchedLowerLayer          = NULL;
    PANSC_TOKEN_CHAIN               pTableListTokenChain        = (PANSC_TOKEN_CHAIN)NULL;
    PANSC_STRING_TOKEN              pTableStringToken           = (PANSC_STRING_TOKEN)NULL;

    if ( !pTableName || AnscSizeOfString(pTableName) == 0 ||
         !pKeyword   || AnscSizeOfString(pKeyword) == 0
       )
    {
        return NULL;
    }

    pTableListTokenChain = AnscTcAllocate(pTableName, ",");

    if ( !pTableListTokenChain )
    {
        return NULL;
    }

    while ((pTableStringToken = AnscTcUnlinkToken(pTableListTokenChain)))
    {
        if ( pTableStringToken->Name )
        {
            if ( AnscEqualString(pTableStringToken->Name, "Device.Ethernet.Interface.", TRUE ) )
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.Ethernet.InterfaceNumberOfEntries");

                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.Ethernet.Interface.", i);

                    if ( ulEntryInstanceNum )
                    {
                        _ansc_sprintf(ucEntryFullPath, "%s%d", "Device.Ethernet.Interface.", ulEntryInstanceNum);

                        _ansc_sprintf(ucEntryParamName, "%s%s", ucEntryFullPath, ".Name");
               
                        ulEntryNameLen = sizeof(ucEntryNameValue);
                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             AnscEqualString(ucEntryNameValue, pKeyword, TRUE ) )
                        {
                            pMatchedLowerLayer =  AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.IP.Interface.", TRUE ) )
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.IP.InterfaceNumberOfEntries");
                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.IP.Interface.", i);

                    if ( ulEntryInstanceNum )
                    {
                        _ansc_sprintf(ucEntryFullPath, "%s%d", "Device.IP.Interface.", ulEntryInstanceNum);

                        _ansc_sprintf(ucEntryParamName, "%s%s", ucEntryFullPath, ".Name");

                        ulEntryNameLen = sizeof(ucEntryNameValue);
                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             AnscEqualString(ucEntryNameValue, pKeyword, TRUE ) )
                        {
                            pMatchedLowerLayer =  AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.USB.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.HPNA.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.DSL.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.WiFi.Radio.", TRUE ) )
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.WiFi.RadioNumberOfEntries");

                for (i = 0; i < ulNumOfEntries; i++)
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.WiFi.Radio.", i);
                    
                    if (ulEntryInstanceNum)
                    {
                        _ansc_sprintf(ucEntryFullPath, "%s%d.", "Device.WiFi.Radio.", ulEntryInstanceNum);
                        
                        _ansc_sprintf(ucEntryParamName, "%s%s", ucEntryFullPath, "Name");
                        
                        ulEntryNameLen = sizeof(ucEntryNameValue);
                        if (( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                            AnscEqualString(ucEntryNameValue, pKeyword, TRUE) )
                        {
                            pMatchedLowerLayer = AnscCloneString(ucEntryFullPath);
                            
                            break;
                        }
                    }
                }
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.HomePlug.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.MoCA.Interface.", TRUE ) )
            {

                parameterValStruct_t varStruct;
                ulEntryNameLen   = sizeof(ucEntryNameValue);
                  ulNumOfEntries = 0;
                AnscCopyString(ucEntryParamName,"Device.MoCA.InterfaceNumberOfEntries");
                varStruct.parameterName = ucEntryParamName;
                varStruct.parameterValue = ucEntryNameValue;

                if (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen))
                {
                    AnscTraceFlow(("<HL>%s not found %s\n",__FUNCTION__,varStruct.parameterName ));
                    break;
                }

                //AnscTraceFlow(("<HL>%s ucEntryNameValue=%s\n", __FUNCTION__,ucEntryNameValue));
                _ansc_sscanf(ucEntryNameValue,"%d",&ulNumOfEntries);
                //AnscTraceFlow(("<HL>%s Wifi # of entries=%d\n", __FUNCTION__,ulNumOfEntries));

                i = 0;
                ulEntryInstanceNum =1;
                while (i < ulNumOfEntries)
                {
                    _ansc_memset(ucEntryParamName, 0, sizeof(ucEntryParamName));
                    _ansc_memset(ucEntryNameValue, 0, sizeof(ucEntryNameValue));
                    _ansc_sprintf(ucEntryParamName,"Device.MoCA.Interface.%d.Name",ulEntryInstanceNum);                    
                        
                    if (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen))
                    {
                        AnscTraceFlow(("<HL>%s WiFi instance(%d) not found\n", __FUNCTION__,
                            ulEntryInstanceNum));
                        ulEntryInstanceNum++;
                        continue;
                    }  
                    //AnscTraceFlow(("<HL>%s WiFi instance(%d) has name =%s inputName=%s\n",__FUNCTION__,ulEntryInstanceNum,ucEntryNameValue,pKeyword));
                    if ( AnscEqualString(ucEntryNameValue, pKeyword, TRUE ) )
                    {
                        _ansc_sprintf(ucEntryFullPath,"Device.MoCA.Interface.%d",ulEntryInstanceNum);
                        pMatchedLowerLayer =  AnscCloneString(ucEntryFullPath);

                        break;
                    }
                    ulEntryInstanceNum++;
                    i++;
                }

            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.X_CISCO_COM_GRE.Interface.", TRUE ) )
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.X_CISCO_COM_GRE.InterfaceNumberOfEntries");

                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.X_CISCO_COM_GRE.Interface.", i);

                    if ( ulEntryInstanceNum )
                    {
                        _ansc_sprintf(ucEntryFullPath, "%s%d", "Device.X_CISCO_COM_GRE.Interface.", ulEntryInstanceNum);

                        _ansc_sprintf(ucEntryParamName, "%s%s", ucEntryFullPath, ".Name");
               
                        ulEntryNameLen = sizeof(ucEntryNameValue);
                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             AnscEqualString(ucEntryNameValue, pKeyword, TRUE ) )
                        {
                            pMatchedLowerLayer =  AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.UPA.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.ATM.Link.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.PTM.Link.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.Ethernet.Link.", TRUE ) )
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.Ethernet.LinkNumberOfEntries");

                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.Ethernet.Link.", i);

                    if ( ulEntryInstanceNum )
                    {
                        _ansc_sprintf(ucEntryFullPath, "%s%d", "Device.Ethernet.Link.", ulEntryInstanceNum);

                        _ansc_sprintf(ucEntryParamName, "%s%s", ucEntryFullPath, ".Name");
               
                        ulEntryNameLen = sizeof(ucEntryNameValue);
                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             AnscEqualString(ucEntryNameValue, pKeyword, TRUE ) )
                        {
                            pMatchedLowerLayer =  AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.Ethernet.VLANTermination.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.WiFi.SSID.", TRUE ) )
            {
                parameterValStruct_t varStruct;
                ulEntryNameLen   = sizeof(ucEntryNameValue);
                ulNumOfEntries = 0;
                AnscCopyString(ucEntryParamName,"Device.WiFi.SSIDNumberOfEntries");
                varStruct.parameterName = ucEntryParamName;
                varStruct.parameterValue = ucEntryNameValue;
                if (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen))
                {
                    AnscTraceFlow(("<HL>%s not found %s\n",__FUNCTION__,varStruct.parameterName ));
                    break;
                }
                //AnscTraceFlow(("<HL>%s ucEntryNameValue=%s\n", __FUNCTION__,ucEntryNameValue));
                _ansc_sscanf(ucEntryNameValue,"%d",&ulNumOfEntries);
                //AnscTraceFlow(("<HL>%s Wifi # of entries=%d\n", __FUNCTION__,ulNumOfEntries));
                i = 0;
                ulEntryInstanceNum =1;
                while (i < ulNumOfEntries)
                {
                    _ansc_memset(ucEntryParamName, 0, sizeof(ucEntryParamName));
                    _ansc_memset(ucEntryNameValue, 0, sizeof(ucEntryNameValue));
                    _ansc_sprintf(ucEntryParamName,"Device.WiFi.SSID.%d.Name",ulEntryInstanceNum);                    
                        
                    if (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen))
                    {
                        AnscTraceFlow(("<HL>%s WiFi instance(%d) not found\n", __FUNCTION__,
                            ulEntryInstanceNum));
                        ulEntryInstanceNum++;
                        continue;
                    }  
                    //AnscTraceFlow(("<HL>%s WiFi instance(%d) has name =%s inputName=%s\n",__FUNCTION__,ulEntryInstanceNum,ucEntryNameValue,pKeyword));
                    if ( AnscEqualString(ucEntryNameValue, pKeyword, TRUE ) )
                    {
                        _ansc_sprintf(ucEntryFullPath,"Device.WiFi.SSID.%d",ulEntryInstanceNum);
                        pMatchedLowerLayer =  AnscCloneString(ucEntryFullPath);

                        break;
                    }
                    ulEntryInstanceNum++;
                    i++;
                }
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.Bridging.Bridge.", TRUE ) )
            {
                ulNumOfEntries =  CosaGetParamValueUlong("Device.Bridging.BridgeNumberOfEntries");
                CcspTraceInfo(("----------CosaUtilGetLowerLayers, bridgenum:%d\n", ulNumOfEntries));
                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.Bridging.Bridge.", i);
                    CcspTraceInfo(("----------CosaUtilGetLowerLayers, instance num:%d\n", ulEntryInstanceNum));

                    if ( ulEntryInstanceNum )
                    {
                        _ansc_sprintf(ucEntryFullPath, "%s%d", "Device.Bridging.Bridge.", ulEntryInstanceNum);
                        _ansc_sprintf(ucLowerEntryPath, "%s%s", ucEntryFullPath, ".PortNumberOfEntries"); 
                        
                        ulEntryPortNum = CosaGetParamValueUlong(ucLowerEntryPath);  
                        CcspTraceInfo(("----------CosaUtilGetLowerLayers, Param:%s,port num:%d\n",ucLowerEntryPath, ulEntryPortNum));

                        for ( j = 1; j<= ulEntryPortNum; j++) {
                            _ansc_sprintf(ucLowerEntryName, "%s%s%d", ucEntryFullPath, ".Port.", j);
                            _ansc_sprintf(ucEntryParamName, "%s%s%d%s", ucEntryFullPath, ".Port.", j, ".Name");
                            CcspTraceInfo(("----------CosaUtilGetLowerLayers, Param:%s,Param2:%s\n", ucLowerEntryName, ucEntryParamName));
                        
                            ulEntryNameLen = sizeof(ucEntryNameValue);
                            if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                                 AnscEqualString(ucEntryNameValue, pKeyword , TRUE ) )
                            {
                                pMatchedLowerLayer =  AnscCloneString(ucLowerEntryName);
                                CcspTraceInfo(("----------CosaUtilGetLowerLayers, J:%d, LowerLayer:%s\n", j, pMatchedLowerLayer));
                                break;
                            }
                        }
                    }
                }
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.PPP.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.DSL.Channel.", TRUE ) )
            {
            }
            
            if ( pMatchedLowerLayer )
            {
		AnscFreeMemory(pTableStringToken);
                break;
            }
        }

        AnscFreeMemory(pTableStringToken);
    }

    if ( pTableListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pTableListTokenChain);
    }

    /* Too many trace. Disable this  -- Yan */
    /*
    CcspTraceWarning
        ((
            "CosaUtilGetLowerLayers: %s matched LowerLayer(%s) with keyword %s in the table %s\n",
            pMatchedLowerLayer ? "Found a":"Not find any",
            pMatchedLowerLayer ? pMatchedLowerLayer : "",
            pKeyword,
            pTableName
        ));
    */

    return pMatchedLowerLayer;
}

/*
 *
 */
/*mapping for LinkType conversion*/

/* please don't alter LinkType, it can be used as index starting with 1*/
LINKTYPE_MAP_T g_linktype_map[COSA_DML_LINK_TYPE_TOTAL] = {
    {   "Device.Ethernet.Interface.", 
        "Ethernet", 
        COSA_DML_LINK_TYPE_Eth 
    },
    {   "Device.Ethernet.Link.", 
        "EthLink",
        COSA_DML_LINK_TYPE_EthLink 
    },
    {   "Device.Ethernet.VLANTermination.", 
        "Ethernet.VLANTermination", 
        COSA_DML_LINK_TYPE_EthVlan
    },
    {   "Device.USB.Interface.", 
        "USB", 
        COSA_DML_LINK_TYPE_Usb
    },
    {   "Device.HPNA.Interface.", 
        "HPNA", 
        COSA_DML_LINK_TYPE_Hpna
    },
    {   "Device.MoCA.Interface.", 
        "MoCA", 
        COSA_DML_LINK_TYPE_Moca
    },
    {   "Device.HomePlug.Interface.", 
        "HomePlug", 
        COSA_DML_LINK_TYPE_HomePlug
    },
    {   "Device.UPA.Interface.", 
        "UPA", 
        COSA_DML_LINK_TYPE_Upa
    },
    {   "Device.WiFi.SSID.", 
        "WiFi", 
        COSA_DML_LINK_TYPE_WiFiSsid
    },
    {   "Device.Bridging.Bridge.", 
        "Bridge", 
        COSA_DML_LINK_TYPE_Bridge
    },
    {   "Device.PPP.Interface.", 
        "PPP", 
        COSA_DML_LINK_TYPE_PPP
    },
    {   "DOCSIS", 
        "DOCSIS", 
        COSA_DML_LINK_TYPE_DOCSIS
    }
};


COSA_DML_LINK_TYPE CosaUtilGetLinkTypeFromStr(char* pLinkTypeStr)
{
    int index=0;
    if(NULL == pLinkTypeStr)
        return COSA_DML_LINK_TYPE_LAST;

    for(index=0; index<COSA_DML_LINK_TYPE_TOTAL; index++)
    {
        if(NULL != g_linktype_map[index].LinkTypeStr && strstr(pLinkTypeStr, g_linktype_map[index].LinkTypeStr) != NULL)
        {
            return g_linktype_map[index].LinkType;
        }
    }

    return COSA_DML_LINK_TYPE_LAST;
}

char* CosaUtilGetStrFromLinkTypePath(char* pLinkTypePath){
    int index=0;

    if(NULL == pLinkTypePath)
        return NULL;

    AnscTraceFlow(("%s: %s\n", __FUNCTION__, pLinkTypePath));
    for(index=0; index<COSA_DML_LINK_TYPE_TOTAL; index++)
    {
        //if(!strncmp(g_linktype_map[index].LinkTypePath, pLinkTypePath, 
        //        sizeof(g_linktype_map[index].LinkTypePath)))
        if(NULL != g_linktype_map[index].LinkTypePath && strstr(pLinkTypePath, g_linktype_map[index].LinkTypePath) != NULL)
        {
            AnscTraceFlow(("%s: return index %d\n", __FUNCTION__, index));
            return g_linktype_map[index].LinkTypeStr;
        }
    }

    return NULL;
}

char* CosaUtilGetLinkTypeStr(COSA_DML_LINK_TYPE LinkType)
{
    int index=0;
    AnscTraceFlow(("%s: %d\n", __FUNCTION__, LinkType));
    for(index=0; index<COSA_DML_LINK_TYPE_TOTAL; index++)
    {
        if(g_linktype_map[index].LinkType == LinkType)
        {
            AnscTraceFlow(("%s: return index %d\n", __FUNCTION__, index));
            return g_linktype_map[index].LinkTypeStr;
        }
    }
    return NULL;
}

char* CosaUtilGetLinkTypePath(COSA_DML_LINK_TYPE LinkType)
{
    int index=0;
    AnscTraceFlow(("%s: %d\n", __FUNCTION__, LinkType));
    for(index=0; index<COSA_DML_LINK_TYPE_TOTAL; index++)
    {
        if(g_linktype_map[index].LinkType == LinkType)
        {
            AnscTraceFlow(("%s: return index %d\n", __FUNCTION__, index));
            return g_linktype_map[index].LinkTypePath;
        }
    }
    return NULL;
}

COSA_DML_LINK_TYPE CosaUtilGetLinkTypeFromPath(char*pLinkTypePath)
{
    int index=0;
    AnscTraceFlow(("%s: %s\n", __FUNCTION__, pLinkTypePath));
    for(index=0; index<COSA_DML_LINK_TYPE_TOTAL; index++)
    {
        //if(!strncmp(g_linktype_map[index].LinkTypePath, pLinkTypePath, 
        //        sizeof(g_linktype_map[index].LinkTypePath)))
        if(strstr(pLinkTypePath, g_linktype_map[index].LinkTypePath))
        {
            AnscTraceFlow(("%s: return index %d\n", __FUNCTION__, index));
            return g_linktype_map[index].LinkType;
        }
    }
    return COSA_DML_LINK_TYPE_LAST;
}


ANSC_STATUS
CosaUtilConstructLowerLayers
    (
        COSA_DML_LINK_TYPE          LinkType,
        ULONG                       InstNumber,
        char*                       pLowerLayersBuf,
        PULONG                      pBufLen        
    )
{
    char*                           linkTypePath;

    linkTypePath = CosaUtilGetLinkTypePath(LinkType);

    /* TBD - Need to check the pBufLen */

    if ( LinkType == COSA_DML_LINK_TYPE_Bridge )
    {
        /* Special processing for Bridge type LowerLayers */
        _ansc_sprintf(pLowerLayersBuf, "%s%d.Port.1", linkTypePath, InstNumber);
    }
    else
    {
        _ansc_sprintf(pLowerLayersBuf, "%s%d", linkTypePath, InstNumber);
    }

    AnscTraceFlow(("%s, size %d, buf len %d\n", pLowerLayersBuf, _ansc_strlen(pLowerLayersBuf), *pBufLen));
    return  ANSC_STATUS_SUCCESS;
}


/*
 *  Retrieve the parameter Name of the LowerLayer
 */
ANSC_STATUS
CosaUtilGetLowerLayerName
    (
        COSA_DML_LINK_TYPE          LinkType,
        ULONG                       InstNumber,
        char*                       pParamValueBuf,
        PULONG                      pBufLen        
    )
{
    ANSC_STATUS                     returnStatus;
    char                            pParamPath[256] = {0};
    ULONG                           ParamPathLen    = sizeof(pParamPath);

    returnStatus = CosaUtilConstructLowerLayers(LinkType, InstNumber, pParamPath, &ParamPathLen);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }
    else
    {
        _ansc_strcat(pParamPath, ".Name");
    }

    returnStatus = CosaGetParamValueString(pParamPath, pParamValueBuf, pBufLen);

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        AnscTraceFlow(("CosaUtilGetLowerLayerName -- value %s\n", pParamValueBuf));

        return  returnStatus;
    }
    else
    {
        AnscTraceWarning(("CosaUtilGetLowerLayerName -- failure %d, to retrieve %s\n", returnStatus, pParamPath));

        return  returnStatus;
    }
}


/*
 * Find the management port in Bridge and return name from that port
 * It is the name of the bridge.
 */
PUCHAR
CosaUtilFindBridgeName(char* pBridgePath)
{
    ULONG                           j                           = 0;
    ULONG                           ulEntryNameLen              = 256;
    UCHAR                           ucEntryParamName[256]       = {0};
    UCHAR                           ucEntryNameValue[256]       = {0};
   
    UCHAR                           ucLowerEntryPath[256]       = {0};
    UCHAR                           ucLowerEntryName[256]       = {0};
    ULONG                           ulEntryPortNum              = 0;
    PUCHAR                          pMatchedBridgeName          = NULL;
    BOOL                            bMgrPort                    = FALSE;

    _ansc_sprintf(ucLowerEntryPath, "%s%s", pBridgePath, ".PortNumberOfEntries"); 
                        
    ulEntryPortNum = CosaGetParamValueUlong(ucLowerEntryPath);  
    AnscTraceFlow(("%s: Param:%s,port num:%d\n", __FUNCTION__, ucLowerEntryPath, ulEntryPortNum));

    for ( j = 1; j<= ulEntryPortNum; j++) {
        _ansc_sprintf(ucLowerEntryName, "%s%s%d", pBridgePath, ".Port.", j);
        _ansc_sprintf(ucEntryParamName, "%s%s%d%s", pBridgePath, ".Port.", j, ".ManagementPort");
        AnscTraceFlow(("%s: Param:%s,Param2:%s\n",__FUNCTION__, ucLowerEntryName, ucEntryParamName)); 
                        
        bMgrPort = CosaGetParamValueBool(ucEntryParamName);
        if(bMgrPort)
        {
            _ansc_sprintf(ucEntryParamName, "%s%s%d%s", pBridgePath, ".Port.", j, ".Name");
                
            ulEntryNameLen = sizeof(ucEntryNameValue);
            if (0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) 
            {
                // not include port instance.
                pMatchedBridgeName =  AnscCloneString(ucEntryNameValue);
                AnscTraceFlow(("%s: J:%d, Name:%s\n", __FUNCTION__, j, pMatchedBridgeName));
                break;
            }
        }
    }

    return pMatchedBridgeName;
}

/*
 * Return Device.Bridging.Bridge.x
 * not include port instance number.
 */
PUCHAR
CosaUtilFindBridgePath(char* pBridgeName)
{
    ULONG                           ulNumOfEntries              = 0;
    ULONG                           i                           = 0;
    ULONG                           j                           = 0;
    ULONG                           ulEntryNameLen              = 256;
    UCHAR                           ucEntryParamName[256]       = {0};
    UCHAR                           ucEntryNameValue[256]       = {0};
    UCHAR                           ucEntryFullPath[256]        = {0};
    UCHAR                           ucLowerEntryPath[256]       = {0};
    UCHAR                           ucLowerEntryName[256]       = {0};
    ULONG                           ulEntryInstanceNum          = 0;
    ULONG                           ulEntryPortNum              = 0;
    PUCHAR                          pMatchedLowerLayer          = NULL;
    

    ulNumOfEntries =  CosaGetParamValueUlong("Device.Bridging.BridgeNumberOfEntries");
    CcspTraceInfo(("----------CosaUtilGetLowerLayers, bridgenum:%d\n", ulNumOfEntries));
    AnscTraceFlow(("%s: bridgenum:%d\n", __FUNCTION__, ulNumOfEntries));
    for ( i = 0 ; i < ulNumOfEntries; i++ )
    {
        ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.Bridging.Bridge.", i);
        CcspTraceInfo(("----------CosaUtilGetLowerLayers, instance num:%d\n", ulEntryInstanceNum));
        AnscTraceFlow(("%s: instance num:%d\n", __FUNCTION__, ulEntryInstanceNum));
        if ( ulEntryInstanceNum )
        {
            _ansc_sprintf(ucEntryFullPath, "%s%d", "Device.Bridging.Bridge.", ulEntryInstanceNum);
            _ansc_sprintf(ucLowerEntryPath, "%s%s", ucEntryFullPath, ".PortNumberOfEntries"); 
                        
            ulEntryPortNum = CosaGetParamValueUlong(ucLowerEntryPath);  
            CcspTraceInfo(("----------CosaUtilGetLowerLayers, Param:%s,port num:%d\n",ucLowerEntryPath, ulEntryPortNum));

            for ( j = 1; j<= ulEntryPortNum; j++) {
                _ansc_sprintf(ucLowerEntryName, "%s%s%d", ucEntryFullPath, ".Port.", j);
                _ansc_sprintf(ucEntryParamName, "%s%s%d%s", ucEntryFullPath, ".Port.", j, ".Name");
                CcspTraceInfo(("----------CosaUtilGetLowerLayers, Param:%s,Param2:%s\n", ucLowerEntryName, ucEntryParamName));
                        
                ulEntryNameLen = sizeof(ucEntryNameValue);
                if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                        AnscEqualString(ucEntryNameValue, pBridgeName , TRUE ) )
                {
                    // not include port instance.
                    pMatchedLowerLayer =  AnscCloneString(ucEntryFullPath);
                    CcspTraceInfo(("----------CosaUtilGetLowerLayers, J:%d, LowerLayer:%s\n", j, pMatchedLowerLayer));
                    break;
                }
            }
        }
    }

    return pMatchedLowerLayer;
}
/*
    CosaUtilGetFullPathNameByKeyword
    
   Description:
        This funcation serves for searching other pathname  except lowerlayer.
        
    PUCHAR                      pTableName
        This is the Table names divided by ",". For example 
        "Device.Ethernet.Interface., Device.Dhcpv4." 
        
    PUCHAR                      pParameterName
        This is the parameter name which hold the keyword. eg: "name"
        
    PUCHAR                      pKeyword
        This is keyword. eg: "wan0".

    return value
        return result string which need be free by the caller.
*/
PUCHAR
CosaUtilGetFullPathNameByKeyword
    (
        PUCHAR                      pTableName,
        PUCHAR                      pParameterName,
        PUCHAR                      pKeyword
    )
{

    ULONG                           ulNumOfEntries              = 0;
    ULONG                           ulChildNumOfEntries         = 0;
    ULONG                           i                           = 0;
    ULONG                           ulEntryNameLen              = 256;
    UCHAR                           ucEntryParamName[256]       = {0};
    UCHAR                           ucEntryNameValue[256]       = {0};
    UCHAR                           ucTmp[128]                  = {0};
    UCHAR                           ucTmp2[128]                 = {0};
    UCHAR                           ucEntryFullPath[256]        = {0};
    PUCHAR                          pMatchedLowerLayer          = NULL;
    ULONG                           ulEntryInstanceNum          = 0;
    ULONG                           ucLength                    = 0;    
    PANSC_TOKEN_CHAIN               pTableListTokenChain        = (PANSC_TOKEN_CHAIN)NULL;
    PANSC_STRING_TOKEN              pTableStringToken           = (PANSC_STRING_TOKEN)NULL;
    PUCHAR                          pString                     = NULL;
    PUCHAR                          pString2                    = NULL;

    if ( !pTableName || AnscSizeOfString(pTableName) == 0 ||
         !pKeyword   || AnscSizeOfString(pKeyword) == 0   ||
         !pParameterName   || AnscSizeOfString(pParameterName) == 0
       )
    {
        return NULL;
    }

    pTableListTokenChain = AnscTcAllocate(pTableName, ",");

    if ( !pTableListTokenChain )
    {
        return NULL;
    }

    while ((pTableStringToken = AnscTcUnlinkToken(pTableListTokenChain)))
    {
        if ( pTableStringToken->Name )
        {
            /* Get the string XXXNumberOfEntries */
            pString2 = &pTableStringToken->Name[0];
            pString  = pString2;
            for (i = 0;pTableStringToken->Name[i]; i++)
            {
                if ( pTableStringToken->Name[i] == '.' )
                {
                    pString2 = pString;
                    pString  = &pTableStringToken->Name[i+1];
                }
            }

            pString--;
            pString[0] = '\0';
            _ansc_sprintf(ucTmp2, "%s%s", pString2, "NumberOfEntries");                
            pString[0] = '.';

            /* Enumerate the entry in this table */
            if ( TRUE )
            {
                pString2--;
                pString2[0]='\0';
                _ansc_sprintf(ucTmp, "%s.%s", pTableStringToken->Name, ucTmp2);                
                pString2[0]='.';
                ulNumOfEntries =       CosaGetParamValueUlong(ucTmp);

                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex(pTableStringToken->Name, i);

                    if ( ulEntryInstanceNum )
                    {
                        _ansc_sprintf(ucEntryFullPath, "%s%d%s", pTableStringToken->Name, ulEntryInstanceNum, ".");

                        _ansc_sprintf(ucEntryParamName, "%s%s", ucEntryFullPath, pParameterName);
               
                        ulEntryNameLen = sizeof(ucEntryNameValue);
                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             AnscEqualString(ucEntryNameValue, pKeyword, TRUE ) )
                        {
                            pMatchedLowerLayer =  AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }

            if ( pMatchedLowerLayer )
            {
                break;
            }
        }

        AnscFreeMemory(pTableStringToken);
    }

    if ( pTableListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pTableListTokenChain);
    }

/*
    CcspTraceWarning
        ((
            "CosaUtilGetFullPathNameByKeyword: %s matched parameters(%s) with keyword %s in the table %s(%s)\n",
            pMatchedLowerLayer ? "Found a":"Not find any",
            pMatchedLowerLayer ? pMatchedLowerLayer : "",
            pKeyword,
            pTableName,
            pParameterName
        ));
*/

    return pMatchedLowerLayer;
}

#if  defined(_ANSC_LINUX)

ULONG
CosaUtilChannelValidate
    (
        UINT                       uiRadio,
        ULONG                      Channel
    )
{
    unsigned long channelList_5G [] = {36, 40, 44, 48, 52, 56, 60, 64, 149, 153, 157, 161, 165};
    int i;
    switch(uiRadio)
    {
        case 1:
             if((Channel < 1) || (Channel > 11))
                return 0;
             return 1;
        case 2:
             for(i=0; i<13; i++)
             {
                if(Channel == channelList_5G[i])
                  return 1;
             }
             return 0;
             break;
        default:
             break;
     }
     return 0;
}

ULONG
CosaUtilChannelValidate2
    (
        UINT                       uiRadio,
        ULONG                      Channel,
        char                       *channelList
    )
{
    // This should be updated to use the possible channels list  Device.WiFi.Radio.1.PossibleChannels instead of a static list.
    unsigned long channelList_5G [] = {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161, 165};
    int i;

    // Channel maybe 0 if radio is disabled or auto channel was set
    if (Channel == 0) {
        return 1;
    }

    // If channelList is provided use it.
    if (channelList != NULL) {
        char chan[4];
        sprintf(chan,"%d",Channel);
        if (strstr(channelList,chan) != NULL) {
            return 1;
        } 
    }

    switch(uiRadio)
    {
        case 1:
             if((Channel < 0) || (Channel > 11))
                return 0;
             return 1;
        case 2:
             for(i=0; i<24; i++)
             {
                if(Channel == channelList_5G[i])
                  return 1;
             }
             return 0;
             break;
        default:
             break;
     }
     return 0;
}

ULONG CosaUtilIoctlXXX(char * if_name, char * method, void * input)
{
    ULONG ret = 0;
    struct ifreq ifr;
	int sock;

    if (!if_name || !method)
    {
        return 0;
    }
    
	sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0 ) 
    {
        return 0;
    }

    strncpy(ifr.ifr_name, if_name, IF_NAMESIZE);

    if (!strcmp(method, "mtu"))
    {
        if (ioctl(sock, SIOCGIFMTU, &ifr) == 0)
        {
            ret = ifr.ifr_mtu;
            goto _EXIT;
        }
        else 
        {
            goto _EXIT;
        }
    }
    if (!strcmp(method, "setmtu"))
    {
        ULONG mtu = *(ULONG *)input;
        ifr.ifr_mtu = mtu;

        ret = ioctl(sock, SIOCSIFMTU, &ifr);
        goto _EXIT;
    }
    else if (!strcmp(method, "status"))
    {
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
        {
            ret = ifr.ifr_flags;
            goto _EXIT;

        }
        else 
        {
            goto _EXIT;
        }        
    }
    else if (!strcmp(method, "netmask"))
    {
        if (ioctl(sock, SIOCGIFNETMASK, &ifr) == 0)
        {
            memcpy(&ret, ifr.ifr_netmask.sa_data + 2,4);
            goto _EXIT;

        }
        else 
        {
            goto _EXIT;
        }        
    }
    else if (!strcmp(method, "set_netmask"))
    {
        ULONG mask = *(ULONG *)input;

        /*first get netmask then modify it*/
        if (ioctl(sock, SIOCGIFNETMASK, &ifr) == 0)
        {
            memcpy(ifr.ifr_netmask.sa_data + 2, &mask, sizeof(mask));
            ret = ioctl(sock, SIOCSIFNETMASK, &ifr);
            
            goto _EXIT;
        }
        else 
        {
            ret = -1;
            goto _EXIT;
        }        
    }
_EXIT:
    close(sock);
    return ret;
}

#else

ULONG
CosaUtilIoctlXXX
    (
        char*                       if_name,
        char*                       method,
        void*                       input
    )
{
    return  IFF_UP;
}

#endif


#define NET_STATS_FILE "/proc/net/dev"
int CosaUtilGetIfStats(char * ifname, PCOSA_DML_IF_STATS pStats)
{
    int    i;
    FILE * fp;
    char buf[1024] = {0} ;
    char * p;
    int    ret = 0;

    fp = fopen(NET_STATS_FILE, "r");
    
    if (fp)
    {
        i = 0;
        while (fgets(buf, sizeof(buf), fp))
        {
            if (++i <= 2) continue;
            
            if (p = strchr(buf, ':'))
            {
                if (strstr(buf, ifname))
                {
                    memset(pStats, 0, sizeof(*pStats));
                    if (sscanf(p+1, "%d %d %d %d %*d %*d %*d %*d %d %d %d %d %*d %*d %*d %*d", 
                               &pStats->BytesReceived, &pStats->PacketsReceived, &pStats->ErrorsReceived, &pStats->DiscardPacketsReceived,
                               &pStats->BytesSent, &pStats->PacketsSent, &pStats->ErrorsSent, &pStats->DiscardPacketsSent
                            ) == 8)
                    {
                        /*found*/
                        ret = TRUE;
                        goto _EXIT;
                    }
                }
                else continue;
            }
            else continue;
        }
        
    }   

_EXIT:
    fclose(fp);
    return ret;
}

ULONG NetmaskToNumber(char *netmask)
{
    char * pch;
    ULONG val;
    ULONG i;
    ULONG count = 0;

    pch = strtok(netmask, ".");
    while (pch != NULL)
    {
        val = atoi(pch);
        for (i=0;i<8;i++)
        {
            if (val&0x01)
            {
                count++;
            }
            val = val >> 1;
        }
        pch = strtok(NULL,".");
    }
    return count;
}

ANSC_STATUS
CosaUtilGetStaticRouteTable
    (
        UINT                        *count,
        StaticRoute                 **out_sroute
    )
{
#if ( defined(_COSA_DRG_CNS_) || defined(_COSA_DRG_TPG_))
    int i;
    int j;
    StaticRoute *sroute;
    char cmd[512];
    char line_buf[512];
    int line_count;
    char *pch = NULL;

    if (NULL == count || NULL == out_sroute) {
        return ANSC_STATUS_FAILURE;
    }
    
    *count = 0;
    
    snprintf(cmd, sizeof(cmd), "route -n | grep -v \"^127.0.0\" > %s", "/tmp/.route_table_tmp");
    system(cmd);

    FILE *fp = fopen("/tmp/.route_table_tmp", "r");
    FILE *fp2 = NULL;
    
    if (!fp) {
        return ANSC_STATUS_FAILURE;
    }
    
    line_count = 0;
    while (NULL != fgets(line_buf, sizeof(line_buf), fp)) {
        line_count++;
    }
    
    *count = line_count - 2; // Skip the first two lines
    if (*count <= 0) {
        fclose(fp);
        return ANSC_STATUS_SUCCESS;
    }

    sroute = (StaticRoute *) malloc(sizeof(StaticRoute) * (*count));
    if (NULL == sroute) {
        fclose(fp);
        return ANSC_STATUS_FAILURE;
    }
    bzero(sroute, sizeof(StaticRoute) * (*count));
    
    // Seek to beginning of file
    fseek(fp, 0, SEEK_SET);
    
    // Read past the first two lines
    for (i = 0; i < 2; i++) {
        if (NULL == fgets(line_buf, sizeof(line_buf), fp)) {
    	    break;
        }
    }
    
    for (i = 0; i < *count && fgets(line_buf, sizeof(line_buf), fp) != NULL; i++) {
        char *start;
        int len;
        
        // Set the destination LAN IP
        start = line_buf;
        len = strcspn(start, " ");
        if (len >= 16) {
            len = 16 - 1;
        }
        strncpy(sroute[i].dest_lan_ip, start, len);
        sroute[i].dest_lan_ip[len] = 0;
        
        // Set the gateway
        start += len;
        start += strspn(start, " ");
        len = strcspn(start, " ");
        if (len >= 16) {
            len = 16 - 1;
        }
        strncpy(sroute[i].gateway, start, len);
        sroute[i].gateway[len] = 0;
        
        // Set the netmask
        start += len;
        start += strspn(start, " ");
        len = strcspn(start, " ");
        if (len >= 16) {
            len = 16 - 1;
        }
        strncpy(sroute[i].netmask, start, len);
        sroute[i].netmask[len] = 0;
        
        // Skip the next one columns
        for (j = 0; j < 1; j++) {
            start += len;
	        start += strspn(start, " ");
	        len = strcspn(start, " ");
        }
        
        // set metric
        start += len;
        start += strspn(start, " ");
        sroute[i].metric = atoi(start);

        // Skip the next two columns
        for (j = 0; j < 2; j++) {
            start += len;
	        start += strspn(start, " ");
	        len = strcspn(start, " ");
        }

        // Lookup the interface
        start += len;
        start += strspn(start, " ");
        
        // Set the route's interface
        strncpy(sroute[i].dest_intf, start, 9);

        len = strlen(sroute[i].dest_intf);
        if (sroute[i].dest_intf[len-1] == '\n')
            sroute[i].dest_intf[len-1] = '\0';

        if (TRUE)
        {
            snprintf(cmd, sizeof(cmd), "/sbin/ip route show %s/%d",
                     sroute[i].dest_lan_ip,
                     NetmaskToNumber(sroute[i].netmask));

            if (((fp2 = popen(cmd,"r")) != NULL) && (fgets(line_buf, sizeof(line_buf), fp2)))
            {
                pch = strtok(line_buf, " ");

                while(pch != NULL)
                {
                    if (!strcmp(pch, "proto"))
                    {
                        pch = strtok(NULL, " ");
                        strcpy(sroute[i].origin, pch);
                        break;
                    }
                    pch = strtok(NULL, " ");
                }
            }
        }

    }

    if (fp2)
    {
        fclose(fp2);
    }
    fclose(fp);
    
    *out_sroute = sroute;

    return ANSC_STATUS_SUCCESS;
#else
    return ANSC_STATUS_SUCCESS;
#endif

}

#define IPV6_ADDR_LOOPBACK      0x0010U
#define IPV6_ADDR_LINKLOCAL     0x0020U
#define IPV6_ADDR_SITELOCAL     0x0040U
#define IPV6_ADDR_COMPATv4      0x0080U
#define IPV6_ADDR_SCOPE_MASK    0x00f0U

#if  !defined(_COSA_SIM_)

/* function: is_usg_in_bridge_mode
   description: judge if USG is in bridge mode or not
   input: none
   output: TRUE in bridge mode, otherwise FALSE
   return: ANSC_STATUS_SUCCESS/ANSC_STATUS_FAILURE
 */
ANSC_STATUS is_usg_in_bridge_mode(BOOL *pBridgeMode)
{
    char retVal[128] = {'\0'};
    ULONG retLen;
        retLen = sizeof( retVal );
    if (pBridgeMode == NULL)
        return ANSC_STATUS_FAILURE;

    if (0 == CosaGetParamValueString(
                "Device.X_CISCO_COM_DeviceControl.LanManagementEntry.1.LanMode",
                retVal,
                &retLen)){
        if (AnscEqualString(retVal, "router", TRUE))
            *pBridgeMode = FALSE;
        else
            *pBridgeMode = TRUE;
        return ANSC_STATUS_SUCCESS;
    }else
        return ANSC_STATUS_FAILURE;

}

/*caller must free(*pp_info)*/
#define _PROCNET_IFINET6  "/proc/net/if_inet6"
#define MAX_INET6_PROC_CHARS 200

typedef struct v6sample {
           unsigned int bitsToMask;
           char intrName[20];
           unsigned char ipv6_addr[40];
           char address6[40];
           unsigned int devIndex;
           unsigned int flags;
           unsigned int scopeofipv6;
           char prefix_v6[41];
}ifv6Details;

int getIpv6Scope(int scope_v6)
{
    int scopeToReturn = scope_v6 & IPV6_ADDR_SCOPE_MASK;

            if(scopeToReturn == 0)
                return IPV6_ADDR_SCOPE_GLOBAL;                          
            else if( scopeToReturn == IPV6_ADDR_LINKLOCAL)
                return IPV6_ADDR_SCOPE_LINKLOCAL;
            else if( scopeToReturn == IPV6_ADDR_SITELOCAL)
                return IPV6_ADDR_SCOPE_SITELOCAL;
            else if( scopeToReturn == IPV6_ADDR_COMPATv4)
                return IPV6_ADDR_SCOPE_COMPATv4;
            else if( scopeToReturn == IPV6_ADDR_LOOPBACK)
                return IPV6_ADDR_SCOPE_LOOPBACK;
            else
                return IPV6_ADDR_SCOPE_UNKNOWN;
}

int parseProcfileParams(char* lineToParse,ifv6Details *detailsToParse,char* interface)
{

    struct sockaddr_in6 sAddr6;
    char splitv6[8][5];
    CcspTraceDebug(("%s, Parse the line read from file\n",__FUNCTION__));

    if (lineToParse == NULL)
           return 0;

    if(sscanf(lineToParse, "%s %x %x %x %x %s", detailsToParse->ipv6_addr,&detailsToParse->devIndex,
              &detailsToParse->bitsToMask,&detailsToParse->scopeofipv6,&detailsToParse->flags,detailsToParse->intrName) == 6)
    {

       CcspTraceDebug(("%s, Check if interface matches\n",__FUNCTION__));
       if (!strcmp(interface, detailsToParse->intrName))
       {
           CcspTraceDebug(("%s,Interface matched\n",__FUNCTION__));
           //Convert the raw interface ip to IPv6 format
           int position,placeholder=0;
           for (position=0; detailsToParse->ipv6_addr[position] != '\0'; position++)
           {
               detailsToParse->address6[placeholder] = detailsToParse->ipv6_addr[position];
               placeholder++;
               // Positions at which ":" should be put.
               if((position==3)||(position==7)||(position==11)||(position==15)||
                   (position==19)||(position==23)||(position==27))
               {
                   detailsToParse->address6[placeholder] = ':';
                   placeholder++;
               }
           }
           detailsToParse->address6[placeholder] = '\0';
           CcspTraceDebug(("%s,Interface IPv6 address calculation\n",__FUNCTION__));
           CcspTraceDebug(("IPv6 address is %s \n",detailsToParse->address6));
           inet_pton(AF_INET6, detailsToParse->address6,(struct sockaddr *) &sAddr6.sin6_addr);
           sAddr6.sin6_family = AF_INET6;
           inet_ntop(AF_INET6, (struct sockaddr *) &sAddr6.sin6_addr, detailsToParse->address6, sizeof(detailsToParse->address6));
           CcspTraceDebug(("%s,Interface IPv6 address is: %s\n",__FUNCTION__,detailsToParse->address6));

           if(sscanf(lineToParse, "%4s%4s%4s%4s%4s%4s%4s%4s", splitv6[0], splitv6[1], splitv6[2],
                                                              splitv6[3], splitv6[4],splitv6[5], splitv6[6], splitv6[7])==8)
           {
               memset(detailsToParse->prefix_v6,0,sizeof(detailsToParse->prefix_v6));
               int iCount =0;
               for (iCount=0; (iCount< ( detailsToParse->bitsToMask%16 ? (detailsToParse->bitsToMask/16+1):detailsToParse->bitsToMask/16)) && iCount<8; iCount++)
               {
                  sprintf(detailsToParse->prefix_v6+strlen(detailsToParse->prefix_v6), "%s:",splitv6[iCount]);
               }
               CcspTraceDebug(("%s,Interface IPv6 prefix calculation done\n",__FUNCTION__));
            }
            return 1;
      }
      else
      {
         CcspTraceDebug(("%s,Interface not found\n",__FUNCTION__));
         return 0;
      }
    }
    else
    {
      CcspTraceError(("%s,Interface line read failed\n",__FUNCTION__));
      return 0;
    }
}
int CosaUtilGetIpv6AddrInfo (char * ifname, ipv6_addr_info_t ** pp_info, int * p_num)
{
    FILE * fp = NULL;
    ipv6_addr_info_t * p_ai = NULL;
    char procLine[MAX_INET6_PROC_CHARS];
    ifv6Details v6Details;
    int parsingResult;
    int    i = 0;
    
    if (!ifname || !pp_info || !p_num)
        return -1;

    *p_num = 0;

    fp = fopen(_PROCNET_IFINET6, "r");
    if (!fp)
        return -1;
    
    while(fgets(procLine, MAX_INET6_PROC_CHARS, fp))
    {

        parsingResult=parseProcfileParams(procLine, &v6Details,ifname);
        if (parsingResult == 1)
        { 
            (*p_num)++;
            *pp_info = realloc(*pp_info,  *p_num * sizeof(ipv6_addr_info_t));
            if (!*pp_info) 
            {
                fclose(fp);
                return -1;
            }

            p_ai = &(*pp_info)[*p_num-1];
            strncpy(p_ai->v6addr, v6Details.address6, sizeof(p_ai->v6addr));

            // Get the scope of IPv6
            p_ai->scope = getIpv6Scope(v6Details.scopeofipv6);
            CcspTraceInfo(("%s,Interface scope is : %d\n",__FUNCTION__,v6Details.scopeofipv6));           
 
            memset(p_ai->v6pre, 0, sizeof(p_ai->v6pre));
            if(v6Details.prefix_v6)
                 strcpy(p_ai->v6pre,v6Details.prefix_v6);
            else
                CcspTraceInfo(("%s,Interface ipv6 prefix is NULL\n",__FUNCTION__));

            sprintf(p_ai->v6pre+strlen(p_ai->v6pre), ":/%d", v6Details.bitsToMask);
            
        }
    }

    fclose(fp);
    return 0;
}

#else
ANSC_STATUS is_usg_in_bridge_mode(BOOL *pBridgeMode)
{
    *pBridgeMode = FALSE;
    return ANSC_STATUS_SUCCESS;
}

int CosaUtilGetIpv6AddrInfo (char * ifname, ipv6_addr_info_t ** pp_info, int * p_num)
{
    *p_num = 0;
    *pp_info = NULL;
    return 0;
}
#endif

int safe_strcpy(char * dst, char * src, int dst_size)
{
    if (!dst || !src) return -1;

    memset(dst, 0, dst_size);
    _ansc_strncpy(dst, src, _ansc_strlen(src)<=dst_size-1 ? _ansc_strlen(src):dst_size-1 );

    return 0;
}

int  __v6addr_mismatch(char * addr1, char * addr2, int pref_len)
{
    char * p = NULL, * p2 = NULL;
    int i = 0;
    int num = 0;
    int mask = 0;
    char addr1_buf[128] = {0};
    char addr2_buf[128] = {0};
    struct in6_addr in6_addr1;
    struct in6_addr in6_addr2;

    if (!addr1 || !addr2)
        return -1;
    
    safe_strcpy(addr1_buf, addr1, sizeof(addr1_buf));
    safe_strcpy(addr2_buf, addr2, sizeof(addr2_buf));

    if ( inet_pton(AF_INET6, addr1_buf, &in6_addr1) != 1)
        return -8;
    if ( inet_pton(AF_INET6, addr2_buf, &in6_addr2) != 1)
        return -9;

    num = (pref_len%8)? (pref_len/8+1) : pref_len/8;
    if (pref_len%8)
    {
        for (i=0; i<num-1; i++)
            if (in6_addr1.s6_addr[i] != in6_addr2.s6_addr[i])
                return -3;

        for (i=0; i<pref_len%8; i++)
            mask += 1<<(7-i);
        
        if ( (in6_addr1.s6_addr[num-1] &  mask) == (in6_addr2.s6_addr[num-1] & mask))
            return 0;
        else
            return -4;
    }
    else 
    {
        for (i=0; i<num; i++)
            if (in6_addr1.s6_addr[i] != in6_addr2.s6_addr[i])
                return -5;
    }

    return 0;
}

int  __v6addr_mismatches_v6pre(char * v6addr,char * v6pre)
{
    int pref_len = 0;
    char addr_buf[128] = {0};
    char pref_buf[128] = {0};
    char * p = NULL;

    if (!v6addr || !v6pre)
        return -1;
    
    safe_strcpy(addr_buf, v6addr, sizeof(addr_buf));
    safe_strcpy(pref_buf, v6pre, sizeof(pref_buf));

    if (!(p = strchr(pref_buf, '/')))
        return -1;

    if (sscanf(p, "/%d", &pref_len) != 1)
        return -2;
    *p = 0;

    return __v6addr_mismatch(addr_buf, pref_buf, pref_len);
}

int  __v6pref_mismatches(char * v6pref1,char * v6pref2)
{
    int pref1_len = 0;
    int pref2_len = 0;
    char pref1_buf[128] = {0};
    char pref2_buf[128] = {0};
    char * p = NULL;

    if (!v6pref1 || !v6pref2)
        return -1;
    
    safe_strcpy(pref1_buf, v6pref1, sizeof(pref1_buf));
    safe_strcpy(pref2_buf, v6pref2, sizeof(pref2_buf));

    if (!(p = strchr(pref1_buf, '/')))
        return -1;

    if (sscanf(p, "/%d", &pref1_len) != 1)
        return -2;
    *p = 0;

    if (!(p = strchr(pref2_buf, '/')))
        return -1;

    if (sscanf(p, "/%d", &pref2_len) != 1)
        return -2;
    *p = 0;

    if (pref1_len != pref2_len)
        return -7;

    return __v6addr_mismatch(pref1_buf, pref2_buf, pref1_len);
}

int CosaDmlV6AddrIsEqual(char * p_addr1, char * p_addr2)
{
    if (!p_addr1 || !p_addr2)
        return 0;

    return !__v6addr_mismatch(p_addr1, p_addr2, 128);
}

int CosaDmlV6PrefIsEqual(char * p_pref1, char * p_pref2)
{
    if (!p_pref1 || !p_pref2)
        return 0;

    return !__v6pref_mismatches(p_pref1, p_pref2);
}

int _write_sysctl_file(char * fn, int val)
{
    FILE * fp = fopen(fn, "w+");

    if (fp)
    {
        fprintf(fp, "%d", val);
        fclose(fp);
    }
    
    return 0;
}

/*
 * IPV4 Address check functions
 */
/* addr is in network order */
int IPv4Addr_IsSameNetwork(uint32_t addr1, uint32_t addr2, uint32_t mask)
{
    return (addr1 & mask) == (addr2 & mask);
}

/* addr is in network order */
int IPv4Addr_IsLoopback(uint32_t addr)
{
    return (addr & htonl(0xff000000)) == htonl(0x7f000000);
}

/* addr is in network order */
int IPv4Addr_IsMulticast(uint32_t addr)
{
    return (addr & htonl(0xf0000000)) == htonl(0xe0000000);
}

/* addr is in network order */
int IPv4Addr_IsBroadcast(uint32_t addr, uint32_t net, uint32_t mask)
{
    /* all ones or all zeros (old) */
    if (addr == 0xffffffff)
        return 1;

    /* on the same sub network and host bits are all ones */
    if (IPv4Addr_IsSameNetwork(addr, net, mask)
            && (addr & ~mask) == (0xffffffff & ~mask))
        return 1;

    return 0;
}

/* addr is in network order */
int IPv4Addr_IsNetworkAddr(uint32_t addr, uint32_t net, uint32_t mask)
{
    if (IPv4Addr_IsSameNetwork(addr, net, mask)
            && (addr & ~mask) == 0)
        return 1;

    return 0;
}

/* addr is in network order */
int IPv4Addr_IsNetmaskValid(uint32_t netmask)
{
    uint32_t mask;
    uint32_t hostorder = ntohl(netmask);

    /* first zero */
    for (mask = 1UL << 31 ; mask != 0; mask >>= 1)
        if ((hostorder & mask) == 0)
            break;

    /* there is no one ? */
    for (; mask != 0; mask >>= 1)
        if ((hostorder & mask) != 0)
            return 0;

    return 1;
}

int IPv4Addr_IsClassA(uint32_t addr)
{
    return IPV4_CLASSA(ntohl(addr));
}

int IPv4Addr_IsClassB(uint32_t addr)
{
    return IPV4_CLASSB(ntohl(addr));
}

int IPv4Addr_IsClassC(uint32_t addr)
{
    return IPV4_CLASSC(ntohl(addr));
}

int IPv4Addr_IsClassD(uint32_t addr)
{
    return IPV4_CLASSD(ntohl(addr));
}

int vsystem(const char *fmt, ...)
{
    char cmd[512];
    va_list ap;
    int n;

    va_start(ap, fmt);
    n = vsnprintf(cmd, sizeof(cmd), fmt, ap);
    va_end(ap);

    if (n < 0 || n >= sizeof(cmd))
        return -1;

    fprintf(stderr, "%s: %s\n", __FUNCTION__, cmd);
    return system(cmd);
}

void chomp(char *line)
{
    if (!line || strlen(line) < 1)
        return;

    if (line[strlen(line) - 1] == '\n')
        line[strlen(line) - 1] = '\0';

    return;
}

int get_if_hwaddr(const char *ifname, char *mac, size_t size)
{
    int sockfd;
    struct ifreq ifr;
    unsigned char *ptr;

    if (!ifname || !mac || size < sizeof("00:00:00:00:00:00"))
        return -1;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", ifname);

    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1) {
        perror("ioctl");
        close(sockfd);
        return -1;
    }

    ptr = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    snprintf(mac, size, "%02x:%02x:%02x:%02x:%02x:%02x",
            ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);

    close(sockfd);
    return 0;
}

#ifdef IPV4ADDR_TEST

#define NEXTARG    argc--, argv++

enum {
    CMD_ISLOOP,
    CMD_ISMULT,
    CMD_COMP,
    CMD_ISBORAD,
    CMD_ISNET,
};

struct arguments {
    int             cmd;
    struct in_addr  addr;
    struct in_addr  net;
    struct in_addr  mask;
};

static uint32_t InaddrGetU32(struct in_addr *addr)
{
    return (uint32_t)addr->s_addr;
}

static void Usage(void)
{
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "    ./v4addr l ADDR\n");
    fprintf(stderr, "    ./v4addr m ADDR\n");
    fprintf(stderr, "    ./v4addr c ADDR1 ADDR2 MASK\n");
    fprintf(stderr, "    ./v4addr b ADDR NET MASK\n");
    fprintf(stderr, "    ./v4addr n ADDR NET MASK\n");
}

static int ParseArgs(int argc, char *argv[], struct arguments *args)
{
    if (argc < 3) {
        Usage();
        exit(1);
    }

    bzero(args, sizeof(*args));

    NEXTARG;
    if (strcmp(*argv, "l") == 0) {
        args->cmd = CMD_ISLOOP;

        NEXTARG;
        if (inet_pton(AF_INET, *argv, &args->addr) <= 0) {
            fprintf(stderr, "invalid addr\n");
            exit(1);
        }
    } else if (strcmp(*argv, "m") == 0) {
        args->cmd = CMD_ISMULT;

        NEXTARG;
        if (inet_pton(AF_INET, *argv, &args->addr) <= 0) {
            fprintf(stderr, "invalid addr\n");
            exit(1);
        }
    } else if (strcmp(*argv, "c") == 0
            || strcmp(*argv, "b") == 0
            || strcmp(*argv, "n") == 0) {

        if (strcmp(*argv, "c") == 0)
            args->cmd = CMD_COMP;
        else if (strcmp(*argv, "b") == 0)
            args->cmd = CMD_ISBORAD;
        else if (strcmp(*argv, "n") == 0)
            args->cmd = CMD_ISNET;

        NEXTARG;
        if (inet_pton(AF_INET, *argv, &args->addr) <= 0) {
            fprintf(stderr, "invalid addr\n");
            exit(1);
        }

        NEXTARG;
        if (inet_pton(AF_INET, *argv, &args->net) <= 0) {
            fprintf(stderr, "invalid addr2/net\n");
            exit(1);
        }

        NEXTARG;
        if (inet_pton(AF_INET, *argv, &args->mask) <= 0
                || !IPv4Addr_IsNetmaskValid(InaddrGetU32(&args->mask))) {
            fprintf(stderr, "invalid mask\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "unknow command `%s'\n", *argv);
        exit(1);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    struct arguments args;

    if (ParseArgs(argc, argv, &args) != 0)
        exit(1);

    switch (args.cmd) {
    case CMD_ISLOOP:
        if (IPv4Addr_IsLoopback(InaddrGetU32(&args.addr)))
            AnscTraceFlow(("Is loopback\n"));
        else
            AnscTraceFlow(("Not loopback\n"));
        break;

    case CMD_ISMULT:
        if (IPv4Addr_IsMulticast(InaddrGetU32(&args.addr)))
            AnscTraceFlow(("Is multicast\n"));
        else
            printf("Not multicast\n");
        break;

    case CMD_COMP:
        if (IPv4Addr_IsSameNetwork(InaddrGetU32(&args.addr), 
                    InaddrGetU32(&args.net), InaddrGetU32(&args.mask)))
            printf("Is in same network\n");
        else
            printf("Not in same network\n");
        break;

    case CMD_ISBORAD:
        if (IPv4Addr_IsBroadcast(InaddrGetU32(&args.addr), 
                    InaddrGetU32(&args.net), InaddrGetU32(&args.mask)))
            printf("Is broadcast\n");
        else
            printf("Not broadcast\n");
        break;

    case CMD_ISNET:
        if (IPv4Addr_IsNetworkAddr(InaddrGetU32(&args.addr), 
                    InaddrGetU32(&args.net), InaddrGetU32(&args.mask)))
            printf("Is network address\n");
        else
            printf("Not network address\n");
        break;

    default:
        Usage();
        exit(1);
    }

    return 0;
}

#endif
