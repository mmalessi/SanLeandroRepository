/* Class Name: Light.
 * Version: 1.0
 *  
 * Description: This class allow to create object "light" which permit the control of the hardware lights.*/
    
#include "mbed.h"
#include "rtos.h"
//#include "SDFileSystem.h"

#ifndef light_h         //This lines avoid to include library again if the library is included.
#define light_h

/* Defined useful constants */
#define MAX_LEVELS  255     //This constant defines the maximun number of levels for representing a color. 0 no color, 255 maximun color. 
#define STEP        0
#define LINEAL      1
#define CUADRATIC   2
#define CUBIC       3
#define MULTILINEAL 10
#define SIGMOID     11
#define SINUSOIDAL  12
#define WELDER      31
#define READFILE    41

#define RED         0
#define GREEN       1
#define BLUE        2

#define PI          3.1415
/*--------------------------*/

class light
{
  private:
    /** This variables are used to connect with a specific pin in the board. They are initialized in the constructor. */
    PwmOut _rPin;
    PwmOut _gPin;
    PwmOut _bPin;
    
    /** This variables are used to storage the level of red, green and blue in the light */
    int iValue[3];    
    
    /** Temporal variables for share data between special methods */
    float fTemp[3][20];
    char *cFileName;
    
    /** This variable are used to indicates which transition is call */
    int uiTransition[3];
    
    /** Flags */
    bool bShow;     //Indicates if the last rValue, gValue and bValue were showed.
    bool bExitThread[3]; //
    bool bAvailable[3];  //Indicates if the thread are available.

    /** Thread Pointers */
    Thread *ledThread[3];
    
    /** Base time for clock of interactions */
    float fTBase;
    
    /** Private functions ***********************************************************************/
    /** Thread start for each color (R-G-B) */
    static void start_rThread(void const *p);
    static void start_gThread(void const *p);
    static void start_bThread(void const *p);
    void createThread(char cLedColor);                  
    
    
  public:
  
    /* Constructors */
    light(PinName rPin,PinName gPin, PinName bPin);  //This constructor allows to create a object "ligth" and set the pins conected in the board
    
    /* Setters methods */
    void setLedColor(char cLedColor, int value);
    void setRGB(int R, int G, int B); //R,G,B could have values between 0 and MAX_LEVELS
    void setfTBase(float time);
    
    /* Getters methods */
    int  getLedColor(char cLedColor);
    int getThreadStatus(char cLedColor);
    
    /* Public functions ***********************************************************************/
    
    /** Thread Functions */
    
    void threadMessage(int iMessage,char cLedColor);    //Message for threads of each color (R-G-B)
    bool isThreadInactive(char cLedColor);              //Detect if a Thread is Inactive 
    bool isThreadAvailable(char cLedColor);
    
    /** This function turn on the light with colors set in rValue, gValue and bValue variables. */
    void show(void);
    bool isShowed(void);
    
    /** Flags */
    void exitAllThreads();
    
    /* LED transtions */
    /** Laucher functions */
    void startStep(char cLedColor, int iFinColor, float fTDown,float fTUp, int iTimes);
    void startLineal(char cLedColor,int iFinColor, float t, int iTimes);
    void startSigmoid(char cLedColor,int iFinColor, float fMean, float fSigma ,float iTime, int iTimes, float fDelay);
    void startSinusoidal(char cLedColor, int iFinColor, float fValue_pp,float fFrecuency, float fPhase,float fMean, float iTimes, float fDelay);
    void startMultiLineal(char cLedColor, int iFinColor1, float t1, int iFinColor2, float t2,int iFinColor3, float t3,int iFinColor4, float t4, int iFinColor5, float t5 ,int iTimes, float Delay); 
    //void startReadFile(char cLedColor,char* _cFileName,float iTimes, float fDelay);
    
    void startWelder(char cLedColor, float fTime, float fDelay);
    
    /** functions */
    void step  (int *iIniColor, float *params,bool *bExitFlag);
    void lineal(int *iIniColor, float *params,bool *bExitFlag);
    void multiLineal(int *iIniColor, float *params,bool *bExitThread);
    void sigmoid(int *iIniColor, float *params,bool *bExitFlag);
    void sinusoidal(int *iIniColor, float *params,bool *bExitFlag);
    
    void readFile(int *iIniColor, float *params,char* cFileName,bool *bExitFlag);
    
    void welder(int *iIniColor, float *params,bool *bExitFlag);
    
    
    
        
    /** this function changes between the colors defined in rValue, gValue y bValue to the new colors defined as parameters in the function, 
     *   using a cubic interpolation for soft changes.
     * @Params int R, int G, unsigned B - New values for red, green and blue colors. Values between 0 and MAX_VALUES
     * @Param int nSteps - Number of transitions between old colors and new colors.
     * @Param int usDelay - Delay between transitions in miliseconds.
     */
    void cubicTo(int R,int G,int B, int nSteps, int msDelay); 
    
};

#endif