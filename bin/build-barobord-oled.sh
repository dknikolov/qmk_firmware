#!/bin/bash

# TODO: consolidate all the build files into one

cp keyboards/xoiviox/barobord/rules.mk keyboards/xoiviox/barobord/rules.mk.bak
cp keyboards/xoiviox/barobord/rules-oled.mk keyboards/xoiviox/barobord/rules.mk
make xoiviox/barobord:sadekbaroudi ; cp .build/xoiviox_barobord_sadekbaroudi* /mnt/g/My\ Drive/qmk-keyboards/barobord-oled/ ;
mv keyboards/xoiviox/barobord/rules.mk.bak keyboards/xoiviox/barobord/rules.mk
