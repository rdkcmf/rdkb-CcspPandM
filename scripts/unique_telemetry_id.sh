#!/bin/sh
source /etc/device.properties
source /etc/utopia/service.d/log_capture_path.sh

TAG_STRING=`syscfg get unique_telemetry_tag`
echo_t "$TAG_STRING"
