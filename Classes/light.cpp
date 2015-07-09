#include "light.h"
extern Serial pc;   //This is used for access to Serial pc(USBTX, USBRX). It is used only for debuggin purposes.

/*Some mutex */
Mutex mutexR,mutexG,mutexB,mutexCreate,mutexFile;
Mutex mutexStartStep;
Mutex mutexStartSigmoid;


/* Constructors ******************************************************************************************/
light::light(PinName rPin, PinName gPin, PinName bPin): _rPin(rPin),_gPin(gPin),_bPin(bPin)
{
  //pc.printf("Inicio el constructor CLASE LIGHT----------------------\n\n\r");
  _rPin=0;
  _gPin=0;
  _bPin=0;
  
  /** Initialize thread exit states */
  bShow=false;
  bExitThread[RED]=false;
  bExitThread[GREEN]=false;
  bExitThread[BLUE]=false;
  
  /** Initialize Transition variable for each color */
  uiTransition[RED]=STEP;
  uiTransition[GREEN]=STEP;
  uiTransition[BLUE]=STEP;
  
  /** Initialize pointers */
  ledThread[RED]=NULL;
  ledThread[GREEN]=NULL;
  ledThread[BLUE]=NULL;
  
  /** Initialize variable for each color */
  
}

/*Setters Methods *******************************************************************************************/
void light::setLedColor(char cLedColor, int value)
{
    if(value>255)value=255;
    if(value<0)value=0;
    iValue[cLedColor]=value;
    bShow=false;
}

void light::setRGB(int R,int G, int B)  //Set RGB color in color variables
{
  setLedColor(RED,R);
  setLedColor(GREEN,G);
  setLedColor(BLUE,B);
}

void light::setfTBase(float time)
{
    fTBase=time;   
}
/* Getters Methods ******************************************************************************************/
int light::getLedColor(char cLedColor)     // Return the value of red color in the color variables
{
  return iValue[cLedColor];
}

int light::getThreadStatus(char cLedColor)
{
    if(ledThread[cLedColor]!=NULL)
        return ledThread[cLedColor]->get_state();
    return NULL;
}
/* General purpose functions ********************************************************************************/

void light::show()                  // Turn on the light with the colors set in the color variables
{
  _rPin.write((float)iValue[RED]/(float)MAX_LEVELS);
  _gPin.write((float)iValue[GREEN]/(float)MAX_LEVELS);
  //pc.printf("gPin.Write=%d/%d=%f\n\r",iValue[GREEN],MAX_LEVELS,iValue[GREEN]/(float)MAX_LEVELS);
  _bPin.write((float)iValue[BLUE]/(float)MAX_LEVELS);
  bShow=true;  
}

bool light::isShowed()              //Inform in the RGB register was read.
{
    return bShow;   
}

/** Threads methods for each color ********************************************************************************/
void light::createThread(char cLedColor)        //Create new Thread for cLedColor LED
{
    char flag=0;
    if(ledThread[cLedColor]==NULL)       //If pointer is create for first time
        {
            mutexCreate.lock();
            flag=1;
            mutexCreate.unlock();
            //pc.printf("      Thread NULL para LED=%d HILO=%p LUZ=%p\n\r",cLedColor,ledThread[cLedColor],this);   //only for debuggin
        }
    else if(isThreadInactive(cLedColor)) //If the thread was created but it is inactive
        {
            mutexCreate.lock();
            delete ledThread[cLedColor];
            bExitThread[cLedColor]=false;
            //pc.printf("      Thread INACTIVE para LED=%d, HILO=%p LUZ=%p bExitThread=%p\n\r",cLedColor,ledThread[cLedColor],this,&bExitThread[cLedColor]);//only for debuggin
            flag=1;
            mutexCreate.unlock();
        }
    if(flag==1)
    {
    switch(cLedColor){
        case RED:
                //pc.printf("ledThread[RED] status=%d\n\r",ledThread[cLedColor]->get_state());
                ledThread[cLedColor]=new Thread(&light::start_rThread,this);
                //pc.printf("  Thread creado para LED=%d, HILO=%p LUZ=%p\n\r",cLedColor,ledThread[cLedColor],this); //only for debuggin
                break;
        case GREEN:
                //pc.printf("ledThread[GREEN] status=%d\n\r",ledThread[cLedColor]->get_state());
                ledThread[cLedColor]=new Thread(&light::start_gThread,this);
                //pc.printf("  Thread creado para LED=%d HILO=%p LUZ=%p bExitThread=%d\n\r",cLedColor,ledThread[cLedColor],this, bExitThread[cLedColor]); //only for debuggin
                break;
        case BLUE:    
                //pc.printf("ledThread[BLUE] status=%d\n\r",ledThread[cLedColor]->get_state());
                ledThread[cLedColor]=new Thread(&light::start_bThread,this);
                //pc.printf("  Thread creado para LED=%d, HILO=%p LUZ=%p bExitThread=%d\n\r",cLedColor,ledThread[cLedColor],this, bExitThread[cLedColor]); //only for debuggin
                break;
                }          
    }
}

