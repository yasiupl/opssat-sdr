#/bin/env sh

IN_FILE=$1
OUT_FILE=$2

## renderfall command
BINARY=bin/renderfall
# BINARY=./renderfall_rgb
# BINARY=./renderfall_bw
# BINARY=renderfall

## Available FFT Windows
# WINDOWS=(hann gaussian square blackmanharris hamming kaiser parzen)
# WINDOWS=(blackmanharris hamming)
WINDOW=blackmanharris

# SIZE=1024
# SIZE=2048
# SIZE=4096
SIZE=8192
# SIZE=16384

DATE=$(date +"%Y-%m-%dT%H-%M-%S")

if [ -z $IN_FILE ]; then
    echo "No input file given"
    exit -1
fi

if [ -z $OUT_FILE ]; then
    OUT_FILE=waterfall
    echo "No output file name given. Defaulting to \"$OUT_FILE\"."
fi

OUTPUT=toGround/${OUT_FILE}_${DATE}
mkdir -p $OUTPUT

FILENAME=${OUT_FILE}_${WINDOW}_${SIZE}
echo $FILENAME
ARGUMENTS="$IN_FILE --format int16 --fftsize $SIZE --window $WINDOW --outfile $OUTPUT/$FILENAME.png"
echo $COMMAND $ARGUMENTS
export LD_PRELOAD=lib/libfftw3.so.3; $COMMAND $ARGUMENTS --verbose
