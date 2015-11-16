#include <bcm2835.h>
#include <stdio.h>
#include "./source/Raspberry_Pi/pi_dht_read.h"



int main( int argc, char** argv )
{
  float humidity = 0, temperature = 0;

  int rtn =  pi_dht_read( DHT22, 4, &humidity, &temperature );

  if( rtn == DHT_ERROR_ARGUMENT )
    printf("Error with arguments!\n");
  else if( rtn == DHT_ERROR_GPIO )
    printf("Error with GPIO!\n");
  else if( rtn == DHT_ERROR_TIMEOUT || rtn == DHT_ERROR_CHECKSUM )
    printf("Error with chksum or timeout!\n");
  else
    printf("Success! Temperature: %f Humidity: %f\n", temperature, humidity );


  return 0;
}
