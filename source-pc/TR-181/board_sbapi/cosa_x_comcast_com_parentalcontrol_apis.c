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

#include "cosa_x_comcast_com_parentalcontrol_apis.h"

#ifdef _COSA_SIM_

ANSC_STATUS
CosaDmlParentalControlInit(ANSC_HANDLE hDml, PANSC_HANDLE phContext)
{
    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedSites
 */
static COSA_DML_MANAGEDSITES g_ManagedSites = {
    .Enable             = FALSE,
};

ANSC_STATUS
CosaDmlMngSites_GetConf(COSA_DML_MANAGEDSITES *conf)
{
    memcpy(conf, &g_ManagedSites, sizeof(COSA_DML_MANAGEDSITES));
    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedServices
 */
static COSA_DML_MANAGED_SERVS g_ManagedServs = {
    .Enable             = FALSE,
};

ANSC_STATUS
CosaDmlMngServs_GetConf(COSA_DML_MANAGED_SERVS *conf)
{
    memcpy(conf, &g_ManagedServs, sizeof(COSA_DML_MANAGED_SERVS));
    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedDevices
 */
static COSA_DML_MANAGED_DEVS g_ManagedDevs = {
    .Enable             = FALSE,
    .AllowAll           = FALSE,
};


ANSC_STATUS
CosaDmlMngDevs_GetConf(COSA_DML_MANAGED_DEVS *conf)
{
    memcpy(conf, &g_ManagedDevs, sizeof(COSA_DML_MANAGED_DEVS));
    return ANSC_STATUS_SUCCESS;
}

/*
 * TrustedUser
 */

#define  MAX_TRUSTEDUSER_ENTRY      1024

static int g_NrTrustedUser  = 0; 

static COSA_DML_TRUSTEDUSER g_TrustedUsers[MAX_TRUSTEDUSER_ENTRY] = {
    {
        .InstanceNumber = 1,
        .Alias          = "cpe-TrustedUser-1",
        .HostDescription = "Host-1",
        .IPAddressType  = IPADDR_IPV4,
        .IPAddress      = "192.168.0.1",
    },
    {
        .InstanceNumber = 2,
        .Alias          = "cpe-TrustedUser-2",
        .HostDescription = "Host-2",
        .IPAddressType  = IPADDR_IPV6,
        .IPAddress      = "2012:CAFE::1",
    },
};
#define  MAX_MSTRUSTEDUSER_ENTRY      4

static int g_NrMSTrustedUsers   = 0;

static COSA_DML_MS_TRUSTEDUSER g_MSTrustedUsers[MAX_MSTRUSTEDUSER_ENTRY] = {
    {
        .InstanceNumber = 1,
        .Alias          = "cpe-MSTrustedUser-1",
        .HostDescription = "Dummy",
        .IPAddressType  = IPADDR_IPV6,
        .IPAddress      = "2012:cafe::3",
        .Trusted        = FALSE,
    },
    {
        .InstanceNumber = 2,
        .Alias          = "cpe-MSTrustedUser-2",
        .HostDescription = "Dummy",
        .IPAddressType  = IPADDR_IPV4,
        .IPAddress      = "192.168.0.4",
        .Trusted        = TRUE,
    },
};


/*
 * Blocked URL
 */

#define  MAX_URL_ENTRY      1024

static int g_NrBlkURL   = 0;

static COSA_DML_BLOCKEDURL g_BlockedURLs[MAX_URL_ENTRY] = {
    {
        .InstanceNumber = 1,
        .Alias          = "cpe-BlockedSite-1",
        .BlockMethod    = BLOCK_METHOD_URL,
        .Site           = "http://baidu.com",
        .AlwaysBlock    = FALSE,
        .StartTime      = "19:00",
        .EndTime        = "22:00",
        .BlockDays      = "Sat,Sun",
    },
    {
        .InstanceNumber = 2,
        .Alias          = "cpe-BlockedSite-2",
        .BlockMethod    = BLOCK_METHOD_KEYWORD,
        .Site           = "keyword",
        .AlwaysBlock    = FALSE,
        .StartTime      = "10:00",
        .EndTime        = "12:00",
        .BlockDays      = "Mon,Wed,Fri",
    },
};

ANSC_STATUS
CosaDmlMngSites_SetConf(COSA_DML_MANAGEDSITES *conf)
{
    int i;
    memcpy(&g_ManagedSites, conf, sizeof(COSA_DML_MANAGEDSITES));
    if(conf != NULL)
    {
        if(!conf->Enable)
        {
		do_parentalControl_Delrule_Sites(); //LNT_EMU
        }
        else
        {
                do_parentalControl_Addrule_Sites(); //LNT_EMU
		for(i=0;i<g_NrBlkURL;i++)
		{	    	
    			do_parentalControl_Sites(ADD,&g_BlockedURLs[i]);
		}
		for(i=0;i<g_NrTrustedUser;i++)
		{	    
		  CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,g_TrustedUsers[i].IPAddress,ADD);
		}	
        }
   }
   return ANSC_STATUS_SUCCESS;
}

static int
BlkURL_InsGetIndex(ULONG ins)
{
    int i;

    for (i = 0; i < g_NrBlkURL; i++)
        if (g_BlockedURLs[i].InstanceNumber == ins)
            return i;

    return -1;
}

ULONG
CosaDmlBlkURL_GetNumberOfEntries(void)
{
    return g_NrBlkURL;
}

ANSC_STATUS
CosaDmlBlkURL_GetEntryByIndex(ULONG index, COSA_DML_BLOCKEDURL *pEntry)
{
    if (index >= g_NrBlkURL)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_BlockedURLs[index];
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_SetValues(ULONG index, ULONG ins, const char *alias)
{
    if (index >= g_NrBlkURL)
        return ANSC_STATUS_FAILURE;

    g_BlockedURLs[index].InstanceNumber = ins;
    snprintf(g_BlockedURLs[index].Alias, sizeof(g_BlockedURLs[index].Alias),
            "%s", alias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_AddEntry(COSA_DML_BLOCKEDURL *pEntry)
{
    if (g_NrBlkURL >= MAX_URL_ENTRY)
        return ANSC_STATUS_FAILURE;

    g_BlockedURLs[g_NrBlkURL++] = *pEntry;
    do_parentalControl_Sites(ADD,&g_BlockedURLs[g_NrBlkURL-1]); //LNT_EMU
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_DelEntry(ULONG ins)
{
    int i;

    if ((i = BlkURL_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    do_parentalControl_Sites(DELETE,&g_BlockedURLs[i]); //LNT_EMU
    memmove(&g_BlockedURLs[i], &g_BlockedURLs[i+1], 
            (g_NrBlkURL - i - 1) * sizeof(COSA_DML_BLOCKEDURL));
    g_NrBlkURL--;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_GetConf(ULONG ins, COSA_DML_BLOCKEDURL *pEntry)
{
    int i;

    if ((i = BlkURL_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_BlockedURLs[i];
    pEntry->InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlBlkURL_SetConf(ULONG ins, COSA_DML_BLOCKEDURL *pEntry)
{
    int i;

    if ((i = BlkURL_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;
    do_parentalControl_Sites(DELETE,&g_BlockedURLs[i]); //LNT_EMU
    g_BlockedURLs[i] = *pEntry;
    g_BlockedURLs[i].InstanceNumber = ins; /* just in case */
    do_parentalControl_Sites(ADD,&g_BlockedURLs[i]);    //LNT_EMU

    return ANSC_STATUS_SUCCESS;
}

static int
TrustedUser_InsGetIndex(ULONG ins)
{
    int i;

    for (i = 0; i < g_NrTrustedUser; i++)
        if (g_TrustedUsers[i].InstanceNumber == ins)
            return i;

    return -1;
}

ULONG
CosaDmlTrustedUser_GetNumberOfEntries(void)
{
    return g_NrTrustedUser;
}

ANSC_STATUS
CosaDmlTrustedUser_GetEntryByIndex(ULONG index, COSA_DML_TRUSTEDUSER *pEntry)
{
    if (index >= g_NrTrustedUser)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_TrustedUsers[index];
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_SetValues(ULONG index, ULONG ins, const char *alias)
{
    if (index >= g_NrTrustedUser)
        return ANSC_STATUS_FAILURE;

    g_TrustedUsers[index].InstanceNumber = ins;
    snprintf(g_TrustedUsers[index].Alias, sizeof(g_TrustedUsers[index].Alias),
            "%s", alias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_AddEntry(COSA_DML_TRUSTEDUSER *pEntry)
{
    if (g_NrTrustedUser >= MAX_TRUSTEDUSER_ENTRY)
        return ANSC_STATUS_FAILURE;

    g_TrustedUsers[g_NrTrustedUser++] = *pEntry;
    CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,g_TrustedUsers[g_NrTrustedUser-1].IPAddress,ADD);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_DelEntry(ULONG ins)
{
    int i;

    if ((i = TrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;
    CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,g_TrustedUsers[i].IPAddress,DELETE);
    memmove(&g_TrustedUsers[i], &g_TrustedUsers[i+1], 
            (g_NrTrustedUser - i - 1) * sizeof(COSA_DML_TRUSTEDUSER));
    g_NrTrustedUser--;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_GetConf(ULONG ins, COSA_DML_TRUSTEDUSER *pEntry)
{
    int i;

    if ((i = TrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_TrustedUsers[i];
    pEntry->InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlTrustedUser_SetConf(ULONG ins, COSA_DML_TRUSTEDUSER *pEntry)
{
    int i;
    if ((i = TrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    g_TrustedUsers[i] = *pEntry;
    g_TrustedUsers[i].InstanceNumber = ins; /* just in case */
    if(pEntry->Trusted)
        CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,g_TrustedUsers[i].IPAddress,ADD); //LNT_EMU
    else
        CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,g_TrustedUsers[i].IPAddress,DELETE); //LNT_EMU
    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedServices.Service
 */

#define  MAX_MSSERV_ENTRY      1024

static int g_NrMSServs   = 0;

static COSA_DML_MS_SERV g_MSServs[MAX_MSSERV_ENTRY] = {
    {
        .InstanceNumber = 1,
        .Alias          = "cpe-MSService-1",
        .Description    = "Service-1",
        .Protocol       = PROTO_TCP,
        .StartPort      = 100,
        .EndPort        = 1024,
        .AlwaysBlock   = FALSE,
        .StartTime      = "19:00",
        .EndTime        = "22:00",
        .BlockDays      = "Sat,Sun",
    },
    {
        .InstanceNumber = 2,
        .Alias          = "cpe-MSService-2",
        .Description    = "Service-2",
        .Protocol       = PROTO_BOTH,
        .StartPort      = 200,
        .EndPort        = 2024,
        .AlwaysBlock   = FALSE,
        .StartTime      = "20:00",
        .EndTime        = "24:00",
        .BlockDays      = "Mon,Tue",
    },
};

ANSC_STATUS
CosaDmlMngServs_SetConf(COSA_DML_MANAGED_SERVS *conf)
{
    int i;
    memcpy(&g_ManagedServs, conf, sizeof(COSA_DML_MANAGED_SERVS));
    if(!conf->Enable)
    {
	    do_parentalControl_Delrule_Services(); //LNT_EMU
    }
    else
    {
            do_parentalControl_Addrule_Services();
	    for(i = 0;i < g_NrMSServs; i++)
	    { 	
		    do_parentalControl_Services(ADD,&g_MSServs[i]); //LNT_EMU
	    }	
	   for(i=0;i<g_NrMSTrustedUsers;i++)
	   {	    
		CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,g_MSTrustedUsers[i].IPAddress,ADD);	   
	   }	
    }

   return ANSC_STATUS_SUCCESS;
}

static int
MSServ_InsGetIndex(ULONG ins)
{
    int i;

    for (i = 0; i < g_NrMSServs; i++)
        if (g_MSServs[i].InstanceNumber == ins)
            return i;

    return -1;
}

ULONG
CosaDmlMSServ_GetNumberOfEntries(void)
{
    return g_NrMSServs;
}

ANSC_STATUS
CosaDmlMSServ_GetEntryByIndex(ULONG index, COSA_DML_MS_SERV *pEntry)
{
    if (index >= g_NrMSServs)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_MSServs[index];
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_SetValues(ULONG index, ULONG ins, const char *alias)
{
    if (index >= g_NrMSServs)
        return ANSC_STATUS_FAILURE;

    g_MSServs[index].InstanceNumber = ins;
    snprintf(g_MSServs[index].Alias, sizeof(g_MSServs[index].Alias),
            "%s", alias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_AddEntry(COSA_DML_MS_SERV *pEntry)
{
    if (g_NrMSServs >= MAX_MSSERV_ENTRY)
        return ANSC_STATUS_FAILURE;

    g_MSServs[g_NrMSServs++] = *pEntry;
    do_parentalControl_Services(ADD,&g_MSServs[g_NrMSServs-1]); //LNT_EMU
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_DelEntry(ULONG ins)
{
    int i;

    if ((i = MSServ_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    do_parentalControl_Services(DELETE,&g_MSServs[i]); //LNT_EMU
    memmove(&g_MSServs[i], &g_MSServs[i+1], 
            (g_NrMSServs - i - 1) * sizeof(COSA_DML_MS_SERV));
    g_NrMSServs--;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_GetConf(ULONG ins, COSA_DML_MS_SERV *pEntry)
{
    int i;

    if ((i = MSServ_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_MSServs[i];
    pEntry->InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMSServ_SetConf(ULONG ins, COSA_DML_MS_SERV *pEntry)
{
    int i;

    if ((i = MSServ_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    do_parentalControl_Services(DELETE,&g_MSServs[i]); //LNT_EMU
    g_MSServs[i] = *pEntry;
    g_MSServs[i].InstanceNumber = ins; /* just in case */
    do_parentalControl_Services(ADD,&g_MSServs[i]);    //LNT_EMU

    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedServices.TrustedUser
 */

static int
MSTrustedUser_InsGetIndex(ULONG ins)
{
    int i;

    for (i = 0; i < g_NrMSTrustedUsers; i++)
        if (g_MSTrustedUsers[i].InstanceNumber == ins)
            return i;

    return -1;
}

ULONG
CosaDmlMSTrustedUser_GetNumberOfEntries(void)
{
    return g_NrMSTrustedUsers;
}

ANSC_STATUS
CosaDmlMSTrustedUser_GetEntryByIndex(ULONG index, COSA_DML_MS_TRUSTEDUSER *pEntry)
{
    if (index >= g_NrMSTrustedUsers)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_MSTrustedUsers[index];
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_SetValues(ULONG index, ULONG ins, const char *alias)
{
    if (index >= g_NrMSTrustedUsers)
        return ANSC_STATUS_FAILURE;

    g_MSTrustedUsers[index].InstanceNumber = ins;
    snprintf(g_MSTrustedUsers[index].Alias, sizeof(g_MSTrustedUsers[index].Alias),
            "%s", alias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_AddEntry(COSA_DML_MS_TRUSTEDUSER *pEntry)
{
    if (g_NrMSTrustedUsers >= MAX_MSTRUSTEDUSER_ENTRY)
        return ANSC_STATUS_FAILURE;

    g_MSTrustedUsers[g_NrMSTrustedUsers++] = *pEntry;
    CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,g_MSTrustedUsers[g_NrMSTrustedUsers-1].IPAddress,ADD);  //LNT_EMU

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_DelEntry(ULONG ins)
{
    int i;

    if ((i = MSTrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;
   
    CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,g_MSTrustedUsers[i].IPAddress,DELETE);   //LNT_EMU
    memmove(&g_MSTrustedUsers[i], &g_MSTrustedUsers[i+1], 
            (g_NrMSTrustedUsers - i - 1) * sizeof(COSA_DML_MS_TRUSTEDUSER));
    g_NrMSTrustedUsers--;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_GetConf(ULONG ins, COSA_DML_MS_TRUSTEDUSER *pEntry)
{
    int i;

    if ((i = MSTrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_MSTrustedUsers[i];
    pEntry->InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMSTrustedUser_SetConf(ULONG ins, COSA_DML_MS_TRUSTEDUSER *pEntry)
{
    int i;

    if ((i = MSTrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    g_MSTrustedUsers[i] = *pEntry;
    g_MSTrustedUsers[i].InstanceNumber = ins; /* just in case */
    if(pEntry->Trusted)
        CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,g_MSTrustedUsers[i].IPAddress,ADD); // LNT_EMU
    else
        CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,g_MSTrustedUsers[i].IPAddress,DELETE); //LNT_EMU

    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedDevices.Device
 */

#define  MAX_MDDEV_ENTRY      4

static int g_NrMDDevs   = 0;
static int AllowAll = MD_TYPE_BLOCK;
static int Type = MD_TYPE_BLOCK;
static COSA_DML_MD_DEV g_MDDevs[MAX_MDDEV_ENTRY] = {
    {
        .InstanceNumber = 1,
        .Alias          = "cpe-MDDevice-1",
        .Type           = MD_TYPE_ALLOW,
        .Description    = "dummy-1",
        .MACAddress     = "00:1A:2B:AA:BB:CC",
        .AlwaysBlock   = FALSE,
        .StartTime      = "19:00",
        .EndTime        = "22:00",
        .BlockDays      = "Sat,Sun",
    },
    {
        .InstanceNumber = 2,
        .Type           = MD_TYPE_BLOCK,
        .Alias          = "cpe-MDDevice-2",
        .Description    = "dummy-2",
        .MACAddress     = "00:1A:2B:11:22:33",
        .AlwaysBlock   = TRUE,
        .StartTime      = "20:00",
        .EndTime        = "24:00",
        .BlockDays      = "Mon,Tue",
    },
};

ANSC_STATUS
CosaDmlMngDevs_SetConf(COSA_DML_MANAGED_DEVS *conf)
{
    int i;
    memcpy(&g_ManagedDevs, conf, sizeof(COSA_DML_MANAGED_DEVS));
    if(!conf->Enable)
    {
	     do_parentalControl_Delrule_Devices();   //LNT_EMU
    }
   else
    {
	    if(conf->AllowAll)
            {
			Type = MD_TYPE_BLOCK;
	    }
	    else
	    {	
			Type = MD_TYPE_ALLOW;
	    }
	     if( AllowAll != Type)
	     {
	     	     do_parentalControl_Delrule_Devices();   //LNT_EMU
	     }		
	     do_parentalControl_Addrule_Devices();
	     for( i=0; i < g_NrMDDevs; i++)
	     {
 
		      if(g_MDDevs[i].Type == Type)
		      {		
		    	      do_parentalControl_Devices(ADD,&g_MDDevs[i]);
		      }	
	     }
     	     AllowAll = Type;

    }

    return ANSC_STATUS_SUCCESS;
}

static int
MDDev_InsGetIndex(ULONG ins)
{
    int i;

    for (i = 0; i < g_NrMDDevs; i++)
        if (g_MDDevs[i].InstanceNumber == ins)
            return i;

    return -1;
}

ULONG
CosaDmlMDDev_GetNumberOfEntries(void)
{
    return g_NrMDDevs;
}

ANSC_STATUS
CosaDmlMDDev_GetEntryByIndex(ULONG index, COSA_DML_MD_DEV *pEntry)
{
    if (index >= g_NrMDDevs)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_MDDevs[index];
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_SetValues(ULONG index, ULONG ins, const char *alias)
{
    if (index >= g_NrMDDevs)
        return ANSC_STATUS_FAILURE;

    g_MDDevs[index].InstanceNumber = ins;
    snprintf(g_MDDevs[index].Alias, sizeof(g_MDDevs[index].Alias),
            "%s", alias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_AddEntry(COSA_DML_MD_DEV *pEntry)
{
    if (g_NrMDDevs >= MAX_MDDEV_ENTRY)
        return ANSC_STATUS_FAILURE;

    g_MDDevs[g_NrMDDevs++] = *pEntry;
    do_parentalControl_Devices(ADD,&g_MDDevs[g_NrMDDevs-1]); //LNT_EMU
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_DelEntry(ULONG ins)
{
    int i;

    if ((i = MDDev_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    do_parentalControl_Devices(DELETE,&g_MDDevs[i]); //LNT_EMU

    memmove(&g_MDDevs[i], &g_MDDevs[i+1], 
            (g_NrMDDevs - i - 1) * sizeof(COSA_DML_MD_DEV));
    g_NrMDDevs--;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_GetConf(ULONG ins, COSA_DML_MD_DEV *pEntry)
{
    int i;

    if ((i = MDDev_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_MDDevs[i];
    pEntry->InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMDDev_SetConf(ULONG ins, COSA_DML_MD_DEV *pEntry)
{
    int i;

    if ((i = MDDev_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;
    do_parentalControl_Devices(DELETE,&g_MDDevs[i]);  //LNT_EMU

    g_MDDevs[i] = *pEntry;
    g_MDDevs[i].InstanceNumber = ins; /* just in case */
    do_parentalControl_Devices(ADD,&g_MDDevs[i]);     //LNT_EMU

    return ANSC_STATUS_SUCCESS;
}

#endif /* _COSA_SIM_ */
