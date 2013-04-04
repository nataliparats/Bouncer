#!/bin/bash
##########################################################################
# Script for running bouncer applications in course ik2213.
# Note to students: you will have to adapt your application to this script
# The script name must not be changed, neither the parameters.
# Simply ignore unused parameters (such as port numbers in case of "ping").
# In the VMs, the tap interface should _always_ be "tap0".
# Tip: use comand 'export' if you need to set environment variables.
# -----------------------------------------------------------------------
# 2009-05-01 Dan Kopparhed dank@kth.se
##########################################################################
ltest=192.168.3.118
btest=172.16.0.113
#ltest=127.0.0.1
#btest=127.0.0.1
##########################################################################

fcheckroot()
{
        OUTPUT=`/usr/bin/whoami 2>&1 `
        if [ $OUTPUT != "root" ];then
                echo "Error: you need to be root for this. Aborting."
                exit 1
        fi
}

fcheckroot

#Normal ping:
#sendip -p ipv4 -is $ltest -p icmp $btest -d 0xffff0001

#Bad ICMP Type:
#sendip -p ipv4 -is $ltest -p icmp -ct 1 $btest -d 0xffff0001

#Bad ICMP BAD Reply ID:
#sendip -p ipv4 -is $ltest -p icmp -ct 0 $btest -d r4

#Bad ICMP code
#sendip -p ipv4 -is $ltest -p icmp -ct 8 -cd 1 $btest -d 0xffff0001

#Bad ICMP checksum:
#sendip -p ipv4 -is $ltest -p icmp -ct 8 -cc 0xffff $btest -d 0xffff0001

#Evil bit:
#sendip -p ipv4 -is $ltest -ifr 1 -p icmp $btest -d 0xffff0001

#Bad IP Version:
#sendip -p ipv4 -is $ltest -iv 0 -p icmp $btest -d 0xffff0001

#Bad IP Protocol:
#sendip -p ipv4 -is $ltest -ip 2 -p icmp $btest -d 0xffff0001

#Bad IP TTL:
#sendip -p ipv4 -is $ltest -it 0 -p icmp $btest -d 0xffff0001

#Bad IP Min Header Length:
#sendip -p ipv4 -is $ltest -ih 4 -p icmp $btest -d 0xffff0001

#Bad IP Max Header Length:
#sendip -p ipv4 -is $ltest -ih 0xf -p icmp $btest -d 0xffff0001


#Bad TCP Checksum:
#sendip -p ipv4 -is $ltest -p tcp -td 8080 -ts 0xfaaf -tc 0 $btest

#Bad TCP Min Length:
#sendip -p ipv4 -is $ltest -p tcp -td 8080 -ts 0xfaaf -tt 4 $btest

#FTP:
#ftp $btest

#TCP:
#w3m $btest
sendip -p ipv4 -is $ltest -p tcp -td 80 $btest

