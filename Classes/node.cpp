#include "node.h"

extern Serial pc;   //This is used for access to Serial pc(USBTX, USBRX). It is used only for debuggin purposes.

/** Constructors **********************************************************/
node::node(void)            //This constructor uses MAX_LIGHTS by default
{
    
}
node::node(char _cNroLights)    //This constructor could specified every number of lights for a node.
{
    pc.baud(9600);      //Only for debuggin purpose
    
    if(_cNroLights>MAX_LIGHTS)
        cNroLights=MAX_LIGHTS;
    else
        cNroLights=_cNroLights;
    
    fTBase=(float)TIME_BASE;
    cStatusTimer=STOP;
    rtNodeTimer=NULL;
    
    for(char i=0;i<cNroLights;i++)
        myLights[i]=NULL;
        
    //sensor1 = NULL;
    //sensor2 = NULL;
}   

/** Private Methods *******************************************************/
void node::fncTimer(void const* arg)        //Static funcion helper for initiate a thread with showLights functions
{
    node* instance = (node*)arg;    //Cast the argument to a node instance pointer
    instance->showLights();         //Call the showLights method
}

/** Public methods */
/** This funcion put the RGB values in the ports for each lights. Additionally, generate timer signal for threads */
void node::showLights()
{
      for(char i=0;i<cNroLights;i++)    //This value depend of how much lights we have in the node. (cNroLights)
        {
            if(myLights[i]!=NULL){      //This line verifies that lights were created
                myLights[i]->show();    //Put the RGB registers in the freescale port
                myLights[i]->threadMessage(0x1,RED);    //Indicate to thread for RED color that it can be continue.
                myLights[i]->threadMessage(0x1,GREEN);  //Indicate to thread for GREEN color that it can be continue.
                myLights[i]->threadMessage(0x1,BLUE);   //Indicate to thread for BLUE color that it can be continue.
                }
        } 
}

/** Timer functions */
void node::timerStart()     //Create and star timer
{
    fTBase=TIME_BASE;       //TIME BASE is defined in node.h
       
    if(rtNodeTimer==NULL)  //Execute only if timer was not create before
         rtNodeTimer = new RtosTimer(&node::fncTimer, osTimerPeriodic, (void *)this);
    
    for(char i=0;i<cNroLights;i++){  //update timer in lights
        if(myLights[i]!=NULL)
           myLights[i]->setfTBase(fTBase);} 
           
    rtNodeTimer->start(fTBase*1000);               //The timer needs a time base in miliSeconds and TIME_BASE is defined in seconds
}

void node::timerStop()      //Stop the timer
{
    rtNodeTimer->stop();    
}


/** LIGTHS functions */
void node::createLight(char nroLight,PinName rPin, PinName gPin, PinName bPin)  //Create new ligths in the node.
{
    if((nroLight>0)&&(nroLight<=cNroLights))
        {
            myLights[nroLight-1]=new light(rPin, gPin, bPin);
            myLights[nroLight-1]->setRGB(0,0,0);
            myLights[nroLight-1]->setfTBase(fTBase);
        }
}

void node::setLightColor(char cNroLight, int R, int G, int B)       //Set a specific colors (RGB) for a light
{
    if((cNroLight>0)&&(cNroLight<=cNroLights))
        myLights[cNroLight-1]->setRGB(R,G,B);    
}

int node::getLedColor(char cNroLight, char cLedColor)
{
   return myLights[cNroLight]->getLedColor(cLedColor); 
}
/* Functions for threads */
void node::exitThread(char cNroLight, char cValue)      //Terminate all threads or an specific thread for LIGHT=cNroLight
{
    switch(cValue)
    {
        case EXIT_ALL_THREADS:
            if((cNroLight>0)&&(cNroLight<=cNroLights))
            {
                myLights[cNroLight-1]->exitAllThreads();
            }
            break;
        case EXIT_RED_THREAD:
            break;
        case EXIT_GREEN_THREAD:
            break;
        case EXIT_BLUE_THREAD:
            break;
    }    
}

/** Efects *************************************************/
void node::digitalPulse(char nroLight, char cLedColor, float fTime,float fDelay)
{
    if((nroLight!=NULL)&&(myLights[nroLight-1]->isThreadAvailable(cLedColor)==true))
          myLights[nroLight-1]->startStep(cLedColor, MAX_LEVELS,fDelay,fTime,1);
}

void node::digitalClockEffectHz(char nroLight, char cLedColor, float fFrecuency, int iTimes)
{
    if((nroLight!=NULL)&&(myLights[nroLight-1]->isThreadAvailable(cLedColor)==true))
          //myLights[nroLight-1]->startMultiLineal(cLedColor,0,(float)(1/(2*fFrecuency)),10,0,10,(float)(1/(2*fFrecuency)),0,0,0,0,-1,0);        
          myLights[nroLight-1]->startStep(cLedColor, MAX_LEVELS,(float)1/(2*fFrecuency),(float)1/(2*fFrecuency),iTimes);
}

