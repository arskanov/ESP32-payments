/*
 * payment.h
 *
 *  Created on: 10 Dec 2017
 *      Author: artturi
 */

#ifndef MAIN_PAYMENT_H_
#define MAIN_PAYMENT_H_

#include <string>
#include <stdint.h>


#include "coins.h"
#include "crypto.h"

extern "C" {
	#include "esp_system.h"
	#include "esp_log.h"
}

static const char Validator_public_address[25] =
{
	0x00, 0x1F, 0x67, 0x05, 0x51, 0xA3, 0x87, 0xDA, 0xCE, 0xF2,
	0xF1, 0xBC, 0x4C, 0x1B, 0xC7, 0x6D, 0x02, 0xC0, 0x1F, 0x90,
	0xF8, 0x51, 0x69, 0xA3, 0xD8

	/*
	 * This is a bitcoin public address
	 * 001F670551A387DACEF2F1BC4C1BC76D02C01F90F85169A3D8 in straight hex,
	 * 13s3MxSMsTwpxGC58KwPXaKTYMUXYJq3xj in Base58
	 */
};

/* Class for creating a payment action */
class Payment {

public:
	uint32_t msg;

	Payment();
	int confirm(std::string signed_msg);

};



#endif /* MAIN_PAYMENT_H_ */
