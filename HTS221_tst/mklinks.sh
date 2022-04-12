export BASENAME=HTS221

echo $BASENAME


ln -s $PDEV_PATH/peripheral_dev_tst/"$BASENAME"_tst/"$BASENAME"_tst.cpp .
ln -s $PDEV_PATH/PeripheralDevices/$BASENAME/hts221.h include/
ln -s $PDEV_PATH/PeripheralDevices/$BASENAME/hts221.cpp src/

ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/getVersion.cpp src/
#ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/ADCInterface.h  include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/I2CInterface.h include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/dev_interface_def.h  include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/getVersion.h include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/DevErrorReporter.h  include/
ln -s $PDEV_PATH/PeripheralDevices/I2Cinterfaces/PicoI2CInterface.h include/