void light::start_rThread(void const *p)                    //start Red Thread
{   
    mutexR.lock();
    light *instance = (light*)p;
    mutexR.unlock();
    switch(instance->uiTransition[RED])
    {
    case STEP: 
        instance->step(&instance->iValue[RED],&instance->fTemp[RED][0],&instance->bExitThread[RED]);
        break;
    case LINEAL:
        //pc.printf("    He lanzado case LINEAL (RED)\n\r");
        instance->lineal(&instance->iValue[RED],&instance->fTemp[RED][0],&instance->bExitThread[RED]);
        break;
    case MULTILINEAL:
        //pc.printf("    He lanzado case MULTI_LINEAL (RED)\n\r");
        instance->multiLineal(&instance->iValue[RED],&instance->fTemp[RED][0],&instance->bExitThread[RED]);
        break;
     case SIGMOID:
        //pc.printf("    He lanzado case SIGMOID para LED=RED, HILO=%p bExitThread=%p\n\r",instance->ledThread[GREEN],&instance->bExitThread[BLUE]);        
        instance->sigmoid(&instance->iValue[RED],&instance->fTemp[RED][0],&instance->bExitThread[RED]);
        break;
    case SINUSOIDAL:
        instance->sinusoidal(&instance->iValue[RED],&instance->fTemp[RED][0],&instance->bExitThread[RED]);
        break;
     case WELDER:
        //pc.printf("    He lanzado case LINEAL (RED)\n\r");
        instance->welder(&instance->iValue[RED],&instance->fTemp[RED][0],&instance->bExitThread[RED]);
        break;
    case READFILE:
        //pc.printf("    He lanzado case LINEAL (RED)\n\r");
        instance->readFile(&instance->iValue[RED],&instance->fTemp[RED][0],instance->cFileName,&instance->bExitThread[RED]);
        break;
    }
      
   
}
void light::start_gThread(void const *p)                    //Start Green Thread
{
    mutexG.lock();
    light *instance = (light*)p;
    mutexG.unlock();   
    switch(instance->uiTransition[GREEN])
    {
    case STEP:
        //pc.printf("    He lanzado case STEP para LED=GREEN, HILO=%p\n\r",instance->ledThread[GREEN]);
        instance->step(&instance->iValue[GREEN],&instance->fTemp[GREEN][0],&instance->bExitThread[GREEN]);
        break;
    case LINEAL:
        //pc.printf("    He lanzado case LINEAL para LED=GREEN, HILO=%p\n\r",instance->ledThread[GREEN]);
        instance->lineal(&instance->iValue[GREEN],&instance->fTemp[GREEN][0],&instance->bExitThread[GREEN]);
        break;
    case MULTILINEAL:
        //pc.printf("    He lanzado case MULTI_LINEAL (GREEN)\n\r");
        instance->multiLineal(&instance->iValue[GREEN],&instance->fTemp[GREEN][0],&instance->bExitThread[GREEN]);
        break;
    case SIGMOID:
        //pc.printf("    He lanzado case SIGMOID para LED=GREEN, HILO=%p bExitThread=%p\n\r",instance->ledThread[GREEN],&instance->bExitThread[GREEN]);
        instance->sigmoid(&instance->iValue[GREEN],&instance->fTemp[GREEN][0],&instance->bExitThread[GREEN]);
        break;
    case SINUSOIDAL:
        instance->sinusoidal(&instance->iValue[GREEN],&instance->fTemp[GREEN][0],&instance->bExitThread[GREEN]);
        break;
    case WELDER:
        //pc.printf("    He lanzado case WELDER para LED=GREEN, HILO=%p \n\r",instance->ledThread[GREEN]);
        instance->welder(&instance->iValue[GREEN],&instance->fTemp[GREEN][0],&instance->bExitThread[GREEN]);
        break;
    case READFILE:
        //pc.printf("    He lanzado case LINEAL (RED)\n\r");
        instance->readFile(&instance->iValue[GREEN],&instance->fTemp[GREEN][0],instance->cFileName,&instance->bExitThread[GREEN]);
        break;
    }
 
}

