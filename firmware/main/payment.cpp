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
	input = (uint32_t)esp_random();
}

int Payment::confirm(std::string signed_output)
{
	/*
	 * Should actually implement an asymmetric cypher verification
	 * against the Payment classes pubkey
	 *
	 * An example can be found at https://github.com/keepkey/keepkey-firmware/blob/master/keepkey/local/baremetal/exchange.c
	 * and
	 * crypto.c, cryptoMessageVerify()
	 */
	if ( signed_output == "1337" )
		return 1;
	else
		return 0;
}



