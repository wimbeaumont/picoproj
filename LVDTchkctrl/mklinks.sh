export BASENAME=AT30TSE75

ln -s $PDEV_PATH/PeripheralDevices/"$BASENAME"x/"$BASENAME"x.h include/
ln -s $PDEV_PATH/PeripheralDevices/"$BASENAME"x/"$BASENAME"x.cpp src/

ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/getVersion.cpp src/
#ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/ADCInterface.h  include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/I2CInterface.h include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/dev_interface_def.h  include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/getVersion.h include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/DevErrorReporter.h  include/
ln -s $PDEV_PATH/PeripheralDevices/I2Cinterfaces/PicoI2CInterface.h include/
