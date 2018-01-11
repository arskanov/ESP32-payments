/*
 * payment.cpp
 *
 *  Created on: 10 Dec 2017
 *      Author: artturi
 */

#include "payment.h"
#include "esp_system.h"

Payment::Payment()
{
	/* Initialize payment */
	input = esp_random();

}


