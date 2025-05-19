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
#define TREE_SIZE 3














int main(void)
{  


    inituart();
    nrfx_systick_init();


    List list = create_empty_list();
   
    nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
    int size = Read_Int();


    int rtc_count = nrfx_rtc_counter_get(&rtc_instace)/32.768;
    srand((int)(rtc_count));  


    int *arr = malloc(size * sizeof(int));
    if (!arr) {
        char err[] = "Memory allocation failed\r\n";
        uarte_write(err, sizeof(err));
        return -1;
    }






    for (int i = 0; i < size; i++)
    {
        arr[i] = rand() % 10;




    }
        array_build(&list , arr , size);


       print_list(list);


    while  (1)
    {


     //  is_sorted(list)
        //Send_Int(arr[i]);


            char msg1[] =
                "\r\nVad vill du göra?\r\n"
                "1. Sortera bubble sort\r\n"
                "2. Kolla om den är sorterad\r\n"
                "3. Städa upp dompas siffror\r\n"
                "4. Sortera quick sort\r\n";
                uarte_write(msg1, sizeof(msg1));
        int val = Read_Int();
           
                switch (val)
                {
                    case 1:
                    {
                     
                        bubble(&list);
                      
               
                        print_list(list);




                        break;
                    }
                    case 2:
                    {
                      int sorterat = is_sorted(list);


                      Send_Int(sorterat);


                    }




                    case 3:
                    {
                       
                        clear_list(&list);


                        break;
                    }


                    case 4:
                    {
                      
                       

                        quicksort(arr, 0, size - 1);
                          


                       /*
                       for(int i=0; i < size; i++)
                        {
                            Send_Int(arr[i]);
                        }

                       */

                        break;

                    }


                    case 5:
                    {   
                          //  nrf_systick_val_clear();
                        //uint32_t pretime = nrf_systick_val_get();
                        insertionSort(arr , size);
                        ////uint32_t posttime = nrf_systick_val_get();

                        //int tid = pretime - posttime;

                        
                        //Send_Int(tid);
                        
                        for(int i=0; i < size; i++)
                        {
                            Send_Int(arr[i]);
                        }
                        
                        break;
                    }


                    case 6:
                    {
                       int sorterad = sorted(arr , size);

                       Send_Int(sorterad);

                    
                    }
   
                }
        free(arr);
    }






   
   








 }












         










