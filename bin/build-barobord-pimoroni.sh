#!/bin/bash

# TODO: consolidate all the build files into one

cp keyboards/barobord/rules-pimoroni.mk keyboards/barobord/rules.mk
make barobord:sadekbaroudi ; cp .build/barobord_sadekbaroudi* /mnt/c/Users/sadek/Google\ Drive/qmk-keyboards/barobord-pimoroni/ ;
rm keyboards/barobord/rules.mk