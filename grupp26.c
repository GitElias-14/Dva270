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


uint8_t uarte_enabled = 0;


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


void inituart(void){

  const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(20, 22);
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
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