void light::start_bThread(void const *p)                    //Start Blue Thread
{
    mutexB.lock();
    light *instance = (light*)p;
    mutexB.unlock();
    switch(instance->uiTransition[BLUE])
    {
    case STEP:
        instance->step(&instance->iValue[BLUE],&instance->fTemp[BLUE][0],&instance->bExitThread[BLUE]);
        //pc.printf("    He lanzado case STEP (BLUE)\n\r");
        break;
    case LINEAL:
        instance->lineal(&instance->iValue[BLUE],&instance->fTemp[BLUE][0],&instance->bExitThread[BLUE]);
        //pc.printf("    He lanzado case LINEAL (BLUE)\n\r");
        break;
    case MULTILINEAL:
        //pc.printf("    He lanzado case MULTI_LINEAL (BLUE)\n\r");
        instance->multiLineal(&instance->iValue[BLUE],&instance->fTemp[BLUE][0],&instance->bExitThread[BLUE]);
        break;
    case SIGMOID:
        //pc.printf("    He lanzado case SIGMOID para LED=BLUE, HILO=%p bExitThread=%p\n\r",instance->ledThread[GREEN],&instance->bExitThread[BLUE]);
        instance->sigmoid(&instance->iValue[BLUE],&instance->fTemp[BLUE][0],&instance->bExitThread[BLUE]);
        break;
    case SINUSOIDAL:
        instance->sinusoidal(&instance->iValue[BLUE],&instance->fTemp[BLUE][0],&instance->bExitThread[BLUE]);
        break;
    case WELDER:
        instance->welder(&instance->iValue[BLUE],&instance->fTemp[BLUE][0],&instance->bExitThread[BLUE]);
        break;
    case READFILE:
        //pc.printf("    He lanzado case LINEAL (RED)\n\r");
        instance->readFile(&instance->iValue[BLUE],&instance->fTemp[BLUE][0],instance->cFileName,&instance->bExitThread[BLUE]);
        break;
    }
}

void light::threadMessage(int iMessage,char cLedColor)      // Send signals to threads
{
    ledThread[cLedColor]->signal_set(iMessage);    
}

void light::exitAllThreads()                                //Abort all threads
{      
       bExitThread[RED]=true;
       bExitThread[GREEN]=true;
       bExitThread[BLUE]=true;
}

bool light::isThreadInactive(char cLedColor)
{
    int iState=ledThread[cLedColor]->get_state();
    if(iState == Thread::Inactive)
        return true;
    else
        return false;
}
 bool light::isThreadAvailable(char cLedColor)
 { 
    //pc.printf("ledThread[%d]=%p status=%d\n\r",cLedColor,ledThread[cLedColor],ledThread[cLedColor]->get_state());
    if((ledThread[cLedColor]->get_state()==Thread::Inactive)||(ledThread[cLedColor]==NULL))
        return true;
    else
        return false;  
 }

/** Base interactions ******************************************************************** Base interactions **/


