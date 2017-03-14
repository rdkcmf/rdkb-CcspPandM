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

#ifdef __GNUC__
#if (!defined _BUILD_ANDROID) && (!defined _NO_EXECINFO_H_)
#include <execinfo.h>
#endif
#endif

#include "ssp_global.h"
#include "stdlib.h"
#include "ccsp_dm_api.h"
#ifdef USE_PCD_API_EXCEPTION_HANDLING
#include "pcdapi.h"
#endif
#ifdef ENABLE_SD_NOTIFY
#include <systemd/sd-daemon.h>
#endif

//#include <docsis_ext_interface.h>

#ifdef INCLUDE_BREAKPAD
#include "breakpad_wrapper.h"
#endif

#define DEBUG_INI_NAME  "/etc/debug.ini"

PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController      = NULL;
PCOMPONENT_COMMON_DM            g_pComponent_Common_Dm  = NULL;
char                            g_Subsystem[32]         = {0};
PCCSP_COMPONENT_CFG             gpPnmStartCfg           = NULL;
PCCSP_FC_CONTEXT                pPnmFcContext           = (PCCSP_FC_CONTEXT           )NULL;
PCCSP_CCD_INTERFACE             pPnmCcdIf               = (PCCSP_CCD_INTERFACE        )NULL;
PCCC_MBI_INTERFACE              pPnmMbiIf               = (PCCC_MBI_INTERFACE         )NULL;
BOOL                            g_bActive               = FALSE;
extern  ANSC_HANDLE                     bus_handle;

int  cmd_dispatch(int  command)
{
    ULONG                           ulInsNumber        = 0;
    parameterValStruct_t            val[3]             = {0};
    char*                           pParamNames[]      = {"Device.X_CISCO_COM_DDNS."};
    parameterValStruct_t**          ppReturnVal        = NULL;
    parameterInfoStruct_t**         ppReturnValNames   = NULL;
    parameterAttributeStruct_t**    ppReturnvalAttr    = NULL;
    ULONG                           ulReturnValCount   = 0;
    ULONG                           i                  = 0;

    switch ( command )
    {
            case	'e' :

#ifdef _ANSC_LINUX
                CcspTraceInfo(("Connect to bus daemon...\n"));

            {
                char                            CName[256];

                if ( g_Subsystem[0] != 0 )
                {
                    _ansc_sprintf(CName, "%s%s", g_Subsystem, gpPnmStartCfg->ComponentId);
                }
                else
                {
                    _ansc_sprintf(CName, "%s", gpPnmStartCfg->ComponentId);
                }

                ssp_PnmMbi_MessageBusEngage
                    ( 
                        CName,
                        CCSP_MSG_BUS_CFG,
                        gpPnmStartCfg->DbusPath
                    );
            }

#endif

                ssp_create_pnm(gpPnmStartCfg);
                ssp_engage_pnm(gpPnmStartCfg);

                g_bActive = TRUE;

                CcspTraceInfo(("Provision & Management Module loaded successfully...\n"));

            break;

            case    'r' :

            CcspCcMbi_GetParameterValues
                (
                    DSLH_MPA_ACCESS_CONTROL_ACS,
                    pParamNames,
                    1,
                    &ulReturnValCount,
                    &ppReturnVal,
                    NULL
                );



            for ( i = 0; i < ulReturnValCount; i++ )
            {
                CcspTraceWarning(("Parameter %d name: %s value: %s \n", i+1, ppReturnVal[i]->parameterName, ppReturnVal[i]->parameterValue));
            }


/*
            CcspCcMbi_GetParameterNames
                (
                    "Device.DeviceInfo.",
                    0,
                    &ulReturnValCount,
                    &ppReturnValNames
                );

            for ( i = 0; i < ulReturnValCount; i++ )
            {
                CcspTraceWarning(("Parameter %d name: %s bWritable: %d \n", i+1, ppReturnValNames[i]->parameterName, ppReturnValNames[i]->writable));
            }
*/
/*
            CcspCcMbi_GetParameterAttributes
                (
                    pParamNames,
                    1,
                    &ulReturnValCount,
                    &ppReturnvalAttr
                );
*/
/*
            CcspCcMbi_DeleteTblRow
                (
                    123,
                    "Device.X_CISCO_COM_SWDownload.SWDownload.1."
                );
*/

			break;

        case    'm':

                AnscPrintComponentMemoryTable(pComponentName);

                break;

        case    't':

                AnscTraceMemoryTable();

                break;

        case    'c':

                ssp_cancel_pnm(gpPnmStartCfg);

                break;

        default:
            break;
    }

    return 0;
}

