#include "sensor.h"
#include "mbed.h"
extern Serial pc;   //This is used for access to Serial pc(USBTX, USBRX). It is used only for debuggin purposes.




/* Constructors ******************************************************************************************/
/*sensor::sensor(void) : _sensors(PTD1)
{
}*/

sensor::sensor(PinName sensorPin, PinName voltageCtrl):  _sensor(sensorPin), _sensorCtrl(voltageCtrl)
{
   
}


float sensor::readS()
{
    return _sensor.read(); 
}

int sensor::sensorState()
{
    return _sensorCtrl.read();
}

void sensor::sensorOn()
{
    _sensorCtrl.write(1);
}

void sensor::sensorOff()
{
    _sensorCtrl.write(0);
}

