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
#include <grupp26.h>
#include <assert.h> //kom ihåg att inkludera detta om ni ska använda asserts
#include <stdio.h>

uint8_t uarte_enabled = 0;
volatile int running = 1;
volatile int val;
volatile int score = 0;
volatile int point = 0 ;

volatile int user_answer_ready = 0;
volatile int user_answer = 0;

void __assert_func(const char *file, int line, const char *func, const char *expr)
{
    if(uarte_enabled)
    {
        char errmsg[100];
        sprintf(errmsg, "Error in function %s, line %d. Expression: %s", func, line, expr);
        uarte_write(errmsg, strlen(errmsg));

        __asm("bkpt 1");
    }
}





nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

const nrfx_rtc_t rtc_instace = NRFX_RTC_INSTANCE(0);
char uarte_buffer;


void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
        char recived = p_event->data.rx.p_data[0];
        
        if(recived == '1' || recived == '2')
        {
            user_answer = recived - '0';
            user_answer_ready = 1;
        }

        nrfx_uarte_rx(&instance, &uarte_buffer, 1);
    
    }
}   

void inituart(void){

  const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(20, 22);
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, uarte_handler);
    if (errr == NRFX_SUCCESS)
    {
      uarte_enabled = 1;
    }

    nrfx_rtc_config_t config_rtc = NRFX_RTC_DEFAULT_CONFIG;
    
    nrfx_err_t errr_rtc = nrfx_rtc_init (&rtc_instace, &config_rtc, NULL);
    nrfx_rtc_enable(&rtc_instace);


}



void uarte_write(char* data, int length)
{
    nrfx_uarte_tx(&instance, data, length, 0);
    while(nrfx_uarte_tx_in_progress(&instance)){};
}


void Read_String(char *data, int max_length)
{
    int index = 0;
    char received_char;


    while (index < max_length - 1) // lämna plats för '\0' i slutet
    {
        // Läs ett tecken via UARTE
        nrfx_uarte_rx(&instance, &received_char, sizeof(received_char));


        // Om tecknet är carriage return, avsluta läsningen
        if (received_char == '\r')
        {
            break;
        }


        // Lägg till det mottagna tecknet i strängen
        data[index++] = received_char;
    }


    // Lägg till null terminator för att avsluta strängen
    data[index] = '\0';


}


int Read_Int( void )//char *data, int max_length)
{
    char buffer[20];
    Read_String(buffer , sizeof(buffer));
   
   
    return  atoi(buffer);
     


}


void Send_Int(int value)
{
    char output_buffer[32];


    sprintf(output_buffer, "%d" , value);


    uarte_write(output_buffer, strlen(output_buffer));






}

void bubble_sort(int *arr ,int size)
{
    
    int temp;

    for (int i = 0; i < size - 1; i++)
    {
        temp = 0;
        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1]){

            int temp2 = arr[j];
            arr[j] = arr[j+1];
            arr[j + 1] = temp2;
            temp = 1;
            }
        }

        if (!temp){

            break;
        }
    }

}
void quicksort(int arr[], int low, int high)
{
    if(low < high)
    {

    int pivotindex = partition(arr, low, high);

    quicksort(arr, low, pivotindex - 1);
    quicksort(arr, pivotindex + 1, high);
    }
    

}

int partition(int arr[], int low, int high)
{
    int pivot = arr[low];
    int dividor = low + 1; //minsta högre än pivot

    for(int i = low + 1; i <= high; i++)
    {
        if(arr[i] < pivot)
        {
            int temp = arr[i];
            arr[i] = arr[dividor];
            arr[dividor] = temp;
            dividor++;
        }
    }
    arr[low] = arr[dividor-1];
    arr[dividor-1] = pivot;
    return dividor - 1;
}


void insertionSort(int arr [], int size)
{
    for(int i = 1; i < size-2 ; i++)
    {
        int streck = arr[i];
        int j = i - 1;

        while(j >= 0 && arr[j] > streck)
        {
            arr[j + 1] = arr[j];

            j = j - 1;

        }

        arr[j + 1] = streck;


    }


}

int sorted(int arr [] , int size)
{

    for(int i = 1; i < size ; i++)
    {
        int j = i -1;

        if (arr[j] > arr[i])
        {
            return 0;

        }

        

    }

    return 1;
}

void printQuestion(void)
{
    int correct = 0;
    user_answer = 0;
    user_answer_ready = 0;
   
    switch (val)
    {
   case 1:
{
    char q[] = "1: Skiner solen i Australien?\n\r1: Ja  2: Nej\n\r";
    uarte_write(q, strlen(q));
    //int ans = Read_Int();
    correct = 1;
    break;
}
case 2:
{
    char q[] = "2: Vad är 2 + 2?\n\r1: 3  2: 4\n\r";
    uarte_write(q, strlen(q));
    
    correct = 2;
    break;
}
case 3:
{
    char q[] = "3: Vilket djur säger 'mjau'?\n\r1: Katt  2: Hund\n\r";
    uarte_write(q, strlen(q));
    
    correct = 1;
    break;
}
case 4:
{
    char q[] = "4: Vad är huvudstaden i Sverige?\n\r1: Göteborg  2: Stockholm\n\r";
    uarte_write(q, strlen(q));

    correct = 2;
    break;
}
case 5:
{
    char q[] = "5: Hur många ben har en spindel?\n\r1: 8  2: 6\n\r";
    uarte_write(q, strlen(q));

    correct = 1;
    break;
}
case 6:
{
    char q[] = "6: Vad heter vår galax?\n\r1: Vintergatan  2: Andromeda\n\r";
    uarte_write(q, strlen(q));

    correct = 1;
    break;
}
case 7:
{
    char q[] = "7: Vilket år är det just nu?\n\r1: 2025  2: 2023\n\r";
    uarte_write(q, strlen(q));

    correct = 1;
    break;
}
case 8:
{
    char q[] = "8: Vilken färg får du om du blandar blått och gult?\n\r1: Grön  2: Lila\n\r";
    uarte_write(q, strlen(q));
    

    correct = 1;
    break;
}
case 9:
{
    char q[] = "9: Vad är 9 * 9?\n\r1: 81  2: 72\n\r";
    uarte_write(q, strlen(q));

    correct = 1;
    break;
}
case 10:
{
    char q[] = "10: Vem skrev 'Romeo och Julia'?\n\r1: William Shakespeare  2: August Strindberg\n\r";
    uarte_write(q, strlen(q));

    correct = 1;
    break;
}
    default:
        break;
    }
    
    while (!user_answer_ready) {}

    if (user_answer == correct)
    {
        point = 1;
    }
    else
    {
        point = 0;
    }
    countScore();
    
}
void countScore(void)
{
    if (point == 1)
    {
        score ++;
    }
}

