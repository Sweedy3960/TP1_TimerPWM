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
#include "C:\microchip\harmony\v2_06\framework\peripheral\oc\plib_oc.h"

 
void GPWM_Initialize(S_pwmSettings *pData)
{
   // Init les data 
    pData->absSpeed=0;   // vitesse 0 à 99
    pData->absAngle=0;    // Angle  0 à 180
    pData->SpeedSetting=0; // consigne vitesse -99 à +99
    pData->AngleSetting=0;
   // Init état du pont en H
   BSP_EnableHbrige();
    // lance les tmers et OC
   DRV_TMR0_Start();  // start du timer 1 
   DRV_TMR1_Start();
   DRV_TMR2_Start();
   DRV_TMR3_Start();
   DRV_OC0_Start();
   DRV_OC1_Start();
  
   

 
}
 
// Obtention vitesse et angle (mise a jour des 4 champs de la structure)
void GPWM_GetSettings(S_pwmSettings *pData)	
{
    S_ADCResults AdcRes;
    // Lecture du convertisseur AD
    AdcRes = BSP_ReadAllADC();
    pData->SpeedSetting = ((abs(((float)Sweepingmoy(&AdcRes,0)/MAXVALAD)*ORDONEEPRG))-OFFSETORIG);
    //pData->SpeedSetting = ((abs(((float)AdcRes.Chan0/1023)*198))-99);    // vitesse 0 à 99
    pData->absSpeed =abs( pData->SpeedSetting);
   // pData.absAngle;    // Angle  0 à 180
    //pData.AngleSetting; // consigne angle  -90 à +90
}
int Sweepingmoy(S_ADCResults *AdcRes,int chan)
{
    static int buff[2][11] = {{0}}; 
    static int iterator = 0;
    //static int firstTimeSincePowerUp = 0;
    int i;

    // Stocker la valeur dans le buffer adéquat
    buff[chan][iterator] = (chan) ? AdcRes->Chan0 : AdcRes->Chan1;

    // Incrémenter l'itérateur et gérer le retour à zéro
    iterator++;
    if (iterator == 10)
    {
        iterator = 0;
        //firstTimeSincePowerUp = 1; 
    }

    // Calculer la moyenne si buffer full
    //if (firstTimeSincePowerUp)
    //{
        buff[chan][10] = 0; // Réinitialiser la somme avant de la recalculer
        for (i = 0; i < 10; i++) // On doit parcourir 10 éléments
        {
            buff[chan][10] += buff[chan][i]; // Somme des 10 valeurs
        }
    //}
    return (buff[chan][10] / 10);//(firstTimeSincePowerUp) ? (buff[chan][10] / 10) : 0;
}
 
// Affichage des information en exploitant la structure
void GPWM_DispSettings(S_pwmSettings *pData)
{
    
    lcd_gotoxy(1,2);
    printf_lcd("SpeedSetting: %4d", pData->SpeedSetting);
    lcd_gotoxy(1,3);
    //lcd_ClearLine(3);
    printf_lcd("AbsSpeed: %3d",  pData->absSpeed);
    lcd_gotoxy(1,4);
    
}
 
// Execution PWM et gestion moteur à partir des info dans structure
void GPWM_ExecPWM(S_pwmSettings *pData)
{
  //Fait tourner le moteur dans le sens horaire 
    if(pData->SpeedSetting > 0)
    {
        AIN1_HBRIDGE_W = 1;
        AIN2_HBRIDGE_W = 0;
    }
    //Fait tourner le moteur dans le sens antihoraire 
    else if (pData->SpeedSetting < 0)
    {
        AIN1_HBRIDGE_W = 0;
        AIN2_HBRIDGE_W = 1;
    }
    //Fait que le moteur ne tourne pas
    else 
    {
        STBY_HBRIDGE_W = 0; 
    }
    
    //Calcul de la largueur d'impulsion pour le moteur DC
    PLIB_OC_PulseWidth16BitSet(OC_ID_2, (pData->absSpeed*VAL_MAX_TIMER2)/DIVISION);
    //Calcul de la largeur d'impulsion pour le servomoteur 
    PLIB_OC_PulseWidth16BitSet(OC_ID_3, (((pData->absAngle*DELTA_VALEUR_LARGEUR_IMPULSION)/ANGLE_ABS)+VAL_LARGEUR_IMPULSION_06));
}
 
// Execution PWM software
void GPWM_ExecPWMSoft(S_pwmSettings *pData)
{
    static uint8_t compteurRpc = 0;

    // Si la valeur absolue de la vitesse est supérieure au compteurRpc, éteindre la LED
    if (pData->absSpeed > compteurRpc)
    {
        BSP_LEDOff(BSP_LED_2);
    }
    else
    {
        // Sinon, allumer la LED
        BSP_LEDOn(BSP_LED_2);
    }

    // Incrémente le compteurRpc
    compteurRpc++;

    // Remet à zéro le compteurRpc s'il dépasse la valeur 99
    if (compteurRpc >= 99)
    {
        compteurRpc = 0;
    }
}
