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

public:
	unsigned int input;


	Payment();
	int confirm(std::string signed_output);
};



#endif /* MAIN_PAYMENT_H_ */
