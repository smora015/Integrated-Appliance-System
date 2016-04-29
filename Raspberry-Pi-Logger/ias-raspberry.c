/*
   Author:      Sergio Morales (sergio.bcoe@gmail.com)
   Date:        11:24 PM Thursday, April 28, 2016
   Description: A simple Raspberry Pi DHT 22 automatic logger.
*/

#include <bcm2835.h>  // GPIO Drivers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./source/Raspberry_Pi/pi_dht_read.h" // DHT22 Drivers


#define LOG_DELAY 180000
#define LOG_FILENAME "/temp_hum.txt"

int main( int argc, char** argv )
{
   float humidity = 0, temperature = 0; // Log temp and humidity data
   FILE *pFile;                         // Write data to temp_hum.txt file
   char buffer[100];                    // Temp string
   time_t t = time( NULL );             // Get current date

   struct tm* lt = localtime(&t);       // Convert time struct to formatted string
   int rtn;

   struct timeval t1, t2;               // Used to set up delays
   double elapsed_time;

   pFile = fopen(LOG_FILENAME, "a");

   if( pFile == NULL ) perror("Error opening file!\n");
   else
   {
      t = time(NULL);
      lt = localtime(&t);

      fprintf(pFile, "\n\nToday's Date (UTC): %s \n", asctime(lt) );
      fflush(pFile);
   }  
   fclose(pFile);

   for(;;)
   {
      rtn =  pi_dht_read( DHT22, 4, &humidity, &temperature );
      if( rtn == DHT_ERROR_ARGUMENT )
      {
         printf("Error with arguments!\n");
         return 1;
      }
      else if( rtn == DHT_ERROR_GPIO )
      {
         printf("Error with GPIO! Exiting...check out README.md on usage and troubleshooting.\n");
         return 2;
      }
      else if( rtn == DHT_ERROR_TIMEOUT || rtn == DHT_ERROR_CHECKSUM )
      {
         printf("Error with chksum or timeout!\n");
      }

      else
      {
         printf("Success! Temperature: %f Humidity: %f\n", temperature, humidity );

         // Open up log file, specific to today's date.
         pFile = fopen(LOG_FILENAME, "a");

         // Append data
         if( pFile == NULL ) perror("Error opening file!\n");
         else
         {
            // Get date
            t = time(NULL);
            lt = localtime(&t);

            strftime( buffer, sizeof(buffer), "%I:%M %p.", lt );

            fprintf(pFile, "%s | Temp: %f, Hum: %f\n", buffer, temperature, humidity);
            fflush(pFile);
         }

         // Close file as soon as data entry is finished. This is to avoid full data loss, as user may unexpectedly disconnect power. TODO: Implement button to shut down this program for fail-safe method of writing data.
         fclose(pFile);


         // Wait for 3 minutes until next log
         gettimeofday( &t1, NULL );
         for( ;; )
         {
            gettimeofday( &t2, NULL );
            elapsed_time = (t2.tv_sec - t1.tv_sec ) * 1000.0;    // sec to ms
            elapsed_time += (t2.tv_usec - t1.tv_usec ) / 1000.0; // us to ms
            
            
            if( elapsed_time > LOG_DELAY )
            break;
         }
      }
      

   }
   return 0;
}