/** STEP */
void light::startStep(char cLedColor,int iFinColor, float fTDown,float fTUp, int iTimes)  //Parameterized Step Function
{
    //mutexStartStep.lock();
    //pc.printf("He llamado a starStep\n\r");
    fTemp[cLedColor][0]=iFinColor;      //Final color
    fTemp[cLedColor][1]=fTDown;         //This is the time in down state
    fTemp[cLedColor][2]=fTUp;           //This is the time in up state
    fTemp[cLedColor][3]=iTimes;         //This is the number of times That process is executed 
    //fTemp[cLedColor][5]=delay;           //This is the delay before the routine run
    uiTransition[cLedColor]=STEP;
    //pc.printf("ledThread[%d] status en startStep=%d\n\r",cLedColor,ledThread[cLedColor]->get_state());
    createThread(cLedColor);
    //mutexStartStep.unlock();
    //pc.printf("START_STEP lanzado con LED=%d, HILO=%p LUZ=%p\n\r",cLedColor,ledThread[cLedColor],this);
    //pc.printf("ledThread[%d] creado en startStep. Dir=%p \n\r",cLedColor,ledThread[cLedColor]);\
    
}

void light::step(int *iIniColor, float *params,bool *bExitThread)       //Step Function
{
    //pc.printf("      Estoy ejecutando STEP para LUZ=%p\n\r",this);
    //pc.printf("        Estoy en STEP bExitThread=%d dir=%p\n\r",*bExitThread,bExitThread);
    Mutex mutexStep;
    mutexStep.lock();
    long int iPulsesDown=(long int)(params[1]/fTBase);
    long int iPulsesUp=(long int)(params[2]/fTBase);
    long int iPulsesDownTemp=iPulsesDown;
    long int iPulsesUpTemp=iPulsesUp;
    int temp=*iIniColor;
    float fTempNroIte;
    mutexStep.unlock();
    if(params[3]<0)
        fTempNroIte=1;
    else
        fTempNroIte=params[3];
    //pc.printf("      Estoy ejecutando STEP-2 para LUZ=%p\n\r",this);
    while((!*bExitThread)&&(fTempNroIte>0))
    { 
        //pc.printf("*bExitThread %d\n\r",*bExitThread);
        mutexStep.lock();
        iPulsesDownTemp=iPulsesDown;
        iPulsesUpTemp=iPulsesUp;
        *iIniColor=temp;
        //*(iIniColor+1)=*iIniColor;  //Solo para el experimento
        //*(iIniColor+2)=*iIniColor;  //Solo para el experimento
        mutexStep.unlock();
        //pc.printf("      Estoy ejecutando STEP-3 para LUZ=%p\n\r",this);
        while((!*bExitThread)&&(iPulsesDownTemp>0))
            {
                Thread::signal_wait(0x1);
                //pc.printf("Ejecutando hilo step-tDown... %ld\n\r",iPulsesDown);
                iPulsesDownTemp--;
            }   
        mutexStep.lock();
        *iIniColor=params[0];   
        //*(iIniColor+1)=*iIniColor;  //Solo para el experimento
        //*(iIniColor+2)=*iIniColor;  //Solo para el experimento
        mutexStep.unlock();
        //pc.printf("      Estoy ejecutando STEP-4 para LUZ=%p\n\r",this);
        while((!*bExitThread)&&(iPulsesUpTemp>0))
            {
                Thread::signal_wait(0x1);
                //pc.printf("Ejecutando hilo step-tUp... %ld\n\r",iPulsesUpTemp);
                //mutexStep.lock();
                iPulsesUpTemp--;
                //mutexStep.unlock();
            }   
    //pc.printf("      Estoy ejecutando STEP-5 para LUZ=%p\n\r",this);
    if(params[3]>0)fTempNroIte--;
    }
    *iIniColor=temp;
    //*(iIniColor+1)=*iIniColor;  //Solo para el experimento
    //*(iIniColor+2)=*iIniColor;  //Solo para el experimento
    //pc.printf("-->Estoy finalizando STEP para LUZ=%p\n\r", this); 
}

/** LINEAL */
void light::startLineal(char cLedColor,int iFinColor, float t, int iTimes)
{
    fTemp[cLedColor][0]=iFinColor;
    fTemp[cLedColor][1]=t;
    fTemp[cLedColor][2]=iTimes;
    ///fTemp[cLedColor][3]=fTBase;
    
    uiTransition[cLedColor]=LINEAL;
    //pc.printf("ledThread[%d] status en startLineal=%d\n\r",cLedColor,ledThread[cLedColor]->get_state());
    createThread(cLedColor);
    //pc.printf("START_LINEAL lanzado con LED=%d, HILO=%p LUZ=%p\n\r",cLedColor,ledThread[cLedColor],this);
    //pc.printf("ledThread[%d] creado en startLineal. Dir=%p \n\r",cLedColor,ledThread[cLedColor]);

}

