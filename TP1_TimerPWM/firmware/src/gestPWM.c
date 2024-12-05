/*--------------------------------------------------------*/
// GestPWM.c
/*--------------------------------------------------------*/
//	Description :	Gestion des PWM 
//			        pour TP1 2016-2017
//
//	Auteur 		: 	C. HUBER
//
//	Version		:	V1.1
//	Compilateur	:	XC32 V1.42 + Harmony 1.08
//
/*--------------------------------------------------------*/



#include "GestPWM.h"
#include "Mc32DriverLcd.h"
#include "bsp.h"
#include "app.h"
#include "Mc32DriverAdc.h"

S_pwmSettings PWMData;      // pour les settings

void GPWM_Initialize(S_pwmSettings *pData)
{
   // Init les data 
    //pData.absSpeed=0;    // vitesse 0 à 99
    //pData.absAngle=0;    // Angle  0 à 180
    //pData.SpeedSetting =0; // consigne vitesse -99 à +99
    //pData.AngleSetting =0;
    
   // Init état du pont en H
    STBY_HBRIDGE_W=1;
    BIN1_HBRIDGE_W=0;
    BIN2_HBRIDGE_W=0;
    AIN1_HBRIDGE_W=0;
    AIN2_HBRIDGE_W=0;
   // lance les tmers et OC
    DRV_TMR0_Start();  // start du timer 1          
            

}

// Obtention vitesse et angle (mise a jour des 4 champs de la structure)
void GPWM_GetSettings(S_pwmSettings *pData)	
{
    // Lecture du convertisseur AD
    appData.AdcRes.Chan0 = 0;
    // conversion
    //pData.absSpeed;    // vitesse 0 à 99
    //pData.absAngle;    // Angle  0 à 180
    //pData.SpeedSetting; // consigne vitesse -99 à +99
    //pData.AngleSetting; // consigne angle  -90 à +90
    
}


// Affichage des information en exploitant la structure
void GPWM_DispSettings(S_pwmSettings *pData)
{
    lcd_gotoxy(1,2);
   //printf_lcd("SpeedSetting: %3d", pData.SpeedSetting);
    lcd_gotoxy(1,3);
    //printf_lcd("AbsSpeed: %3d",  pData.absSpeed);
    lcd_gotoxy(1,4);
    //printf_lcd("AbsSpeed: %3d",  pData.absSpeed);
}

// Execution PWM et gestion moteur à partir des info dans structure
void GPWM_ExecPWM(S_pwmSettings *pData)
{
    
}

// Execution PWM software
void GPWM_ExecPWMSoft(S_pwmSettings *pData)
{
    
}


