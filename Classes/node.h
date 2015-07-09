/* Class Name: Light.
 * Version: 1.0
 *  
 * Description: This class allow to create object "light" which permit the control of the hardware lights.*/
    
#include "mbed.h"
#include "rtos.h"
#include "light.h"
#include "sensor.h"

#ifndef node_h         //This lines avoid to include library again if the library is included.
#define node_h

/* Defined useful constants */
#define TIME_BASE   0.005       //This is the time base by default for timer (in seconds).
#define MAX_LIGHTS  4           //Max number of the lights in the node
#define MAX_SENSORS  5 
#define DIGITAL 1
#define ANALOG  0

#define STOP                0
#define START               1

#define INF                 -1

#define EXIT_ALL_THREADS    0
#define EXIT_RED_THREAD     1
#define EXIT_GREEN_THREAD   2
#define EXIT_BLUE_THREAD    3
/*--------------------------*/

class node
{
  private:
    /** Timer variables */
    RtosTimer *rtNodeTimer;   //This timer calls periodically the show() function in the lights.
    float fTBase;           //It is the periodicity of the timer. By default TIME_BASE
    char cStatusTimer;       //Indicates the state of the timer (START or STOP); 
    
    /** Lights variables */
    light *myLights[MAX_LIGHTS]; //Array pointers to lights in the node.
    char cNroLights;             //Indicate the number of actives lights in the node
    
    /** Sensor variables */
    sensor *mySensors[MAX_SENSORS];
    
    
    
    /** flags */
    
    
    /** Private functions */
    static void fncTimer(void const *arg);  //Method helper for start the timer
                          
  public:
   
    /* Constructors */
    node();                     //No defined. Use node(char _cNroLights)
    node(char _cNroLights);

    /* Setters methods */
    void setPinLight(char nroLight,PinName rPin,PinName gPin, PinName bPin);
    void setLightColor(char cNroLight, int R, int G, int B);
      
    /* Getters methods */
    int getLedColor(char NroLight, char cLedColor);
    
    /* Public functions */
    void timerStart();
    void timerStop();
    void createLight(char nroLight,PinName rPin, PinName gPin, PinName bPin);
    void showLights();
    
    
    /* Sensor functions */
    void createSensor(PinName sensorPin, PinName voltageCtrl, int num);
    bool readDSensor(int sensorNum);
    void sensorVccOff(int sensorNum);
    void sensorVccOn(int sensorNum);
    int sensorVccState(int sensorNum);
    
    
    
        /** Threads control */
    void exitThread(char cNroLight, char cValue);
    
    /** Efects *********************************************/
    void digitalPulse(char nroLigth, char cLedColor, float fTime, float fDelay);
    
    void digitalClockEffectHz(char nroLight, char cLedColor, float fFrecuency,int iTimes);                       //Digital clock effect base on frecuency (Hz)
    void digitalClockEffectHz(char nroLight, char cLedColor, float fFrecuency,float fDuttyCycle,int iTimes);     //Digital clock effect base on frecuency (Hz)
    
    void gaussianEffect(char nroLight, char cLedColor, float fDuration);
    void gaussianEffect(char nroLight, char cLedColor,int iMaxValue, float fDuration,float fDelay);
    void gaussianEffect(char nroLight, char cLedColor,int iMaxValue, float fDuration,float fDelay, int iTimes);
    
    void sinusoidalEffectHz(char nroLight, char cLedColor, float fFrecuency,int iTimes);
    void sinusoidalEffectHz(char nroLight, char cLedColor, float fFrecuency, int iTimes,float fDelay);
    void sinusoidalEffectHz(char nroLight, char cLedColor, float fFrecuency,float fPicoValue,float fOffset,float fDelay,int iTimes);
    
   // void readFile(char nroLight, char* _cFileName,float fDelay,float iTimes);

    
   // void effect();
    
};

#endif