static void _print_stack_backtrace(void)
{
#ifdef __GNUC__
#if (!defined _BUILD_ANDROID) && (!defined _NO_EXECINFO_H_)
        void* tracePtrs[100];
        char** funcNames = NULL;
        int i, count = 0;

        int fd;
        const char* path = CCSP_PNM_BACKTRACE_FILE;

        /*
         *  print backtrace to the backtrace file
         */
        count = backtrace( tracePtrs, 100 );

        fd = open(path, O_RDWR | O_CREAT);
        if (fd < 0)
        {
            fprintf(stderr, "failed to open backtrace file: %s", path);
        }
        else
        {
            backtrace_symbols_fd( tracePtrs, count, fd);
            close(fd);
        }
        /*
         *  print backtrace to stdout
         */
        backtrace_symbols_fd(tracePtrs, count, 1);
#endif
#endif
}

#if defined(_ANSC_LINUX)
static void daemonize(void) {
	int fd;
	switch (fork()) {
	case 0:
		break;
	case -1:
		// Error
		CcspTraceInfo(("Error daemonizing (fork)! %d - %s\n", errno, strerror(
				errno)));
		exit(0);
		break;
	default:
		_exit(0);
	}

	if (setsid() < 	0) {
		CcspTraceInfo(("Error demonizing (setsid)! %d - %s\n", errno, strerror(errno)));
		exit(0);
	}

    /*
     *  What is the point to change current directory?
     *
    chdir("/");
     */

#ifndef  _DEBUG

	fd = open("/dev/null", O_RDONLY);
	if (fd != 0) {
		dup2(fd, 0);
		close(fd);
	}
	fd = open("/dev/null", O_WRONLY);
	if (fd != 1) {
		dup2(fd, 1);
		close(fd);
	}
	fd = open("/dev/null", O_WRONLY);
	if (fd != 2) {
		dup2(fd, 2);
		close(fd);
	}
#endif
}

