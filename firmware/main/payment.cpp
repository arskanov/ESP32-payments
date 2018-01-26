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

	/* Generate coin info */
	const CoinInfo *coin = coinByName("Bitcoin");

	// Four bytes = uint32 for now
	uint8_t message[4];
	sprintf((char * )message,"%u",msg);

	uint8_t rawmsglen = 4;

	/* Debug prints before attempting to verify */
	//ESP_LOGD("Payment", "Signature length: %u\nRawMsgBytes: %s\nRawMsgLength: %u\n",signature_len, rawmsg.c_str(),rawmsg.length());

	uint8_t ret = cryptoMessageVerify(coin,
			message, rawmsglen,
			Validator_public_address,
			signature);

	if (signature_len == 65 && ret == 0)
	{
		return 1;
	}
	else
	{
		ESP_LOGD("Payment", "Return value from cryptoMessageVerify: %u\n", ret);
		return 0;
	}
}



