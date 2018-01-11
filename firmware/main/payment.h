/*
 * payment.h
 *
 *  Created on: 10 Dec 2017
 *      Author: artturi
 */

#ifndef MAIN_PAYMENT_H_
#define MAIN_PAYMENT_H_


/* Class for creating a payment action */
class Payment {
private:
	/* Public key for verification of signed payment */
	const int pubkey = 1234;

public:
	unsigned int input;
	Payment();
	confirm(unsigned int signed_output);
};



#endif /* MAIN_PAYMENT_H_ */
