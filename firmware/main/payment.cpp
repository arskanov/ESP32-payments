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
	 * The signed output should be in the format of
	 * [1 id byte, r (32Byte), s (32Byte)],
	 * where r and s are points on ecdsa curve.
	 *
	 * Usually signed messages are encoded in base64,
	 * so they have to be decoded before being received here.
	 */

	/* Generate coin info */
	const CoinInfo *coin = coinByName("Bitcoin");

	uint8_t signature_len = signed_output.length();

	// Four bytes = uint32 for now
	char raw_msg_bytes[15];

	sprintf(raw_msg_bytes,"%u",msg);
	std::string rawmsg(raw_msg_bytes);

	/* Debug prints before attempting to verify */
	ESP_LOGD("Payment", "Signature length: %u\nRawMsgBytes: %s\nRawMsgLength: %u\n",signature_len, rawmsg.c_str(),rawmsg.length());

	uint8_t ret = cryptoMessageVerify(coin,
			(const uint8_t *)rawmsg.c_str(), rawmsg.length(),
			Validator_public_address,
			(const uint8_t*)signed_output.c_str());

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



