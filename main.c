 #include "stm32f7xx.h"                  // Device header

extern void configuration_overclock(void);
extern int Init_GUIThread (void);
extern void Init_Ethernet (void);

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif
#ifdef RTE_CMSIS_RTOS2                  // when RTE component CMSIS RTOS2 is used
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#endif

// Prototypes des fonctions

void init_servo(void);
void init_sensor_ultrasons(void);
void servo_set_angle(int deg);
void distance_cm(void);

/*----------------------------------------------------------------------------
 *      Tâche de test (vide) : à compléter par les étudiants
 *---------------------------------------------------------------------------*/
osThreadId_t tid_Test;

static void Thread_Test (void *argument)
{
  (void)argument;

  // Initialisations propres à la tâche (si besoin) ici

  while (1)
  {
    // Votre boucle de test ici (mesure de distance, balayage du servo, etc.)

		// Debut balayage 
		
		for(int i=0; i<180; i++)
		{
			servo_set_angle(i);
			osDelay(100); 
		}
		
		for(int i=180; i>0; i--)
		{
			servo_set_angle(i);
			osDelay(100); 
		}
		
		// Fin balayage
		
		
		
    osDelay(100);   // période de la boucle : 100 ms
  }
}

static int Init_TestThread (void)
{
  tid_Test = osThreadNew(Thread_Test, NULL, NULL);
  if (tid_Test == NULL) {
    return -1;
  }
  return 0;
}


int main(void)
{
  configuration_overclock(); //pour pouvoir utiliser l'écran LCD

	/**********************************
	Début Initialisation périphérique
	**********************************/

	//Initialiser vos périphériques ici
	
	RCC->AHB1ENR |= (1 << 8); // Activation du périphérique GPIOI
	GPIOI->MODER |= (1 << 0) | (1 << 4); // Mode du broche PI0 et PI2 en General purpose OUTPUT  
	
	RCC->APB1ENR |= (1 << 29); // Activation du périphérque DAC sur la pin 29
	RCC->APB1ENR |= (3 << 8); // Mode du broche PA4 (DAC) en mode ANALOG sur la pin 8
	DAC->CR |= (1 << 0); // Activation de la voie du DAC (EN1 car DAC_OUTPUT1) 
	
	init_servo();
	init_sensor_ultrasons();
	
	RCC->AHB1ENR |= (1 << 5);	// Activation du GPIOF
	RCC->APB2ENR |= (1 << 17); // Activation du TIM10_CH1
	GPIOF->MODER |= (2 << 12); // PF6 en AF mode
	GPIOF->AFR[0] |= (3 << 24); // PF6 en AF3
	TIM10->CCMR1 |= (6 << 4); //
	TIM10->CCER |= (1 << 0);
	TIM10->PSC = 7;
	TIM10->ARR = 39999;
	TIM10->CR1 |= (1 << 0); // Demarrage du compteur

	/**********************************
	Fin Initialisation périphérique
	**********************************/

	/**********************************
	Utilisation RTOS (Keil RTX5)
	**********************************/
	#ifdef RTE_CMSIS_RTOS2
	  /* Initialize CMSIS-RTOS2 */
	  osKernelInitialize ();

	  /* Create thread functions that start executing */
		Init_Ethernet();
		Init_GUIThread();
		Init_TestThread();   // tâche de test (boucle avec osDelay)

	  /* Start thread execution */
	  osKernelStart();
	#endif

  /* Infinite loop  (ne s'exécute pas avec le RTOS!!!)*/
  while (1)
  {
  }
}

// Codes des fonctions

void init_servo(void) // A3
{
	RCC->AHB1ENR |= (1 << 5); // Activation du GPIOF
	RCC->APB1ENR |= (1 << 7); // Activation du Timer 13 
	
	GPIOF->MODER |= (2 << 16); // Configuration du mode en alternate
	GPIOF->AFR[1] |= (9 << 0); // Activation, du AF9
	
	TIM13->CCMR1 |= (6 << 4);
	TIM13->CCER |= (1 << 0);
	
	TIM13->PSC = 31; // Après verif,
	TIM13->ARR = 62499;
	TIM13->CCR1 = 1500;
	
	TIM13->CR1 |= (1 << 0); //Demarrage du compteur 
}

void init_sensor_ultrasons(void) // D12
{
	RCC->AHB1ENR |= (1 << 1); // Activation du GPIOB
	RCC->APB1ENR |= (1 << 6); // Activation du TIM12
	
	GPIOB->MODER |= (2 << 28); // Configuration en mode alternate
	GPIOB->AFR[1] |= (9 << 24); // Activation du AF9
	
	TIM12->CCMR1 |= (6 << 4);
	TIM12->CCER |= (1 << 0);
	
	TIM12->PSC = 39;
	TIM12->ARR = 62499;
	TIM12->CCR1 = 25;
	TIM12->CR1 |= (1 << 0); // Demarrage du compteur 
}


void servo_set_angle(int deg)
{
	int result; 
	
	if(deg < 0) deg = 0;
	if(deg > 180) deg = 180;
	
	result = 3125 + (deg*3125) / 180;
	
	TIM13->CCR1 = result;
}

void distance_cm(void)
{
}