ln -s $PDEV_PATH/PdevProj/SensBoxEnvMbed/src/SensBoxEnvMbed.cpp SensBoxEnvSer.cpp

ln -s $PDEV_PATH/PdevProj/SensBoxEnv/lib/scpiparser.h  include/
ln -s $PDEV_PATH/PdevProj/SensBoxEnv/lib/envsensread.h  include/
ln -s $PDEV_PATH/PdevProj/SensBoxEnv/lib/env_scpiparser.h include/
ln -s $PDEV_PATH/PdevProj/SensBoxEnv/lib/AT30TSE75x_E.h  include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/I2CInterface.h include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/DevErrorReporter.h include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/DACInterface.h include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/ADCInterface.h include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/dev_interface_def.h include/
ln -s $PDEV_PATH/PeripheralDevices/I2Cinterfaces/PicoI2CInterface.h include/
ln -s $PDEV_PATH/PeripheralDevices/I2Cinterfaces/DummyI2CInterface.h include/
ln -s $PDEV_PATH/PeripheralDevices/I2Cinterfaces/DummyDigitalOut.h include/
ln -s $PDEV_PATH/PeripheralDevices/I2Cinterfaces/DummyDigitalIn.h include/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/getVersion.h include/
ln -s $PDEV_PATH/PeripheralDevices/AT30TSE75x/AT30TSE75x.h include/
ln -s $PDEV_PATH/PeripheralDevices/HTS221/hts221.h include/
ln -s $PDEV_PATH/PeripheralDevices/VEML7700/veml7700.h include/

ln -s $PDEV_PATH/PdevProj/SensBoxEnv/lib/scpiparser.cpp src/
ln -s $PDEV_PATH/PdevProj/SensBoxEnv/lib/envsensread.cpp src/
ln -s $PDEV_PATH/PdevProj/SensBoxEnv/lib/env_scpiparser.cpp src/
ln -s $PDEV_PATH/PeripheralDevices/DevInterfaces/getVersion.cpp src/
ln -s $PDEV_PATH/PeripheralDevices/AT30TSE75x/AT30TSE75x.cpp src/
ln -s $PDEV_PATH/PeripheralDevices/HTS221/hts221.cpp src/
ln -s $PDEV_PATH/PeripheralDevices/VEML7700/veml7700.cpp src/
