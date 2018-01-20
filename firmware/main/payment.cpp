/*
 * payment.cpp
 *
 *  Created on: 10 Dec 2017
 *      Author: artturi
 */

#include "payment.h"

Payment::Payment()
{
	/* Initialize payment */
	msg = (uint32_t)esp_random();
}

int Payment::confirm(std::string signed_output)
{
	/*
	 * An example can be found at https://github.com/keepkey/keepkey-firmware/blob/master/keepkey/local/baremetal/exchange.c
	 * and
	 * crypto.c, cryptoMessageVerify()
	 */

	/* Generate coin info */
	const CoinInfo *coin = coinByName("Bitcoin");

	uint8_t signature_len = signed_output.length();

	// Four bytes = uint32 for now
	uint8_t raw_msg_bytes[4];
	uint8_t raw_msg_len = 4;
	if (signature_len == 65 && cryptoMessageVerify(coin, raw_msg_bytes, raw_msg_len, Validator_public_address, (const uint8_t*)signed_output.c_str()) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



