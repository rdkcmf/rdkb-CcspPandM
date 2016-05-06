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
#if 1
static BOOL DEV_ENABLE = false;
static BOOL DEV_ALLOWALL = false;
static int forwardCounter = 1;
static int inputCounter = 1;

struct  InputRuleMap
{
	LONG ins;
	int Type;
	LONG ruleNumber;

} INPUT_RULEMAP[1024];

struct  forwardRuleMap
{
	LONG ins;
	int Type;
	LONG ruleNumber;

} FORWARD_RULEMAP[1024];
#endif
#ifdef _COSA_SIM_
#if 1
void CosaDmlTrustedUser_Accept(int block_type,int i,int Operation);

void printForwardTable(int start)
{
	int count;
	for(count=start; count<forwardCounter; count++)
	{
		 printf("\n%ld	%d	%ld\n",FORWARD_RULEMAP[count].ins, FORWARD_RULEMAP[count].Type,FORWARD_RULEMAP[count].ruleNumber); 
	}
}	
	
void printInputTable(int start)
{
	int count;
	for(count=start; count<inputCounter; count++)
	{
		 printf("\n%ld	%d	%ld\n",INPUT_RULEMAP[count].ins, INPUT_RULEMAP[count].Type,INPUT_RULEMAP[count].ruleNumber); 
	
	}	
}
void insertForwardTable(LONG ins, int type)
{
	int count;
	for(count=forwardCounter+1; count>1; count--)
	{
		FORWARD_RULEMAP[count].ins = FORWARD_RULEMAP[count-1].ins;
		FORWARD_RULEMAP[count].ruleNumber = FORWARD_RULEMAP[count-1].ruleNumber +1;
		FORWARD_RULEMAP[count].Type = FORWARD_RULEMAP[count-1].Type;
	}
	FORWARD_RULEMAP[1].ins = ins;
	FORWARD_RULEMAP[1].ruleNumber = 1;
	FORWARD_RULEMAP[1].Type = type;
	forwardCounter++;
}

void forwardTableDelete(int start)
{
	int count;
	for(count=start; count<forwardCounter; count++)
	{
		FORWARD_RULEMAP[count].ins = FORWARD_RULEMAP[count+1].ins;
		FORWARD_RULEMAP[count].ruleNumber = FORWARD_RULEMAP[count+1].ruleNumber -1;
		FORWARD_RULEMAP[count].Type = FORWARD_RULEMAP[count+1].Type;
	}
	forwardCounter--;
}

LONG getForwardRuleNumber(int type, LONG instanceNumber)
{
	int i;
	for(i=1;i<forwardCounter;i++)
		if((FORWARD_RULEMAP[i].Type == type) && (FORWARD_RULEMAP[i].ins == instanceNumber))	
			return FORWARD_RULEMAP[i].ruleNumber;	

}

LONG getInputRuleNumber(int type, LONG instanceNumber)
{
	int i;
	for(i=1;i<inputCounter;i++)
		if((INPUT_RULEMAP[i].Type == type) && (INPUT_RULEMAP[i].ins == instanceNumber))	
			return INPUT_RULEMAP[i].ruleNumber;	

}

void inputTableDelete(int start)
{
	int count;
	for(count=start; count<inputCounter; count++)
	{
		INPUT_RULEMAP[count].ins = INPUT_RULEMAP[count+1].ins;
		INPUT_RULEMAP[count].ruleNumber = INPUT_RULEMAP[count+1].ruleNumber-1;
		INPUT_RULEMAP[count].Type = INPUT_RULEMAP[count+1].Type;
	}
	inputCounter--;
}
#endif
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
#if 0
ANSC_STATUS
CosaDmlMngSites_SetConf(COSA_DML_MANAGEDSITES *conf)
{
    memcpy(&g_ManagedSites, conf, sizeof(COSA_DML_MANAGEDSITES));
    return ANSC_STATUS_SUCCESS;
}
#endif
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
#if 0
ANSC_STATUS
CosaDmlMngServs_SetConf(COSA_DML_MANAGED_SERVS *conf)
{
    memcpy(&g_ManagedServs, conf, sizeof(COSA_DML_MANAGED_SERVS));
    return ANSC_STATUS_SUCCESS;
}
#endif

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
#if 0
ANSC_STATUS
CosaDmlMngDevs_SetConf(COSA_DML_MANAGED_DEVS *conf)
{
    memcpy(&g_ManagedDevs, conf, sizeof(COSA_DML_MANAGED_DEVS));
    return ANSC_STATUS_SUCCESS;
}
#endif
/*
 * Blocked URL
 */