void light::lineal(int *iIniColor, float *params,bool *bExitThread)
{
    //pc.printf("      Estoy ejecutando LINEAL para LUZ=%p\n\r",this);
    long int iPulses=(long int)(params[1]/fTBase);
    float fTempNroIte;
    int temp=*iIniColor;
    
    /** Calculate the constants (a & b) of a + bt*/
    float a=(float)(*iIniColor);
    float b=(params[0]-a)/params[1];
    
    if(params[2]<0)             //it is used for the infinite case. (When the number of iterations is -1)
        fTempNroIte=1;
    else
        fTempNroIte=params[2];
        
    long int i;
    
    while((!*bExitThread)&&(fTempNroIte>0))
        {
            i=0;
            
            *iIniColor=temp;
            while((!*bExitThread)&&(i<=iPulses))
                {
                    Thread::signal_wait(0x1);
                    //pc.printf("Estoy luego del signal wait. iPulses=%ld  i=%ld color=%d\n\r",iPulses,i,*iIniColor);
                    *iIniColor=(int)(a+b*(fTBase*i));
                    //pc.printf("Color(%ld)=%d\n\r",(long int)params[3]*i,*iIniColor);
                    i++;
                }
            if(params[2]>0)fTempNroIte--;
        }
}

void light::startMultiLineal(char cLedColor, int iFinColor1, float t1, int iFinColor2, float t2,int iFinColor3, float t3,int iFinColor4, float t4, int iFinColor5, float t5,int iTimes, float fDelay)
{
    //pc.printf("\nEstoy iniciando START_MULTILINEAL para LED[%d] LUZ=%p\n\r", cLedColor,this);
    fTemp[cLedColor][0]=0;
    fTemp[cLedColor][1]=0;
    fTemp[cLedColor][2]=iTimes;
    fTemp[cLedColor][3]=fDelay;
    fTemp[cLedColor][4]=0;          //reserved
    fTemp[cLedColor][5]=0;          //reserved
    fTemp[cLedColor][6]=iFinColor1;
    fTemp[cLedColor][7]=t1;
    fTemp[cLedColor][8]=iFinColor2;
    fTemp[cLedColor][9]=t2;
    fTemp[cLedColor][10]=iFinColor3;
    fTemp[cLedColor][11]=t3;
    fTemp[cLedColor][12]=iFinColor4;
    fTemp[cLedColor][13]=t4;
    fTemp[cLedColor][14]=iFinColor5;
    fTemp[cLedColor][15]=t5;
    
    uiTransition[cLedColor]=MULTILINEAL;
    //pc.printf(" StartMultilinea-> ledThread[%d] STATUS==%d LIGHT=%p\n\r",cLedColor,ledThread[cLedColor]->get_state(),this);
    createThread(cLedColor);
    //pc.printf(" StartMultilinea-> ledThread[%d] creado. Dir=%p LIGHT=%p\n\r",cLedColor,ledThread[cLedColor],this);
}