void sig_handler(int sig)
{

#ifndef _COSA_INTEL_USG_ATOM_
    CcspBaseIf_deadlock_detection_log_print(sig);
#endif

    if ( sig == SIGINT ) {
    	signal(SIGINT, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGINT received!\n"));
        exit(0);
    }
    else if ( sig == SIGUSR1 ) {
    	signal(SIGUSR1, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGUSR1 received!\n"));
    }
    else if ( sig == SIGUSR2 ) {
    	CcspTraceInfo(("SIGUSR2 received!\n"));
    }
    else if ( sig == SIGCHLD ) {
    	signal(SIGCHLD, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGCHLD received!\n"));
    }
    else if ( sig == SIGPIPE ) {
    	signal(SIGPIPE, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGPIPE received!\n"));
    }
    else if ( sig == SIGTERM )
    {
        CcspTraceInfo(("SIGTERM received!\n"));
        exit(0);
    }
    else if ( sig == SIGKILL )
    {
        CcspTraceInfo(("SIGKILL received!\n"));
        exit(0);
    }
	else if ( sig == SIGALRM ) 
	{

    	signal(SIGALRM, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGALRM received!\n"));
		RDKLogEnable = GetLogInfo(bus_handle,"eRT.","Device.LogAgent.X_RDKCENTRAL-COM_LoggerEnable");
		RDKLogLevel = (char)GetLogInfo(bus_handle,"eRT.","Device.LogAgent.X_RDKCENTRAL-COM_LogLevel");
		PAM_RDKLogLevel = GetLogInfo(bus_handle,"eRT.","Device.LogAgent.X_RDKCENTRAL-COM_PAM_LogLevel");
		PAM_RDKLogEnable = (char)GetLogInfo(bus_handle,"eRT.","Device.LogAgent.X_RDKCENTRAL-COM_PAM_LoggerEnable");
	}
    else {
    	/* get stack trace first */
    	_print_stack_backtrace();
    	CcspTraceInfo(("Signal %d received, exiting!\n", sig));
    	exit(0);
    }
}

static int is_core_dump_opened(void)
{
    FILE *fp;
    char path[256];
    char line[1024];
    char *start, *tok, *sp;
#define TITLE   "Max core file size"

    snprintf(path, sizeof(path), "/proc/%d/limits", getpid());
    if ((fp = fopen(path, "rb")) == NULL)
        return 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if ((start = strstr(line, TITLE)) == NULL)
            continue;

        start += strlen(TITLE);
        if ((tok = strtok_r(start, " \t\r\n", &sp)) == NULL)
            break;

        fclose(fp);

        if (strcmp(tok, "0") == 0)
            return 0;
        else
            return 1;
    }

    fclose(fp);
    return 0;
}

#endif

int main(int argc, char* argv[])
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    int                             cmdChar            = 0;
    BOOL                            bRunAsDaemon       = TRUE;
    int                             idx                = 0;
    char                            cmd[1024]          = {0};
    FILE                           *fd                 = NULL;
    DmErr_t                         err;
    char                            *subSys            = NULL;
    extern ANSC_HANDLE bus_handle;
	#ifdef FEATURE_SUPPORT_RDKLOG
	rdk_logger_init(DEBUG_INI_NAME);
	#endif

    /*
     *  Load the start configuration
     */
    gpPnmStartCfg = (PCCSP_COMPONENT_CFG)AnscAllocateMemory(sizeof(CCSP_COMPONENT_CFG));
    
    if ( gpPnmStartCfg )
    {
        CcspComponentLoadCfg(CCSP_PNM_START_CFG_FILE, gpPnmStartCfg);
    }
    else
    {
        CcspTraceError(("RDKB_SYSTEM_BOOT_UP_LOG : PandM,Insufficient resources for start configuration, quit!\n"));
        printf("Insufficient resources for start configuration, quit!\n");
        exit(1);
    }
    
    /* Set the global pComponentName */
    pComponentName = gpPnmStartCfg->ComponentName;

#if defined(_DEBUG) && defined(_COSA_SIM_)
    AnscSetTraceLevel(CCSP_TRACE_LEVEL_INFO);
#endif

    for (idx = 1; idx < argc; idx++)
    {
        if ( (strcmp(argv[idx], "-subsys") == 0) )
        {
            AnscCopyString(g_Subsystem, argv[idx+1]);
            CcspTraceWarning(("\nSubsystem is %s\n", g_Subsystem));
        }
        else if ( strcmp(argv[idx], "-c" ) == 0 )
        {
            bRunAsDaemon = FALSE;
        }
    }

#if  defined(_ANSC_WINDOWSNT)

    AnscStartupSocketWrapper(NULL);

    display_info();

    cmd_dispatch('e');

    while ( cmdChar != 'q' )
    {
        cmdChar = getchar();

        cmd_dispatch(cmdChar);
    }
#elif defined(_ANSC_LINUX)
    if ( bRunAsDaemon )
        daemonize();

    /*This is used for ccsp recovery manager */
    fd = fopen("/var/tmp/CcspPandMSsp.pid", "w+");
    if ( !fd )
    {
        CcspTraceWarning(("Create /var/tmp/CcspPandMSsp.pid error. \n"));
    }
    else
    {
        sprintf(cmd, "%d", getpid());
        fputs(cmd, fd);
        fclose(fd);
    }
#ifdef INCLUDE_BREAKPAD
    breakpad_ExceptionHandler();
#endif
#ifndef INCLUDE_BREAKPAD

    if (is_core_dump_opened())
    {
        signal(SIGUSR1, sig_handler);
        signal(SIGPIPE, SIG_IGN);
        CcspTraceWarning(("Core dump is opened, do not catch signal\n"));
    }
    else
    {
        CcspTraceWarning(("Core dump is NOT opened, backtrace if possible\n"));
        signal(SIGTERM, sig_handler);
        signal(SIGINT, sig_handler);
        signal(SIGUSR1, sig_handler);
        signal(SIGUSR2, sig_handler);

        signal(SIGSEGV, sig_handler);
        signal(SIGBUS, sig_handler);
        signal(SIGKILL, sig_handler);
        signal(SIGFPE, sig_handler);
        signal(SIGILL, sig_handler);
        signal(SIGQUIT, sig_handler);
        signal(SIGHUP, sig_handler);
        signal(SIGPIPE, SIG_IGN);
    }

#ifdef USE_PCD_API_EXCEPTION_HANDLING
    printf("Registering PCD exception handler\n");
    CcspTraceWarning(("RDKB_SYSTEM_BOOT_UP_LOG : PandM Registering PCD exception handler... \n"));
    PCD_api_register_exception_handlers( argv[0], NULL );
#endif
#endif

    cmd_dispatch('e');

	RDKLogEnable = GetLogInfo(bus_handle,g_Subsystem,"Device.LogAgent.X_RDKCENTRAL-COM_LoggerEnable");
	RDKLogLevel = (char)GetLogInfo(bus_handle,g_Subsystem,"Device.LogAgent.X_RDKCENTRAL-X_RDKCENTRAL-COM_LogLevel");
	PAM_RDKLogLevel = GetLogInfo(bus_handle,g_Subsystem,"Device.LogAgent.X_RDKCENTRAL-COM_PAM_LogLevel");
	PAM_RDKLogEnable = (char)GetLogInfo(bus_handle,g_Subsystem,"Device.LogAgent.X_RDKCENTRAL-COM_PAM_LoggerEnable");
    // printf("Calling Docsis\n");

    // ICC_init();
    // DocsisIf_StartDocsisManager();

#ifdef _COSA_SIM_
    subSys = "";        /* PC simu use empty string as subsystem */
#else
    subSys = NULL;      /* use default sub-system */
#endif
    err = Cdm_Init(bus_handle, subSys, NULL, NULL, pComponentName);
    if (err != CCSP_SUCCESS)
    {
        fprintf(stderr, "Cdm_Init: %s\n", Cdm_StrError(err));
        exit(1);
    }

#ifdef ENABLE_SD_NOTIFY
    sd_notifyf(0, "READY=1\n"
              "STATUS=CcspPandMSsp is Successfully Initialized\n"
              "MAINPID=%lu", (unsigned long) getpid());
	
    CcspTraceInfo(("RDKB_SYSTEM_BOOT_UP_LOG : P&M sd_notify Called\n"));
#endif

    system("touch /tmp/pam_initialized");

#if defined(_COSA_INTEL_USG_ARM_)
    {
        // touch pam_initialized on the ATOM side
        #define DATA_SIZE 1024
        FILE *fp1;
        char buf[DATA_SIZE] = {0};
        char cmd1[DATA_SIZE] = {0};
        char cmd2[DATA_SIZE] = {0};

        // Grab the ATOM RPC IP address
        sprintf(cmd1, "cat /etc/device.properties | grep ATOM_ARPING_IP | cut -f 2 -d\"=\"");

        fp1 = popen(cmd1, "r");
        if (fp1 == NULL) {
            CcspTraceError(("Error opening command pipe! \n"));
            return FALSE;
        }

        fgets(buf, DATA_SIZE, fp1);

        buf[strcspn(buf, "\r\n")] = 0; // Strip off any carriage returns

        if (buf[0] != 0 && strlen(buf) > 0) {
            CcspTraceInfo(("Reported an ATOM IP of %s \n", buf));
            sprintf(cmd2, "rpcclient %s \"/bin/touch /tmp/pam_initialized\"", buf);
            //CcspTraceInfo(("Command to run \"%s\"", cmd2));
            system(cmd2);
        }

        if (pclose(fp1) != 0) {
            /* Error reported by pclose() */
            CcspTraceError(("Error closing command pipe! \n"));
        }
    }
#endif

    printf("Entering P&M loop\n");
    CcspTraceWarning(("RDKB_SYSTEM_BOOT_UP_LOG : Entering P&M loop... \n"));

    if ( bRunAsDaemon )
    {
        while(1)
        {
            sleep(30);
        }
    }
    else
    {
        while ( cmdChar != 'q' )
        {
            cmdChar = getchar();

            cmd_dispatch(cmdChar);
        }
    }
#endif

    err = Cdm_Term();
    if (err != CCSP_SUCCESS)
    {
        fprintf(stderr, "Cdm_Term: %s\n", Cdm_StrError(err));
        exit(1);
    }

    if ( g_bActive )
    {
        ssp_cancel_pnm(gpPnmStartCfg);

        g_bActive = FALSE;
    }

    return 0;
}