#if 0
#define  MAX_URL_ENTRY      4
#endif
#if 1
#define  MAX_URL_ENTRY      1024
#endif
#if 0
static int g_NrBlkURL   = MAX_URL_ENTRY / 2;
#endif
#if 1
static int g_NrBlkURL   = 0;
#endif

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
#if 1
void BlkURL_UpdateConfigFiles(int OPERATION, ULONG ins)
{
	int i,len;
	char newvar[1024];
	char cmd[1024]= {'\0'};
	char protocol1[10];
	char url[1024];
	const char s[] = "://";
	char *token;
	char action1[15];
	char strWord[1024];	
	LONG ruleNumber;
	switch(OPERATION)
	{
		case ADD:
			sprintf(newvar,"iptables  -t filter -I FORWARD %ld ",forwardCounter);
			break;
		case ENABLE:
		case DISABLE:
		case REPLACE:
			ruleNumber = getForwardRuleNumber(SITE_TYPE,g_BlockedURLs[ins].InstanceNumber);	
			sprintf(newvar,"iptables -t filter -R FORWARD %ld ",ruleNumber);
			break;
		case DELETE:
			ruleNumber = getForwardRuleNumber(SITE_TYPE,g_BlockedURLs[ins].InstanceNumber);	
			sprintf(newvar,"iptables -t filter -D FORWARD %ld ",ruleNumber);
			break;
	}
	if(OPERATION == DISABLE)
	{
		sprintf(action1," ACCEPT");
	}
	else
	{
		sprintf(action1," DROP");
	}
	if(g_BlockedURLs[ins].BlockMethod == BLOCK_METHOD_URL)
	{
		token = strtok(g_BlockedURLs[ins].Site,s);
		len = strlen(token);
		strcpy(strWord,g_BlockedURLs[ins].Site+len+3);
	}
	else if(g_BlockedURLs[ins].BlockMethod == BLOCK_METHOD_KEYWORD)
	{ 
		strcpy(strWord,g_BlockedURLs[ins].Site);
	}

	if(OPERATION == DELETE)
	{
		system(newvar);
		forwardTableDelete(ruleNumber);
		printForwardTable(1);
	}
	else
	{
		if( !g_BlockedURLs[ins].AlwaysBlock )
		{
			snprintf(cmd,sizeof(cmd),"%s  -m string --algo bm --string %s -m time --timestart %s --timestop %s --weekdays %s -j %s",newvar,strWord,g_BlockedURLs[ins].StartTime,g_BlockedURLs[ins].EndTime,g_BlockedURLs[ins].BlockDays,action1);
		}
		else
		{
			snprintf(cmd,sizeof(cmd),"%s  -m string --algo bm --string %s -j %s",newvar,strWord,action1);
		}
		system(cmd);
	}
	if(OPERATION == ADD)
	{
		FORWARD_RULEMAP[forwardCounter].ins =g_BlockedURLs[ins].InstanceNumber;
		FORWARD_RULEMAP[forwardCounter].ruleNumber = forwardCounter;
		FORWARD_RULEMAP[forwardCounter].Type = SITE_TYPE;
		forwardCounter++;
		printForwardTable(1);
	}

}

ANSC_STATUS
CosaDmlMngSites_SetConf(COSA_DML_MANAGEDSITES *conf)
{
	int i;
	memcpy(&g_ManagedSites, conf, sizeof(COSA_DML_MANAGEDSITES));
	for(i=0;i<g_NrBlkURL;i++)
	{
		if(conf != NULL)
		{
			if(!conf->Enable)
			{

				BlkURL_UpdateConfigFiles(DISABLE,i);
			}
			else
			{
				BlkURL_UpdateConfigFiles(ENABLE,i);
			}
		}
	}

	return ANSC_STATUS_SUCCESS;
}
#endif
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
#if 1
	BlkURL_UpdateConfigFiles(ADD,g_NrBlkURL-1);
