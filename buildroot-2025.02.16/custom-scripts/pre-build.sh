#!/bin/sh
HOST=`ip route show | head --lines 1 | awk '{print $9}'`
mkdir -p $BASE_DIR/../overlay/etc/init.d/
cat $BASE_DIR/../custom-scripts/network-config | sed 's/\[IP-DO-HOST\]/'"$HOST"'/g' > $BASE_DIR/../overlay/etc/init.d/S41network-config
chmod +x $BASE_DIR/../overlay/etc/init.d/S41network-config

#=====Modulos =====
make -C $BASE_DIR/../modules/hello
make -C $BASE_DIR/../modules/miscdrv
make -C $BASE_DIR/../modules/procdrv
#make -C $BASE_DIR/../modules/timerdrv 
make -C $BASE_DIR/../modules/chardriver
make -C $BASE_DIR/../modules/waitdrv
make -C $BASE_DIR/../modules/segments
#=====Apps========================
make -C $BASE_DIR/../apps/banner
#make -C $BASE_DIR/../apps/monitor
#=====schedule
#make -C $BASE_DIR/../apps/sched-query
#make -C $BASE_DIR/../apps/sched-burn
#make -C $BASE_DIR/../apps/sched-rt
#make -C $BASE_DIR/../apps/schedrt-dl
#==========Discoo============= 3.x
#make -C $BASE_DIR/../apps/disk-test-raw
#make -C $BASE_DIR/../apps/disk-test-sector-read
#make -C $BASE_DIR/../apps/disk-test
#make -C $BASE_DIR/../apps/trace-dist
#==========Mem-teste 4.1
make -C $BASE_DIR/../apps/mem-test-malloc
make -C $BASE_DIR/../apps/mem-test-write
make -C $BASE_DIR/../apps/mem-test-read
make -C $BASE_DIR/../apps/mem-test-ulimit
#=++++++++++++Segmentacao 4.2
make -C $BASE_DIR/../apps/segments-address
make -C $BASE_DIR/../apps/segments-symbols
#===========PAginacao 4.3
make -C $BASE_DIR/../apps/paging-test





