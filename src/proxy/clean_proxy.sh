#!/bin/bash
if [ -z $ZLYNN_HOME ]
then
    echo "LYNN_HOME is not set."
    exit -2
fi

rm -rf $LYNN_HOME/src/proxy/worker/gen-cpp $LYNN_HOME/src/proxy/master/gen-cpp

exit 0