void node::digitalClockEffectHz(char nroLight, char cLedColor, float fFrecuency, float fDC, int iTimes)
{
    if((nroLight!=NULL)&&(myLights[nroLight-1]->isThreadAvailable(cLedColor)==true))
          //myLights[nroLight-1]->startMultiLineal(cLedColor,0,(float)(1/(2*fFrecuency)),10,0,10,(float)(1/(2*fFrecuency)),0,0,0,0,-1,0);        
          myLights[nroLight-1]->startStep(cLedColor, MAX_LEVELS,(float)((1/fFrecuency)*(1-fDC)),((1/fFrecuency)*fDC),iTimes);
}

void node::sinusoidalEffectHz(char nroLight, char cLedColor, float fFrecuency, int iTimes)
{
    if((nroLight!=NULL)&&(myLights[nroLight-1]->isThreadAvailable(cLedColor)==true))      
          myLights[nroLight-1]->startSinusoidal(cLedColor,MAX_LEVELS,MAX_LEVELS/2,(float)fFrecuency,0,128,iTimes,0);
}
         
void node::sinusoidalEffectHz(char nroLight, char cLedColor, float fFrecuency, int iTimes,float fDelay)
{
    if((nroLight!=NULL)&&(myLights[nroLight-1]->isThreadAvailable(cLedColor)==true))      
          myLights[nroLight-1]->startSinusoidal(cLedColor,MAX_LEVELS,MAX_LEVELS/2,(float)fFrecuency,0,128,iTimes,fDelay);
}

void node::sinusoidalEffectHz(char nroLight, char cLedColor, float fFrecuency,float fVp,float fOffset,float fDelay,int iTimes)
{
    if((nroLight!=NULL)&&(myLights[nroLight-1]->isThreadAvailable(cLedColor)==true))      
          myLights[nroLight-1]->startSinusoidal(cLedColor,MAX_LEVELS,fVp,(float)fFrecuency,0,fOffset,iTimes,fDelay);
}

void node::gaussianEffect(char nroLight, char cLedColor,float t)
{
    if((nroLight!=NULL)&&(myLights[nroLight-1]->isThreadAvailable(cLedColor)==true))
                myLights[nroLight-1]->startSigmoid(cLedColor,MAX_LEVELS,(float)t/2,(float)t/7,t,1,0);    
}

void node::gaussianEffect(char nroLight, char cLedColor,int iMaxValue, float fDuration,float fDelay)
{
    if((nroLight!=NULL)&&(myLights[nroLight-1]->isThreadAvailable(cLedColor)==true))
                myLights[nroLight-1]->startSigmoid(cLedColor,iMaxValue,(float)fDuration/2,(float)fDuration/7,fDuration,1,fDelay);    
}

void node::gaussianEffect(char nroLight, char cLedColor,int iMaxValue, float fDuration,float fDelay, int iTimes)
{
    if((nroLight!=NULL)&&(myLights[nroLight-1]->isThreadAvailable(cLedColor)==true))
                myLights[nroLight-1]->startSigmoid(cLedColor,iMaxValue,(float)fDuration/2,(float)fDuration/7,fDuration,iTimes,fDelay);    
}

/*void node::readFile(char nroLight,char* _cFileName, float fDelay,float iTimes)
{
    char cLedColor=RED;
    if((nroLight!=NULL)&&(myLights[nroLight-1]->isThreadAvailable(cLedColor)==true))
        myLights[nroLight-1]->startReadFile(cLedColor,_cFileName,iTimes,fDelay);
}
*/
/** SENSOR functions -----------------------------------------------------------------------------*/
void node::createSensor(PinName sensorPin, PinName voltageCtrl, int num)  //Create new ligths in the node.
{
    if (num <= 5) {
        mySensors[num - 1] = new sensor(sensorPin, voltageCtrl);
    } else {
        pc.printf("Sensor number %d is invalid", num);
    }
}

bool node::readDSensor(int sensorNum)
{
    if (sensorNum <= 5) {
        if(mySensors[sensorNum - 1]->readS()>0.2)
            return true;
        else
            return false;
        //return mySensors[sensorNum - 1]->readS();  //It is ok for analog sensor
    } else {
        return 100; //error case
    }
}

void node::sensorVccOn(int sensorNum)
{
    if (sensorNum <= 5) {
        return mySensors[sensorNum - 1]->sensorOn();
    } else {
        pc.printf("Sensor number %d is invalid", sensorNum);
    }
        
}

void node::sensorVccOff(int sensorNum)
{
    if (sensorNum <= 5) {
        return mySensors[sensorNum - 1]->sensorOff();
    } else {
        pc.printf("Sensor number %d is invalid", sensorNum);        
    }
}
int node::sensorVccState(int sensorNum)
{
    if (sensorNum <= 5) {
        return mySensors[sensorNum - 1]->sensorState();
    } else {
        pc.printf("Sensor number %d is invalid", sensorNum);
        return 10000;
    }
}



/*------------------------------------------------------------------------------------------------*/

/** Only for testing */
/*void node::effect()
{
    myLights[0]->startReadFile(RED,"/sd/dataSLeandro/data.txt",-1,0);
}*/