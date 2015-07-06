/* Class Name: Sensor.
 * Version: 1.0
 *  
 * Description: This class is used to create Sensor objects for each node. */
    
#include "mbed.h"
//#include "SDFileSystem.h"


#ifndef sensor_h         //This lines avoid to include library again if the library is included.
#define sensor_h

/* Defined useful constants */
#define MAX_SENSORS 5     //max nubmer of sensors per node.

/*--------------------------*/

class sensor
{
  private:
    /** This variables are used to connect with a specific pin in the board. They are initialized in the constructor. */
    AnalogIn _sensor;
  //DigitalIn _sensor;
  DigitalOut _sensorCtrl;
    
    
  public:
  
    /* Constructors */
    sensor(PinName sensorInPin, PinName voltageCtrl);
    
    /* sensor methods */
    float readS(void);
 // int readS(void); //read value of sensor
    void sensorOn(void);
    void sensorOff(void);
    int sensorState(void);
};

#endif