void light::multiLineal(int *iIniColor, float *params,bool *bExitThread)
{
    //pc.printf("      Estoy ejecutando MULTI_LINEAL para LUZ=%p\n\r",this);  //only for debugging
    
    // Delay segment
    long int iDelay=(long int)(params[3]/fTBase);
    while((!*bExitThread)&&(iDelay>0))
        {
           Thread::signal_wait(0x1);
           iDelay--;
        }
    //pc.printf("      Estoy ejecutando MULTI_LINEAL (Break Point 1) para LUZ=%p\n\r",this);  //only for debugging
    float fTempNroIte,a,b;
    int temp=*iIniColor;
    long int iPulses,i;    
    char j;
    params[4]=*iIniColor;       //Initial point
    params[5]=0;
    
    
    if(params[2]<0)             //it is used for the infinite case. (When the number of iterations is -1)
        fTempNroIte=1;
    else
        fTempNroIte=params[2];

    //pc.printf("      Estoy ejecutando MULTI_LINEAL (Break Point 2) para LUZ=%p\n\r",this);  //only for debugging
    while((!*bExitThread)&&(fTempNroIte>0))
        {
           params[4]=temp;
           j=0;
           //pc.printf("      Estoy ejecutando MULTI_LINEAL (Break Point 3) para LUZ=%p, j=%d\n\r",this,j);  //only for debugging
           while((!*bExitThread)&&(j<9))
            {
                //iPulses=(long int)((params[j+7]-params[j+5])/fTBase);
                iPulses=(long int)((params[j+7])/fTBase);
                i=0;
                a=(float)(params[j+4]);
                if(params[j+7]==0)
                    b=0;
                else
                    b=(params[j+6]-params[j+4])/(params[j+7]);
                //pc.printf("Estoy ejecutando mlineal con iPulses=%ld  j=%ld color=%d a=%f b=%f \n\r",iPulses,j,params[j+4],a,b);
                
                while((!*bExitThread)&&(i<=iPulses))
                    {
                        Thread::signal_wait(0x1);
                        //pc.printf("Estoy luego del signal wait. iPulses=%ld  i=%ld color=%d\n\r",iPulses,i,*iIniColor);
                        *iIniColor=(int)(a+b*(fTBase*i));
                        //pc.printf("Color(%ld)=%d\n\r",(long int)params[3]*i,*iIniColor);
                        i++;
                    }   
                j+=2;
            }
            if(params[2]>0)fTempNroIte--;
        }
    //pc.printf("Termine MULTILINEAL....\n\r");
}
/* Sigmoid funcion */
void light::startSigmoid(char cLedColor,int iFinColor, float fMean, float fSigma, float iTime, int iTimes, float fDelay)
{
    //pc.printf("\nEstoy iniciando START_SIGMOID para LED[%d] LUZ=%p\n\r", cLedColor,this);
    //mutexStartSigmoid.lock();
    fTemp[cLedColor][0]=iFinColor;
    fTemp[cLedColor][1]=iTime;
    fTemp[cLedColor][2]=iTimes;
    fTemp[cLedColor][3]=fDelay;
    fTemp[cLedColor][4]=fMean;
    if(fMean<0)fTemp[cLedColor][4]=iTime/2;
    fTemp[cLedColor][5]=fSigma;
    uiTransition[cLedColor]=SIGMOID;
    //pc.printf(" StartSigmoid-> ledThread[%d] STATUS==%d LIGHT=%p\n\r",cLedColor,ledThread[cLedColor]->get_state(),this);
    createThread(cLedColor);
    //mutexStartSigmoid.unlock();
    //pc.printf(" StartSigmoid-> ledThread[%d] creado. Dir=%p LIGHT=%p\n\r",cLedColor,ledThread[cLedColor],this);
}

void light::sigmoid(int *iIniColor, float *params,bool *bExitThread){
    //pc.printf("        Estoy iniciando SIGMOID para LUZ=%p\n\r", this);                         //only for debugging
    //pc.printf("        Estoy en SIGMOID bExitThread=%d dir=%p\n\r",*bExitThread,bExitThread);   //only for debugging
    Mutex miMutex;
    
    long int iPulses;
    float fTempNroIte;          //This variable is used to make the process several times (iTimes)
    //int temp;
    long int iDelay;
    long int i;
    double num,deno;

    miMutex.lock();
    iPulses=(long int)(params[1]/fTBase);
    //temp=*iIniColor;
    iDelay=(long int)(params[3]/fTBase);
    miMutex.unlock();
    
    //pc.printf("        Estoy en SIGMOID bExitThread=%d fTempNroIte=%f iIniColor=%d FinColor=%f \n\r",*bExitThread,fTempNroIte,*iIniColor,params[0]);
    
    if(params[2]<0)             //it is used for the infinite case. (When the number of iterations is -1)
        fTempNroIte=1;
    else
        fTempNroIte=params[2];
        
    /** Wait for time delay */
     while((!*bExitThread)&&(iDelay>0))
        {
           Thread::signal_wait(0x1);
           miMutex.lock();
           iDelay--;
           miMutex.unlock();
        }   
    
    /** Function */
    //pc.printf("        Estoy en SIGMOID bExitThread=%d fTempNroIte=%f iIniColor=%d FinColor=%f \n\r",*bExitThread,fTempNroIte,*iIniColor,params[0]);
    while((!*bExitThread)&&(fTempNroIte>0))
        {
            
            i=0;
            //miMutex.lock();
            //*iIniColor=temp;
            //miMutex.unlock();
            
            while((!*bExitThread)&&(i<=iPulses))
                {  
                    Thread::signal_wait(0x1);
                    miMutex.lock();
                    num=-pow((double)(i*fTBase-params[4]),(double)2);
                    deno=2*pow((double)params[5],(double)2);
                    *iIniColor=(int)(params[0]*exp(num/deno));
                    //*(iIniColor+1)=*iIniColor;  //Solo para el experimento
                    //*(iIniColor+2)=*iIniColor;  //Solo para el experimento
                    
                    
                    miMutex.unlock();
                    //pc.printf("i=%d num=%lf deno=%lf iIniColor=%d\n\r",i,num,deno,*iIniColor);
                    i++;
                } 
            if(params[2]>0){
                fTempNroIte--;
                }
        }
 
    //pc.printf("-->Estoy finalizando SIGMOID para LUZ=%p\n\r", this);    
}
    
