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
#define MSG_MAX_CHARS 12

/*
 * This is a bitcoin public address
 */
static const char Validator_public_address[45] = "13s3MxSMsTwpxGC58KwPXaKTYMUXYJq3xj";


/* Class for creating a payment action */
class Payment {

public:
	uint32_t msg;

	Payment();
	int confirm(uint8_t *signature, uint8_t signature_len);

};



#endif /* MAIN_PAYMENT_H_ */
