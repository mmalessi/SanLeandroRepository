#include "mbed.h"
//#include "SDFileSystem.h"

/** For Debuggin */
Serial pc(USBTX, USBRX);

#include "light.h"   //This is our classes
#include "node.h"
#include "rtos.h"

/** Define some useful constants */
#define LIGHT1  1   
#define LIGHT2  2
#define LIGHT3  3   
#define LIGHT4  4

//SDFileSystem sd(PTE3, PTE1, PTE2, PTE4, "sd"); // the pinout on the mbed board for SDHC card.

/** Switch in the board (for emulate sensors) */
DigitalIn sw2(SW2);
DigitalIn sw3(SW3);

node myNode(4);                 //Create a node object (global scope) with two lights

/** Main program */
int main(void)
{
    pc.baud(9600);      //Only for debuggin purpose
    
    /* Initializate SmartNode */
    myNode.createLight(LIGHT1,PTC2,PTD0,PTC4);          //Initialize LIGHT1 and connect to the ports: PTC3, PTD0, PTC4
    myNode.createLight(LIGHT2,PTA1, PTA2, PTC3);        //Initialize LIGHT2 and connect to the ports: PTA1, PTA2, PTC2
    myNode.createLight(LIGHT3,PTD1,PTD3,PTD2);          //Initialize LIGHT3 and connect to the ports: D13, D12, D11
    myNode.createLight(LIGHT4,PTA0, PTC11, PTC10);      //Initialize LIGHT4 and connect to the ports: D8, A5, A4
    

 /* myNode.createSensor(PTD1, 1); //13
    myNode.createSensor(PTD3, 2); //12
    myNode.createSensor(PTA0, 3); //8
    myNode.createSensor(PTB23, 4); //4 */
    myNode.timerStart();                            //Start timer
    
    
    /** Example 1 
    * Use of digitalClock effect method.*/
    //myNode.digitalClockEffectHz(LIGHT1,RED,4,20); //doesnt work        //Binary Clock of 4 Hz for 20 times
    //myNode.digitalClockEffectHz(LIGHT1,GREEN,1,INF);      //Binary Clock of 1 Hz for infinite (INF) times
    //myNode.digitalClockEffectHz(LIGHT1,GREEN,4,0.2,INF);  //Binary Clock of 4 Hz with dutty cycle of 20% in up and INF times
    
    
    /** Example 2
    * Use of Gaussian Effect method. */ 
    //myNode.gaussianEffect(LIGHT1,GREEN,3);                //Gaussian in GREEN led, LIGHT1, with 3s duration
//    myNode.gaussianEffect(LIGHT1,BLUE,255,3,9);   //doesnt work        //Gaussian in BLUE led, with 255 max blue value, 3s duration y 9s delay
//    myNode.gaussianEffect(LIGHT2,RED,MAX_LEVELS,3,0,INF); //doesnt work //Gaussian in RED led, with MAX_LEVELS max blue value, 3s duration, no delay and infinite iterations
    
    
    /** Example 4
    * Use of sinusoidal effect method. */
//    myNode.sinusoidalEffectHz(LIGHT1,RED,0.4,INF);            //0.4 Hz Sin(t) for INF times. Offset=128;
//    myNode.sinusoidalEffectHz(LIGHT1,GREEN,2,10,6);           //2 Hz Sin(t), 4 times and 6s Delay; Offset=128;
//    myNode.sinusoidalEffectHz(LIGHT2,BLUE,0.1,255,0,6,INF);   //0.1 Hz Sin(t),255 pico Value, 0 Offset, 6s Delay and INF times
    
   /** Example 5
   * Use of Stop Threads to Stop infinite Threads */
 //  myNode.digitalClockEffectHz(LIGHT1,GREEN,1,INF);      //Binary Clock of 1 Hz for infinite (INF) times
//   myNode.digitalClockEffectHz(LIGHT2,GREEN,1,INF);      //Binary Clock of 1 Hz for infinite (INF) times
//   wait(3);
//   myNode.exitThread(LIGHT1, EXIT_ALL_THREADS);         //Stop threads for LIGHT 1
//   wait(3);
//   myNode.digitalClockEffectHz(LIGHT1,GREEN,1,INF);      //Start Binary Clock of 1 Hz for infinite (INF) times again.
   
   /** Example 6 SENSOR TEST; sensor at pin 13 is 1 and sensor at pin 12 is 2*/
    /** Example 6 SENSOR TEST; sensor at pin 13 is 1 and sensor at pin 12 is 2*/

   myNode.createSensor(PTB10, PTB20, 1); //13, sensor 1 PTD1
   myNode.createSensor(PTB11, PTB20, 2); //12, sensor 2 PTD3
   myNode.createSensor(PTB3, PTB20, 4); //8, sensor 4
   myNode.createSensor(PTB2, PTB20, 3); //4, sensor 3
   
   myNode.setLightColor(LIGHT1,0, 0, 0);
   myNode.setLightColor(LIGHT2,0, 0, 0);
   myNode.setLightColor(LIGHT3,0, 0, 0);
   myNode.setLightColor(LIGHT4,0, 0, 0);
   
   myNode.showLights();
   
   double velocity;
   int iCase=0;         //Used for setting soft o hard lights
   int iTimeCase=0;       //Used for before, during, after, random
   int iCommand=0;
   
   Timer t12; 
   Timer t34;
   bool bFlagDir1=false;
   bool bFlagDir2=false;
   bool bFlagBeginR=false;
   bool bFlagBeginL=false;
   bool bFlagEnd=false;
   
   
   /*****************************/
   // MENU
   
//myNode.gaussianEffect(LIGHT1,GREEN,250,5,0,1);
//wait(1);
//myNode.gaussianEffect(LIGHT3,GREEN,250,5,0,1); 
//wait(10);
//myNode.gaussianEffect(LIGHT3,GREEN,250,5,0,1);
//wait(1);
//myNode.gaussianEffect(LIGHT1,GREEN,250,5,0,1); 
bool s1=0;
bool s2=0;
bool s3=0;
bool s4=0;

//THINGS TO DO
//ADD DELAY FOR SECOND LIGHT
//ADD DELAYS FOR ALL HARD MODES, TURNING ON TOO EARLY


while(true)    //Principal loop!
    
    {        
         pc.printf("Choose soft[1] o hard [2] lights...\n\r");
    
         while(iCase==0)
            {
                //pc.scanf("%d",&iCase);
                iCase = pc.getc();
                pc.printf("%d",iCase);   
            }
         printf("%d selected..\r\n",iCase);
         pc.printf("Choose before[1] o During[2] After[3] Random[4]  Nothing[5]...\r\n");
         while(iTimeCase==0)
            {
                //pc.scanf("%d",&iTimeCase);
                iTimeCase = pc.getc();
                pc.printf("%d",iTimeCase);   
            }
         printf("%d selected..\n\r",iTimeCase);
         
         pc.printf("Revisando estado de los sensores...\n\r");
         while(((double)s1>0.2)||((double)s2>0.2)||((double)s3>0.2)||((double)s4>0.2))
            {
    
                s1 = myNode.readDSensor(1);
                s2 = myNode.readDSensor(2);
                s3 = myNode.readDSensor(3);
                s4 = myNode.readDSensor(4);
            }
         printf("Sensores Ok..\n\r");
         
         pc.printf("Presione 1 para iniciar el test..\n\r");
         while((iCommand!=49))
            {
               // pc.scanf("%d",&iCommand);
               iCommand = pc.getc();
                pc.printf("%d",iCommand);   
            }
         
         pc.printf("Ready..\n\r");       
         
         
         
         while(bFlagEnd==false)
            {
            double distBwnLights = 2.172;
            double distFirstLight = 0.93;
            double secondLightExtra = 0.25;
            /*double dFirstDelay=0;
            double dSecondDelay=0;
            double dThirdDelay=0;
            double dFourthDelay=0;
            double dTimeOn=0;*/
            
            double delayL1 = 0;
            double delayL2 = 0;
            double delayL3 = 0;
            double delayL4 = 0;
            int randomCase = 0;
            
            double lightDuration = 0;
            //double disableWait = 0;
         
            s1 = myNode.readDSensor(1);
            s2 = myNode.readDSensor(2);
            s3 = myNode.readDSensor(3);
            s4 = myNode.readDSensor(4);
     
            if((s1!=s2)&&(s1==true)){
                t12.start();
                bFlagDir1=true;
                bFlagBeginL=true;
            }
            
            if((s3!=s4)&&(s4==true)){
                t34.start();          
                bFlagDir2=true;
                bFlagBeginR=true;
            } 
                   
            if((s4==true)&&(bFlagBeginL==true))
                bFlagEnd=true;
            
            if((s1==true)&&(bFlagBeginR==true))
                bFlagEnd=true;
            
            if((s1==false)&&(s2==false)){
                t12.stop();
                t12.reset();  
                bFlagDir1=false;
            }
            
            if((s3==false)&&(s4==false)){
                t34.stop();
                t34.reset();   
                bFlagDir2=false;
            }
            
            if((s1==true)&&(s2==true)&&(bFlagDir1==true)){
                t12.stop();
                float fTime=t12.read_ms();
                if(fTime>0)
                {
                    velocity=0.91*1000/fTime;
                    lightDuration = (2*distBwnLights)/velocity; // = thirdwait
                    switch(iTimeCase){
                    case 49:         //Before  
                        pc.printf("Caso Before..\n\r");                       
                        distFirstLight = 0.93;
                                 
                        //delayL1 = 0, firstwait = delayL2, secondwait = delayL3, fourthwait = delayL4
                        delayL1 = 0;   
                        delayL2 = (distFirstLight+secondLightExtra)/velocity; //wait to turn second light on, when person is under first light
                        delayL3 = (distBwnLights + distFirstLight)/velocity; //wait to turn third light on, when person under 2nd light
                        delayL4 =  (distFirstLight+(2*distBwnLights))/velocity; //wait to turn fourth light on
                        //disableWait = 6.4/velocity;
                        pc.printf("Time=%f velocity:%f\n\r",fTime,velocity);
                        break;
                    case 50:         //During
                        pc.printf("Caso During..\n\r");                       
                        distFirstLight = 1.53;
                        delayL1 = (distFirstLight/2)/velocity;
                        delayL2 = (distFirstLight+secondLightExtra)/velocity; //wait to turn second light on, when person is under first light
                        delayL3 = (distBwnLights + distFirstLight)/velocity; //wait to turn third light on, when person under 2nd light
                        delayL4 =  (distFirstLight+(2*distBwnLights))/velocity; //wait to turn fourth light on
                        //disableWait = 6.4/velocity;
                        pc.printf("Time=%f velocity:%f\n\r",fTime,velocity);
                        break;
                    case 51:         //After
                        pc.printf("Caso After..\n\r");                       
                        distFirstLight = 2.5;
                        delayL1 = (distFirstLight/2)/velocity;
                        delayL2 = (distFirstLight+secondLightExtra)/velocity; //wait to turn second light on, when person is under first light
                        delayL3 = (distBwnLights + distFirstLight)/velocity; //wait to turn third light on, when person under 2nd light
                        delayL4 =  (distFirstLight+(2*distBwnLights))/velocity; //wait to turn fourth light on
                        //disableWait = 6.4/velocity;
                        pc.printf("Time=%f velocity:%f\n\r",fTime,velocity);
                        break;
                    case 52:         //Random 
                        pc.printf("Caso Random..\n\r");
                        distFirstLight = 1.35;
                        randomCase = rand() % 4;
                        pc.printf("random int is: %d\n\r", randomCase);
                        if (randomCase > 1) { //if rand > 2 d3 = d1, d1 = d2
                            delayL3 = (distFirstLight/2)/velocity;                              
                            delayL1 = distFirstLight/velocity;
                        } else { // else d2 = d1, d1 = d2
                            delayL2 = (distFirstLight/2)/velocity;                              
                            delayL1 = distFirstLight/velocity;
                        }
                        if (delayL3 == 0) {
                            delayL3 = (distFirstLight+(2*distBwnLights))/velocity;
                            delayL4 = (distBwnLights + distFirstLight)/velocity;
                        } else if (delayL2 == 0) {
                            delayL2 = (distFirstLight+(2*distBwnLights))/velocity;
                            delayL4 =  distFirstLight/velocity;   
                        }
                        pc.printf("Time=%f velocity:%f\n\r",fTime,velocity);
                        break;
                    case 53:         //Nothing
                        pc.printf("Time=%f velocity:%f\n\r",fTime,velocity);
                        break;    
                    }
                
                    /** Light effects case */
                    switch(iCase){
                    case 49:
                        myNode.gaussianEffect(LIGHT3,RED,255,delayL3*1.7,delayL1,1);
                        myNode.gaussianEffect(LIGHT3,GREEN,255,delayL3*1.7,delayL1,1);
                        myNode.gaussianEffect(LIGHT3,BLUE,255,delayL3*1.7,delayL1,1);
                        
                        
                        myNode.gaussianEffect(LIGHT1,RED,255,lightDuration*1.7,delayL2,1);
                        myNode.gaussianEffect(LIGHT1,GREEN,255,lightDuration*1.7,delayL2,1);
                        myNode.gaussianEffect(LIGHT1,BLUE,255,lightDuration*1.7,delayL2,1);
                        
                        myNode.gaussianEffect(LIGHT2,RED,255,lightDuration*1.7,delayL3,1);
                        myNode.gaussianEffect(LIGHT2,BLUE,255,lightDuration*1.7,delayL3,1);
                        myNode.gaussianEffect(LIGHT2,GREEN,255,lightDuration*1.7,delayL3,1);
                        
                        myNode.gaussianEffect(LIGHT4,RED,255,lightDuration*1.7,delayL4,1); 
                        myNode.gaussianEffect(LIGHT4,GREEN,255,lightDuration*1.7,delayL4,1);
                        myNode.gaussianEffect(LIGHT4,BLUE,255,lightDuration*1.7,delayL4,1);
                        
                        
                        /*myNode.gaussianEffect(LIGHT3,GREEN,250,dThirdDelay*1.7,dFirstDelay,1);
                        myNode.gaussianEffect(LIGHT1,GREEN,250,dTimeOn*1.7,dSecondDelay,1);
                        myNode.gaussianEffect(LIGHT2,GREEN,250,dTimeOn*1.7,dThirdDelay,1);
                        myNode.gaussianEffect(LIGHT4,GREEN,250,dTimeOn*1.7,dFourthDelay,1);*/
                        break;
                    case 50:
                        myNode.digitalPulse(LIGHT3,RED,delayL3*1.7,delayL1*1.7);
                        myNode.digitalPulse(LIGHT3,GREEN,delayL3*1.7,delayL1*1.7);
                        myNode.digitalPulse(LIGHT3,BLUE,delayL3*1.7,delayL1*1.7);
                        
                        myNode.digitalPulse(LIGHT1,RED,lightDuration*1.7,delayL2*1.55);
                        myNode.digitalPulse(LIGHT1,GREEN,lightDuration*1.7,delayL2*1.55);
                        myNode.digitalPulse(LIGHT1,BLUE,lightDuration*1.7,delayL2*1.55);
                        
                        myNode.digitalPulse(LIGHT2,RED,lightDuration*1.7,delayL3*1.35);
                        myNode.digitalPulse(LIGHT2,GREEN,lightDuration*1.7,delayL3*1.35);
                        myNode.digitalPulse(LIGHT2,BLUE,lightDuration*1.7,delayL3*1.35);
                        
                        myNode.digitalPulse(LIGHT4,RED,lightDuration*1.7,delayL4*1.22);
                        myNode.digitalPulse(LIGHT4,GREEN,lightDuration*1.7,delayL4*1.22);
                        myNode.digitalPulse(LIGHT4,BLUE,lightDuration*1.7,delayL4*1.22);
                        //void digitalClockEffectHz(char nroLight, char cLedColor, float fFrecuency,float fDuttyCycle,int iTimes);
                        
                        break;
                    }  
                }
                pc.printf("Waiting for sensors 1 & 2..\n\r");
                while(((double)s1>0.2)||((double)s2>0.2))
                    {
                       s1 = myNode.readDSensor(1);
                       s2 = myNode.readDSensor(2);
                    }
                t12.reset();
                pc.printf("Ready... \n\r");    
                }
  
            if((s3==true)&&(s4==true)&&(bFlagDir2==true))
                {
                t34.stop();
                float fTime=t34.read_ms();
                if(fTime>0)
                {
                    velocity=0.91*1000/fTime;
                    lightDuration = (2*distBwnLights)/velocity; // = thirdwait
                switch(iTimeCase){
                    case 49:         //Before  
                        pc.printf("Caso Before..\n\r");                       
                                 
                        //delayL1 = 0, firstwait = delayL2, secondwait = delayL3, fourthwait = delayL4
                        delayL1 = 0;   
                        delayL2 = (distFirstLight+secondLightExtra)/velocity; //wait to turn second light on, when person is under first light
                        delayL3 = (distBwnLights + distFirstLight)/velocity; //wait to turn third light on, when person under 2nd light
                        delayL4 =  (distFirstLight+(2*distBwnLights))/velocity; //wait to turn fourth light on
                        //disableWait = 6.4/velocity;
                        pc.printf("Time=%f velocity:%f\n\r",fTime,velocity);
                        break;
                    case 50:         //During
                        pc.printf("Caso During..\n\r");                       
                        distFirstLight = 1.53;
                        delayL1 = (distFirstLight/2)/velocity;
                        delayL2 = (distFirstLight+secondLightExtra)/velocity; //wait to turn second light on, when person is under first light
                        delayL3 = (distBwnLights + distFirstLight)/velocity; //wait to turn third light on, when person under 2nd light
                        delayL4 =  (distFirstLight+(2*distBwnLights))/velocity; //wait to turn fourth light on
                        //disableWait = 6.4/velocity;
                        pc.printf("Time=%f velocity:%f\n\r",fTime,velocity);
                        break;
                    case 51:         //After
                        pc.printf("Caso After..\n\r");                       
                        distFirstLight = 2.5;
                        delayL1 = (distFirstLight/2)/velocity;
                        delayL2 = (distFirstLight+secondLightExtra)/velocity; //wait to turn second light on, when person is under first light
                        delayL3 = (distBwnLights + distFirstLight)/velocity; //wait to turn third light on, when person under 2nd light
                        delayL4 =  (distFirstLight+(2*distBwnLights))/velocity; //wait to turn fourth light on
                        //disableWait = 6.4/velocity;
                        pc.printf("Time=%f velocity:%f\n\r",fTime,velocity);
                        break;
                    case 52:         //Random 
                        pc.printf("Caso Random..\n\r");
                        randomCase = rand() % 4;
                        pc.printf("random int is: %d\n\r", randomCase);
                        if (randomCase > 1) { //if rand > 2 d3 = d1, d1 = d2
                            delayL3 = (distFirstLight/2)/velocity;                              
                            delayL1 = distFirstLight/velocity;
                        } else { // else d2 = d1, d1 = d2
                            delayL2 = (distFirstLight/2)/velocity;                              
                            delayL1 = distFirstLight/velocity;
                        }
                        if (delayL3 == 0) {
                            delayL3 = (distFirstLight+(2*distBwnLights))/velocity;
                            delayL4 = (distBwnLights + distFirstLight)/velocity;
                        } else if (delayL2 == 0) {
                            delayL2 = (distFirstLight+(2*distBwnLights))/velocity;
                            delayL4 =  distFirstLight/velocity;   
                        }
                        pc.printf("Time=%f velocity:%f\n\r",fTime,velocity);
                        break;
                    case 53:
                        break;
                    }
                
                    //** Light effects case 
                switch(iCase){
                    case 49:
                        //pc.printf("delay1 = %f, delay2 = %f, delay3 = %f, delay4 = %f\n\r", delayL1, delayL2, delayL3, delayL4);
                        myNode.gaussianEffect(LIGHT4,RED,255,delayL3*1.7,delayL1,1);
                        myNode.gaussianEffect(LIGHT4,GREEN,255,delayL3*1.7,delayL1,1);
                        myNode.gaussianEffect(LIGHT4,BLUE,255,delayL3*1.7,delayL1,1);
                        
                        myNode.gaussianEffect(LIGHT2,RED,255,lightDuration*1.7,delayL2,1);
                        myNode.gaussianEffect(LIGHT2,GREEN,255,lightDuration*1.7,delayL2,1);
                        myNode.gaussianEffect(LIGHT2,BLUE,255,lightDuration*1.7,delayL2,1);
                        
                        myNode.gaussianEffect(LIGHT1,RED,255,lightDuration*1.7,delayL3,1);
                        myNode.gaussianEffect(LIGHT1,GREEN,255,lightDuration*1.7,delayL3,1);
                        myNode.gaussianEffect(LIGHT1,BLUE,255,lightDuration*1.7,delayL3,1);
                        
                        myNode.gaussianEffect(LIGHT3,RED,255,lightDuration*1.7,delayL4,1);
                        myNode.gaussianEffect(LIGHT3,GREEN,255,lightDuration*1.7,delayL4,1);
                        myNode.gaussianEffect(LIGHT3,BLUE,255,lightDuration*1.7,delayL4,1);
                        break; 
                    case 50:  //
                        myNode.digitalPulse(LIGHT4,RED,delayL3*1.7,delayL1*1.7);
                        myNode.digitalPulse(LIGHT4,GREEN,delayL3*1.7,delayL1*1.7);
                        myNode.digitalPulse(LIGHT4,BLUE,delayL3*1.7,delayL1*1.7);
                        
                        myNode.digitalPulse(LIGHT2,RED,lightDuration*1.7,delayL2*1.55);
                        myNode.digitalPulse(LIGHT2,GREEN,lightDuration*1.7,delayL2*1.55);
                        myNode.digitalPulse(LIGHT2,BLUE,lightDuration*1.7,delayL2*1.55);
                        
                        myNode.digitalPulse(LIGHT1,RED,lightDuration*1.7,delayL3*1.35);
                        myNode.digitalPulse(LIGHT1,GREEN,lightDuration*1.7,delayL3*1.35);
                        myNode.digitalPulse(LIGHT1,BLUE,lightDuration*1.7,delayL3*1.35);
                        
                        myNode.digitalPulse(LIGHT3,RED,lightDuration*1.7,delayL4*1.22);
                        myNode.digitalPulse(LIGHT3,GREEN,lightDuration*1.7,delayL4*1.22);
                        myNode.digitalPulse(LIGHT3,BLUE,lightDuration*1.7,delayL4*1.22);
                        break;
                    }
           
                    pc.printf("Time=%f velocity:%f\n\r",fTime,velocity);   
                }
                pc.printf("Waiting for sensors 3 & 4..\n\r");
                while(((double)s3>0.2)||((double)s4>0.2))
                    {
                       s3 = myNode.readDSensor(3);
                       s4 = myNode.readDSensor(4);
                    }
                t34.reset();
                pc.printf("Ready... \n\r");    
                }
        }      
        pc.printf("Prueba finalizada..\n\r");
        iCase=0;
        iTimeCase=0;
        iCommand=0;
        bFlagBeginL=false;
        bFlagBeginR=false;
        bFlagEnd=false;
    }
}

    /** Example use Read File */
    //myNode.readFile(LIGHT1,"/sd/dataSLeandro/data.txt",0,INF); //Read data.txt file in microSD card. Delay 0s and INF iterations   
