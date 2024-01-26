#!/usr/bin/env sh

EXP_PATH=$(dirname $0)
LIB_PATH=$EXP_PATH/lib
BINARY_PATH=$EXP_PATH/bin
CONFIG_FILE=$EXP_PATH/config.ini
DATE=$(date +"%Y%m%d_%H%M%S")
OUTPUT_PATH=${1}
DOWNLINK_PATH=$(awk -F "=" '/downlink_path/ {printf "%s",$2}' $CONFIG_FILE)
RECORDING_PATH=/dev/mmcblk0p180

## Run one of the routines:

action=$(awk -F "=" '/action/ {printf "%s",$2}' $CONFIG_FILE)

if [[ "$action" == "record" ]]; then
  $EXP_PATH/record.sh $OUTPUT_PATH
fi

if [[ "$action" == "waterfall" ]]; then
  $EXP_PATH/waterfall.sh $RECORDING_PATH $OUTPUT_PATH
fi

if [[ "$action" == "downsample" ]]; then
  $EXP_PATH/downsample.sh $OUTPUT_PATH
fi

## Downlink
downlink_samples=$(awk -F "=" '/downlink_samples/ {printf "%s",$2}' $CONFIG_FILE)

## Downlink from eMMC
if [[ $downlink_samples == true ]]; then
    echo "#### Restore samples from eMMC and put for downlink."
    $EXP_PATH/helper/downlink_from_emmc.sh $DOWNLINK_PATH
else
    echo "#### Samples stored in the eMMC. To downlink them later, run 'cd $PWD; ./helper/downlink_from_emmc.sh'"
fi

downlink_to_ground=$(awk -F "=" '/downlink_to_ground/ {printf "%s",$2}' $CONFIG_FILE)
if [[ $downlink_to_ground == true ]]; then
  filename="exp266_sdr_${action}_${DATE}.tar.gz"
  echo "#### Compress and move to downlink folder. Filename: $filename"
  tar cfzv $DOWNLINK_PATH/$filename $OUTPUT_PATH
  echo "#### Downlink folder:"
  ls -lhR $DOWNLINK_PATH
fi
