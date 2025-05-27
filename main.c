#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrf_gpio.h>
#include <nrfx_uarte.h>
#include <nrfx_systick.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
//#include "BSTree.c"
//#include "BSTree.h"
#include "grupp26.h"  // UART-funktioner
#include <stdlib.h>
#include "list.h"
#include "list.c"




extern void inituart(void);
extern void print_grid(void);
extern volatile int running;
extern nrfx_uarte_t instance;
extern char uarte_buffer;
extern volatile int val;
extern volatile int score;
extern volatile int point;
extern const nrfx_rtc_t rtc_instace;
int main(void)
{


    // Initiera UART och SysTick
    inituart();
    nrfx_systick_init();
    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    

   
    nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
  
    

    nrfx_uarte_rx(&instance, &uarte_buffer, 1);

    while (running)
    {
        int sek = nrfx_rtc_counter_get(&rtc_instace)/32768;


            

            

            
            if (sek >= 10)
            {
               break;
            }
     
            int rtc_count = nrfx_rtc_counter_get(&rtc_instace)/32.768;
            srand((int)(rtc_count));  

            val = rand() %10 + 1;

            printQuestion();
             
      

       

    }
    char msg1[64];
    sprintf(msg1, "Din tid är ute, du fick %d poäng.\n\r", score);
    uarte_write(msg1, strlen(msg1));

   
    


}










         










