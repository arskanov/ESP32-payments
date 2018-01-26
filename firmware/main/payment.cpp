/*
 * payment.cpp
 *
 *  Created on: 10 Dec 2017
 *      Author: artturi
 */

#include "payment.h"
#include "string.h"

Payment::Payment()
{
	/* Initialize payment */

#ifdef DEBUG
#warning Not generating random ID!
	msg = (uint32_t)1337;
#else
#warning Not DEBUG!
	msg = (uint32_t)esp_random();
#endif
}

int Payment::confirm(uint8_t *signature, uint8_t signature_len)
{
	/*
	 * The signed output should be in the format of
	 * [1 id byte, r (32Byte), s (32Byte)],
	 * where r and s are points on ecdsa curve.
	 *
	 * Usually signed messages are encoded in base64,
	 * so they have to be decoded before being received here.
	 */
	if (signature_len != 65)
		return 0;

	/* Generate coin info */
	const CoinInfo *coin = coinByName("Bitcoin");

	/* Read 32byte uint as it's characters */
	uint8_t message[MSG_MAX_CHARS];
	sprintf((char * )message,"%u",msg);
	uint8_t rawmsglen = strlen((char *)message);
	ESP_LOGD("Payment", "Message strlen = %u", rawmsglen);
	if ( cryptoMessageVerify(coin,
			message, rawmsglen,
			Validator_public_address,
			signature) == 0)
		return 1;
	else
		return 0;
}



