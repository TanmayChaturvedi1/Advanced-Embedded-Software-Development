#!/bin/bash

#clear
echo "Printing system details..."					>>	output.txt
echo "User Information - "			$(users)		>>	output.txt
echo "OS Type/Brand - "				$(uname -o)		>>	output.txt
echo "OS Distribution - "			$(lsb_release -d)	>>	output.txt
echo "OS Version - "				$(lsb_release -a)	>>	output.txt
echo "Kernel Version - "			$(uname -r)		>>	output.txt
echo "Kernel gcc version build - "		$(gcc --version)	>>	output.txt
echo "Kernel Build Time - "			$(cat /proc/version)	>>	output.txt
echo "System Architecture Information - "	$(arch)			>>	output.txt
echo "Info on File System Architecture - "	$(free -m)		>>	output.txt
