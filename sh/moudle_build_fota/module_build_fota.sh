#!/bin/sh

CUR_PATH=$(pwd)
echo CurPath:$CUR_PATH

read -p "Input Fota Path:" FOTA_PATH
echo FOTA_PATH:$FOTA_PATH

# Clean Old Tmp
rm -rf $FOTA_PATH/module_fota_new
rm -rf $FOTA_PATH/module_fota_old
rm -rf $FOTA_PATH/module_execute.sh
rm -rf $CUR_PATH/module_fota_out

if [ -d "$FOTA_PATH" ]; then
    # Cp File
    cp -rf $CUR_PATH/module_fota_new $FOTA_PATH
    cp -rf $CUR_PATH/module_fota_old $FOTA_PATH
    cp -rf $CUR_PATH/module_execute.sh $FOTA_PATH
else
    echo "Error: not found. Can not continue."
    exit 1
fi

# Make 
cd $FOTA_PATH && ./module_execute.sh

# Cp Out
mv $FOTA_PATH/module_fota_out $CUR_PATH/module_fota_out

# Clean Tmp
rm -rf $FOTA_PATH/module_fota_new
rm -rf $FOTA_PATH/module_fota_old
rm -rf $FOTA_PATH/module_execute.sh