/** Sinusoidal function */
void light::startSinusoidal(char cLedColor, int iFinColor, float fValue_pp,float fFrecuency, float fPhase,float fMean, float iTimes, float fDelay)
{
    fTemp[cLedColor][0]=iFinColor;
    //fTemp[cLedColor][1]=iTime;
    fTemp[cLedColor][2]=iTimes;
    fTemp[cLedColor][3]=fDelay;
    fTemp[cLedColor][4]=fValue_pp;
    fTemp[cLedColor][5]=fFrecuency;
    fTemp[cLedColor][6]=fPhase;
    fTemp[cLedColor][7]=fMean;
    uiTransition[cLedColor]=SINUSOIDAL;
    //pc.printf(" StartSinusoidal-> ledThread[%d] STATUS==%d LIGHT=%p\n\r",cLedColor,ledThread[cLedColor]->get_state(),this);
    createThread(cLedColor);    
}

void light::sinusoidal(int *iIniColor, float *params,bool *bExitFlag)
{
    Mutex miMutex;
    
    //miMutex.lock();
    long int iPulses=(long int)((1/params[5])/fTBase);
    float fTempNroIte;          //This variable is used to make the process several times (iTimes)
    long int iDelay=(long int)(params[3]/fTBase);  // Wait for time delay 
    //miMutex.unlock();
    //int temp=*iIniColor;
    
    if(params[2]<0)             //it is used for the infinite case. (When the number of iterations is -1)
        fTempNroIte=1;
    else
        fTempNroIte=params[2];
            
     
     while((!*bExitThread)&&(iDelay>0))
        {
           Thread::signal_wait(0x1);
           iDelay--;
        }   
   
    long int i;
    
    while((!*bExitThread)&&(fTempNroIte>0))
        {
            i=0;
            //*iIniColor=temp;
            while((!*bExitThread)&&(i<=iPulses))
                {
                    Thread::signal_wait(0x1);
                    miMutex.lock();
                    *iIniColor=(int)(params[7]+params[4]*sin(2*PI*params[5]*(i*fTBase)+params[6]));
                    
                                       
                    miMutex.unlock();
                    i++;
                    //pc.printf("iIniColor=%d time=%d\r\n",*iIniColor,i*fTBase);
                } 
            if(params[2]>0)fTempNroIte--;
        }
   
}
    
