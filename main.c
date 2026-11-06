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
	GPIOI->MODER |= (1 << 0); // Mode du broche PI0 en General purpose OUTPUT
	
	RCC->APB1ENR |= (1 << 29); // Activation du périphérque DAC sur la pin 29
	RCC->APB1ENR |= (3 << 8); // Mode du broche PA4 (DAC) en mode ANALOG sur la pin 8
	DAC->CR |= (1 << 0); // Activation de la voie du DAC (EN1 car DAC_OUTPUT1) 
	
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