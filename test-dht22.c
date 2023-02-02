/*
 * Copyright (c) 2016, Zolertia <http://www.zolertia.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */
/**
 * \addtogroup zoul-examples
 * @{
 *
 * \defgroup zoul-dht22-test DHT22 temperature and humidity sensor test
 *
 * Demonstrates the use of the DHT22 digital temperature and humidity sensor
 * @{
 *
 * \file
 *         A quick program for testing the DHT22 temperature and humidity sensor
 * \author
 *         Antonio Lignan <alinan@zolertia.com>
 */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "contiki.h"
#include "dev/dht22.h"
#include "dev/leds.h"
/*---------------------------------------------------------------------------*/
double calculate_dew_point (int16_t temperature, int16_t humidity);
PROCESS(remote_dht22_process, "DHT22 test");
AUTOSTART_PROCESSES(&remote_dht22_process);
/*---------------------------------------------------------------------------*/
static struct etimer et;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(remote_dht22_process, ev, data)
{
  int16_t temperature, humidity;
  //double dewpoint, gamma, temp_Out, temp_log;
   //float temp_In, humid_In;
  double dewpoint_In, dewpoint_Out;
  int16_t temp_Out = 40;
  int16_t humid_Out = 920;
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(dht22);

  /* Let it spin and read sensor data */

  while(1) {
    etimer_set(&et, CLOCK_SECOND*5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    /* The standard sensor API may be used to read sensors individually, using
     * the `dht22.value(DHT22_READ_TEMP)` and `dht22.value(DHT22_READ_HUM)`,
     * however a single read operation (5ms) returns both values, so by using
     * the function below we save one extra operation
     */
    if(dht22_read_all(&temperature, &humidity) != DHT22_ERROR) {
      //next 2 printf are fake double values from dht22 example code
      printf("Temperature %d.%d ºC, ", temperature / 10, temperature % 10);
      printf("Humidity %d.%d RH\n", humidity / 10, humidity % 10);

      //Actual values from sensor
      //printf("Temperature actual value %d ºC, ", temperature);
      //printf("Humidity actual value %d RH\n", humidity);



  dewpoint_In = calculate_dew_point (temperature, humidity);
  dewpoint_Out = calculate_dew_point (temp_Out, humid_Out);

  if (dewpoint_Out > dewpoint_In)
    {
      //turn LED_RED;
      leds_toggle(LEDS_RED);
      //printf("The sensor is: %u\n", leds_get());
      printf ("DO NOT Ventilate, close the window\n");
    }
  else
    {
      //turn LED_GREEN;
      leds_toggle(LEDS_GREEN);
      //printf("The sensor is: %u\n", leds_get());
      printf ("Ventilate, open the window\n");
    }
          printf (" \n");

  //double dewpoint_Out = calculate_dew_point(temp_Out, humid_Out);

  //temp_log = log((double) humidity/1000.00);
  //printf("Log %.2lf B:C\n", temp_log);
      /*
      // Converting int16_t values into double
      temp_In = (float) temperature / 10.0f;
      humid_In = (float) humidity / 10.0f;
      printf("Temperature Inside %.2f ºC, ", temp_In);
      printf("Humidity %f RH\n", humid_In);

      //Calculating dew point temperature
      temp_log = log((double) humidity/1000.00);
      temp_Out = (double) (temperature/10);
      printf("Temperature Outside %.2lf ºC, ", temp_Out);
      gamma = (17.271 * temp_In) / (237.7 + temp_In) + log(humid_In / 100.0);
      dewpoint = ( 237.7 * gamma) / (17.271 - gamma);

      int_dewpoint = (int16_t) (dewpoint * 100);
      printf ("Int dew point %d.%d B:C\n", int_dewpoint / 100, abs(int_dewpoint % 100));
      //printing out results
      printf("Gamma %.2lf ºC\n", gamma);
      printf("Dew Point %.2lf ºC\n", dewpoint);
      printf("Log %.2lf ºC\n", temp_log);
      */
    } else {
      printf("Failed to read the sensor\n");
    }
  }

  PROCESS_END();
}

double
calculate_dew_point (int16_t temperature, int16_t humidity)
{
  double temp_, humid_;
  double gamma, dewpoint;
  int16_t int_dewpoint;

  temp_ = (double) temperature / 10.0;
  humid_ = (double) humidity / 10.0;

  //printf("temp: %f, humid: %f\n", temp_, humid_);

  printf
    ("Calculating dew point for temperature %d.%d Celcius and humidity %d.%d RH\n",
     temperature / 10, temperature % 10, humidity / 10, humidity % 10);

  gamma = (17.271 * temp_) / (237.7 + temp_) + log (humid_ / 100.0);
  //printf("gamma: %f\n", gamma);
  dewpoint = (237.7 * gamma) / (17.271 - gamma);
  //printf ("Dew Point %f B:C\n", dewpoint);

  int_dewpoint = (int16_t) (dewpoint * 100);
  printf ("Dew point %d.%d ºC\n", int_dewpoint / 100,
	  abs (int_dewpoint % 100));

  return dewpoint;
}
/*---------------------------------------------------------------------------*/
/**
 * @}
 * @}
 */