/** Read File */
/** Sinusoidal function */
/*void light::startReadFile(char cLedColor,char* _cFileName,float iTimes, float fDelay)
{
    //pc.printf(" StartSinusoidal-> ledThread[%d] STATUS==%d LIGHT=%p\n\r",cLedColor,ledThread[cLedColor]->get_state(),this);
    cFileName=_cFileName;
    fTemp[cLedColor][2]=iTimes;
    fTemp[cLedColor][3]=fDelay;
    uiTransition[cLedColor]=READFILE;
    createThread(cLedColor);    
}
*/
void light::readFile(int *iIniColor, float *params,char* cFileName,bool *bExitFlag)
{
    
   
    float fTempNroIte;          //This variable is used to make the process several times (iTimes)
    mutexFile.lock();
    long int iDelay=(long int)(params[3]/fTBase);  // Wait for time delay 
    //FILE *data=fopen(cFileName,"r");
    FILE *data=fopen(cFileName,"r");
    
    mutexFile.unlock();
    float dato;
    //int temp=*iIniColor;
    
    
    if(params[2]<0)             //it is used for the infinite case. (When the number of iterations is -1)
        fTempNroIte=1;
    else
        fTempNroIte=params[2];
            
     while((!*bExitThread)&&(iDelay>0))
        {
           Thread::signal_wait(0x1);
           iDelay--;
        }   
   
     while((!*bExitThread)&&(fTempNroIte>0))
        {
            fseek(data,0,SEEK_SET);
            while((!*bExitThread)&&(!feof(data)))
                {
                    Thread::signal_wait(0x1);
                    mutexFile.lock();
                    fscanf(data, "%f",&dato);
                    *iIniColor=(int)dato;
                    fscanf(data, "%f",&dato);
                    *(iIniColor+1)=(int)dato;
                    fscanf(data, "%f",&dato);
                    *(iIniColor+2)=(int)dato;
                    mutexFile.unlock();
                    
                    //pc.printf("iIniColor=%d time=%d\r\n",*iIniColor,i*fTBase);
                } 
            if(params[2]>0)fTempNroIte--;
        }
    fclose(data);
}


/** Special Effects Functions */

/** Soldering */
void light::startWelder(char cLedColor, float fTime, float fDelay){
    //fTemp[cLedColor][0]=iFinColor;
    cLedColor=RED;                      //allways we use the RED thread for this effect.
    fTemp[cLedColor][1]=fTime;
    fTemp[cLedColor][2]=fDelay;
    uiTransition[cLedColor]=WELDER;
    createThread(cLedColor);
}

void light::welder(int *iIniColor, float *params,bool *bExitFlag){
    long int iPulses=(long int)(params[1]/fTBase);
    float fTempNroIte;          //This variable is used to make the process several times (iTimes)
    int temp=*iIniColor;
    
    if(params[2]<0)             //it is used for the infinite case. (When the number of iterations is -1)
        fTempNroIte=1;
    else
        fTempNroIte=params[2];
            
    long int i,pause;
    while((!*bExitThread)&&(fTempNroIte>0))
        {
            i=0;
            *iIniColor=temp;
            while((!*bExitThread)&&(i<=iPulses))
                {
                    Thread::signal_wait(0x1);
                    *iIniColor=(int)(rand()%125)*2;
                    *(iIniColor+1)=*iIniColor;
                    *(iIniColor+2)=*iIniColor;
                    i++;
                     
                    pause=(int)(((rand()%50)/1000)/fTBase);  //random pause. Max pause 50mS
                    while((!*bExitThread)&&(pause>0))
                        {
                            Thread::signal_wait(0x1);
                            pause--;
                            i++;
                        }
                } 
            if(params[2]>0)fTempNroIte--;
        }
}


//* Basic estructure for a function */
/*void light::sigmoid(int *iIniColor, float *params,bool *bExitFlag){
    long int iPulses=(long int)(params[1]/fTBase);
    float fTempNroIte;          //This variable is used to make the process several times (iTimes)
    int temp=*iIniColor;
    
    if(params[2]<0)             //it is used for the infinite case. (When the number of iterations is -1)
        fTempNroIte=1;
    else
        fTempNroIte=params[2];
            
     long int iDelay=(long int)(params[3]/fTBase);  // Wait for time delay 
     while((!*bExitThread)&&(iDelay>0))
        {
           Thread::signal_wait(0x1);
           iDelay--;
        }   
   
    long int i;
    
    while((!*bExitThread)&&(fTempNroIte>0))
        {
            i=0;
            *iIniColor=temp;
            while((!*bExitThread)&&(i<=iPulses))
                {
                    i++;

                } 
            if(params[2]>0)fTempNroIte--;
        }
}
    

}*/