#!/bin/sh
# This script will push the erouter0 IP address from ARM to ATOM.
#-----------------------------------------------------------------------------
if [ "$1" == "" ]; then
	exit 0;
fi

ATOM_RPC_IP=`cat /etc/device.properties | grep ATOM_ARPING_IP | cut -f 2 -d"="`
if [ -e "/usr/bin/rpcclient" ]; then
	#for Puma6
	rpcclient $ATOM_RPC_IP "sec_pushown.sh --ip4=\"$1\""
fi
