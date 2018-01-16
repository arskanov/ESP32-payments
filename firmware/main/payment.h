/*
 * payment.h
 *
 *  Created on: 10 Dec 2017
 *      Author: artturi
 */

#ifndef MAIN_PAYMENT_H_
#define MAIN_PAYMENT_H_

#include <string>


/* Class for creating a payment action */
class Payment {

private:
	/* Save a library-compatible public key here, used only to verify messages */
	const std::string pubkey = "x1337key";
public:
	uint32_t input;


	Payment();
	int confirm(std::string signed_output);
};



#endif /* MAIN_PAYMENT_H_ */