#endif
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_DelEntry(ULONG ins)
{
    int i;

    if ((i = BlkURL_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;
#if 1
	BlkURL_UpdateConfigFiles(DELETE,i);
#endif
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

	g_BlockedURLs[i] = *pEntry;
	g_BlockedURLs[i].InstanceNumber = ins; /* just in case */
#if 1
	BlkURL_UpdateConfigFiles(REPLACE,i);
#endif

    return ANSC_STATUS_SUCCESS;
}

/*
 * TrustedUser
 */
#if 0
#define  MAX_TRUSTEDUSER_ENTRY      4
#endif
#if 1
#define  MAX_TRUSTEDUSER_ENTRY      100
#endif
#if 0
static int g_NrTrustedUser   = MAX_TRUSTEDUSER_ENTRY / 2;
#endif
#if 1
static int g_NrTrustedUser   = 0;
#endif

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
#if 1
	CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,g_NrTrustedUser-1,ADD);
#endif
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_DelEntry(ULONG ins)
{
    int i;

    if ((i = TrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

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
#if 1
	if(pEntry->Trusted)
		CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,i,ADD);
	else
		CosaDmlTrustedUser_Accept(TRUSTEDSITE_TYPE,i,DELETE);
#endif
	return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedServices.Service
 */
#if 0
#define  MAX_MSSERV_ENTRY      4
#endif
#if 1
#define  MAX_MSSERV_ENTRY      1024
#endif
#if 0
static int g_NrMSServs   = MAX_MSSERV_ENTRY / 2;
#endif
#if 1
static int g_NrMSServs   = 0;
#endif

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
#if 1
void UpdateCosaDmlMSServ_Protocol(int OPERATION,ULONG ins)
{
	int i;
	char newvar[1024];
	char action[10];
	ULONG startport;
	char cmd[1024]= {'\0'};
	char protocol[10];
	char protocol1[10];
	LONG ruleNumber;
	switch(OPERATION)
	{
		case ADD:
			sprintf(newvar,"iptables -I FORWARD %ld ",inputCounter);
			break;
		case REPLACE:
		case ENABLE:
		case DISABLE:
			ruleNumber = getInputRuleNumber(SERVICE_TYPE,g_MSServs[ins].InstanceNumber);	
			sprintf(newvar,"iptables -R FORWARD %ld ",ruleNumber);
			break;
		case DELETE:
			ruleNumber = getInputRuleNumber(SERVICE_TYPE,g_MSServs[ins].InstanceNumber);	
			sprintf(newvar,"iptables -D FORWARD %ld ",ruleNumber);
			break;
	}
	switch(g_MSServs[ins].Protocol)
	{
		case PROTO_TCP:
			strcpy(protocol,"tcp");
			break;
		case PROTO_UDP:
			strcpy(protocol,"udp");
			break;
		case PROTO_BOTH:
			strcpy(protocol,"tcp");
			strcpy(protocol1,"udp");
			break;
	}
	if(OPERATION == DISABLE)
	{
		sprintf(action," ACCEPT");
	}
	else
	{
		sprintf(action," DROP");
	}

	if(OPERATION == DELETE)
	{
		system(newvar);
		inputTableDelete(ruleNumber);
	}
	else 
	{
		if(g_MSServs[ins].StartPort == g_MSServs[ins].EndPort)
		{
			if(!g_MSServs[ins].AlwaysBlock)
			{
				if(g_MSServs[ins].Protocol != PROTO_BOTH)
				{
					snprintf(cmd,sizeof(cmd),"%s -p %s  --destination-port %ld -m time --timestart %s --timestop %s --weekdays %s -j %s",newvar,protocol,g_MSServs[ins].StartPort,g_MSServs[ins].StartTime,g_MSServs[ins].EndTime,g_MSServs[ins].BlockDays,action);
					system(cmd);
				}
				else
				{
					snprintf(cmd,sizeof(cmd),"%s -p %s --destination-port %ld -m time --timestart %s --timestop %s --weekdays %s -j %s",newvar,protocol,g_MSServs[ins].StartPort,g_MSServs[ins].StartTime,g_MSServs[ins].EndTime,g_MSServs[ins].BlockDays,action);
					system(cmd);
					snprintf(cmd,sizeof(cmd),"%s -p %s --destination-port %ld -m time --timestart %s --timestop %s --weekdays %s-j %s",newvar,protocol1,g_MSServs[ins].StartPort,g_MSServs[ins].StartTime,g_MSServs[ins].EndTime,g_MSServs[ins].BlockDays,action);
					system(cmd);

				}
			}
			else
			{
				if(g_MSServs[ins].Protocol != PROTO_BOTH)
				{
					snprintf(cmd,sizeof(cmd),"%s -p %s  --destination-port %ld  -j %s",newvar,protocol,g_MSServs[ins].StartPort,action);
					system(cmd);
				}
				else
				{
					snprintf(cmd,sizeof(cmd),"%s -p %s --destination-port %ld  -j %s",newvar,protocol,g_MSServs[ins].StartPort,action);
					system(cmd);
					snprintf(cmd,sizeof(cmd),"%s -p %s  --destination-port %ld  -j %s",newvar,protocol1,g_MSServs[ins].StartPort,action);
					system(cmd);

				}

			}
		}
		else
		{
			if(!g_MSServs[ins].AlwaysBlock)
			{
				if(g_MSServs[ins].Protocol != PROTO_BOTH)
				{
					snprintf(cmd,sizeof(cmd),"%s -p %s -m multiport --dports %ld:%ld -m time --timestart %s --timestop %s --weekdays %s -j %s",newvar,protocol,g_MSServs[ins].StartPort,g_MSServs[ins].EndPort,g_MSServs[ins].StartTime,g_MSServs[ins].EndTime,g_MSServs[ins].BlockDays,action);
					system(cmd);
				}
				else
				{
					snprintf(cmd,sizeof(cmd),"%s -p %s -m multiport --dports %ld:%ld -m time --timestart %s --timestop %s --weekdays %s -j %s",newvar,protocol,g_MSServs[ins].StartPort,g_MSServs[ins].EndPort,g_MSServs[ins].StartTime,g_MSServs[ins].EndTime,g_MSServs[ins].BlockDays,action);
					system(cmd);
					snprintf(cmd,sizeof(cmd),"%s -p %s -m multiport --dports %ld:%ld -m time --timestart %s --timestop %s --weekdays %s-j %s",newvar,protocol1,g_MSServs[ins].StartPort,g_MSServs[ins].EndPort,g_MSServs[ins].StartTime,g_MSServs[ins].EndTime,g_MSServs[ins].BlockDays,action);
					system(cmd);

				}	
			}
			else
			{
				if(g_MSServs[ins].Protocol != PROTO_BOTH)
				{
					snprintf(cmd,sizeof(cmd),"%s -p %s -m multiport --dports %ld:%ld  -j %s",newvar,protocol,g_MSServs[ins].StartPort,g_MSServs[ins].EndPort,action);
					system(cmd);
				}
				else
				{
					snprintf(cmd,sizeof(cmd),"%s -p %s -m multiport --dports %ld:%ld  -j %s",newvar,protocol,g_MSServs[ins].StartPort,g_MSServs[ins].EndPort,action);
					system(cmd);
					snprintf(cmd,sizeof(cmd),"%s -p %s -m multiport --dports %ld:%ld  -j %s",newvar,protocol1,g_MSServs[ins].StartPort,g_MSServs[ins].EndPort,action);
					system(cmd);

				}
			}

		}
	}
	if(OPERATION == ADD)
	{
		INPUT_RULEMAP[inputCounter].ins = g_MSServs[ins].InstanceNumber;
		INPUT_RULEMAP[inputCounter].ruleNumber = inputCounter;
		INPUT_RULEMAP[inputCounter].Type = SERVICE_TYPE;
		inputCounter++;
	}

}
ANSC_STATUS
CosaDmlMngServs_SetConf(COSA_DML_MANAGED_SERVS *conf)
{
	char cmd[1024];
	int i;
	memcpy(&g_ManagedServs, conf, sizeof(COSA_DML_MANAGED_SERVS));
	for(i=0;i<g_NrMSServs;i++)
	{
		if(!conf->Enable)
		{
			UpdateCosaDmlMSServ_Protocol(DISABLE,i);
		}
		else
		{
			UpdateCosaDmlMSServ_Protocol(ENABLE,i);
		}
	}

	return ANSC_STATUS_SUCCESS;
}
#endif
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
#if 1
	UpdateCosaDmlMSServ_Protocol(ADD,g_NrMSServs-1);  
#endif
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_DelEntry(ULONG ins)
{
    int i;

    if ((i = MSServ_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;
#if 1
	UpdateCosaDmlMSServ_Protocol(DELETE,i);  
#endif
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

	g_MSServs[i] = *pEntry;
	g_MSServs[i].InstanceNumber = ins; /* just in case */
#if 1
	UpdateCosaDmlMSServ_Protocol(REPLACE,i);
#endif

	return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedServices.TrustedUser
 */

#if 0
#define  MAX_MSTRUSTEDUSER_ENTRY      4
#endif
#if 1
#define  MAX_MSTRUSTEDUSER_ENTRY      100
#endif
#if 0
static int g_NrMSTrustedUsers   = MAX_MSTRUSTEDUSER_ENTRY / 2;
#endif
#if 1
static int g_NrMSTrustedUsers   = 0;
#endif

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
#if 1
void CosaDmlTrustedUser_Accept(int block_type,int ins,int operation)
{
	char cmd[1024];
	LONG ruleNumber;
	switch(block_type)
	{
		case TRUSTEDSITE_TYPE:
			if(operation == ADD)
			{
				sprintf(cmd,"iptables -I FORWARD 1 -s %s -j ACCEPT",g_TrustedUsers[ins].IPAddress);
				insertForwardTable(g_TrustedUsers[ins].InstanceNumber,TRUSTEDSITE_TYPE); 
				printForwardTable(1);
			}
			else
			{
				ruleNumber = getForwardRuleNumber(TRUSTEDSITE_TYPE,g_TrustedUsers[ins].InstanceNumber);	
				sprintf(cmd,"iptables -D FORWARD %d ",ruleNumber);
				forwardTableDelete(ruleNumber);
				printForwardTable(1);
			}

			break;
		case TRUSTEDSERVICE_TYPE:
			if(operation == ADD)
			{
				sprintf(cmd,"iptables -I FORWARD 1 -s %s -j ACCEPT",g_MSTrustedUsers[ins].IPAddress);
				insertForwardTable(g_MSTrustedUsers[ins].InstanceNumber,TRUSTEDSERVICE_TYPE); 
				printForwardTable(1);
			}
			else
			{
				ruleNumber = getForwardRuleNumber(TRUSTEDSERVICE_TYPE,g_TrustedUsers[ins].InstanceNumber);	
				sprintf(cmd,"iptables -D FORWARD %d ",ruleNumber);
				forwardTableDelete(ruleNumber);
				printForwardTable(1);
			}
			break;
	}
	system(cmd);

}
#endif

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
	#if 1
	CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,g_NrMSTrustedUsers-1,ADD);
	#endif
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_DelEntry(ULONG ins)
{
    int i;

    if ((i = MSTrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

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
#if 1
	if(pEntry->Trusted)
		CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,i,ADD);
	else
		CosaDmlTrustedUser_Accept(TRUSTEDSERVICE_TYPE,i,DELETE);
#endif

	return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedDevices.Device
 */
#if 0
#define  MAX_MDDEV_ENTRY      4
#endif
#if 1
#define  MAX_MDDEV_ENTRY      1024
#endif
#if 0
static int g_NrMDDevs   = MAX_MDDEV_ENTRY / 2;
#endif
#if 1
static int g_NrMDDevs   = 0;
#endif

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
#if 1
void UpdateCosaDmlMDDev_MacAddress(int OPERATION, ULONG ins)
{
	int i;
	char newvar[1024];
	char cmd[1024]= {'\0'};
	char protocol1[10];
	char action[15];
	LONG ruleNumber;
	switch(OPERATION)
	{
		case ADD:
			sprintf(newvar,"iptables  -t filter -I INPUT %ld ",inputCounter);
			break;

		case ENABLE:
		case DISABLE:
		case REPLACE:
			ruleNumber = getInputRuleNumber(DEVICE_TYPE,g_MDDevs[ins].InstanceNumber);	
			sprintf(newvar,"iptables -t filter -R INPUT %ld ",ruleNumber);
			break;

		case DELETE:
			ruleNumber = getInputRuleNumber(DEVICE_TYPE,g_MDDevs[ins].InstanceNumber);	
			sprintf(newvar,"iptables -D INPUT %ld ",ruleNumber);
			break;
	}
	if(OPERATION == DELETE)
	{
		system(newvar);
		inputTableDelete(ruleNumber);
		printInputTable(1);
	}
	else 
	{
		if( !g_MDDevs[ins].AlwaysBlock )
		{	
			if(g_MDDevs[ins].Type == MD_TYPE_BLOCK)
			{
				if(OPERATION == DISABLE )
				{
					sprintf(action,"ACCEPT");

				}
				else	
				{
					sprintf(action,"DROP");
				}
				snprintf(cmd,sizeof(cmd),"%s  -m mac --mac-source %s -m time --timestart %s --timestop %s --weekdays %s -j %s",newvar,g_MDDevs[ins].MACAddress,g_MDDevs[ins].StartTime,g_MDDevs[ins].EndTime,g_MDDevs[ins].BlockDays,action);
			}
			else if(g_MDDevs[ins].Type == MD_TYPE_ALLOW)
			{
				if(OPERATION == DISABLE  )
				{
					sprintf(action,"DROP");
				}
				else	
				{
					sprintf(action,"ACCEPT");
				}
				snprintf(cmd,sizeof(cmd),"%s  -m mac --mac-source %s -m time --timestart %s --timestop %s --weekdays %s -j %s",newvar,g_MDDevs[ins].MACAddress,g_MDDevs[ins].StartTime,g_MDDevs[ins].EndTime,g_MDDevs[ins].BlockDays,action);
			}
		}
		else
		{
			if(g_MDDevs[ins].Type == MD_TYPE_BLOCK)
			{
				if(OPERATION == DISABLE  )
				{
					sprintf(action,"ACCEPT");
				}
				else	
				{
					sprintf(action,"DROP");
				}
				snprintf(cmd,sizeof(cmd),"%s  -m mac --mac-source %s -j %s",newvar,g_MDDevs[ins].MACAddress,action);
			}
			else if(g_MDDevs[ins].Type == MD_TYPE_ALLOW)
			{
				if(OPERATION == DISABLE  )
				{
					sprintf(action,"DROP");
				}
				else	
				{
					sprintf(action,"ACCEPT");
				}
				snprintf(cmd,sizeof(cmd),"%s  -m mac --mac-source %s -j %s",newvar,g_MDDevs[ins].MACAddress,action);
			}
		}
		system(cmd);
	}

	if(OPERATION == ADD)
	{
		INPUT_RULEMAP[inputCounter].ins = g_MDDevs[ins].InstanceNumber;
		INPUT_RULEMAP[inputCounter].ruleNumber = inputCounter;
		INPUT_RULEMAP[inputCounter].Type = DEVICE_TYPE;
		inputCounter++;
		printInputTable(1);
	}
}

ANSC_STATUS
CosaDmlMngDevs_SetConf(COSA_DML_MANAGED_DEVS *conf)
{
	int i;
	memcpy(&g_ManagedDevs, conf, sizeof(COSA_DML_MANAGED_DEVS));
	if(DEV_ENABLE != conf->Enable)
	{

		for(i=0;i<g_NrMDDevs;i++)
		{
			if(!conf->Enable)
			{
				UpdateCosaDmlMDDev_MacAddress(DISABLE,i);
			}
			else
			{
				UpdateCosaDmlMDDev_MacAddress(ENABLE,i);
			}
		}
		DEV_ENABLE = conf->Enable;
	}
	return ANSC_STATUS_SUCCESS;
}
#endif

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
#if 1
	UpdateCosaDmlMDDev_MacAddress(ADD,g_NrMDDevs-1);
#endif

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_DelEntry(ULONG ins)
{
	int i;
	if ((i = MDDev_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;
#if 1
	UpdateCosaDmlMDDev_MacAddress(DELETE,i);
#endif
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

	g_MDDevs[i] = *pEntry;
	g_MDDevs[i].InstanceNumber = ins; /* just in case */
#if 1
	UpdateCosaDmlMDDev_MacAddress(REPLACE,i);
#endif


    return ANSC_STATUS_SUCCESS;
}

#endif /* _COSA_SIM_ */
