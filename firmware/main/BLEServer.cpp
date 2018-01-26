/**
 * Create a new BLE server.
 */
#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"
extern "C" {
	#include "esp_log.h"
	#include "driver/gpio.h"
}
#include <string>
#include <Task.h>
#include "payment.h"


#include "sdkconfig.h"


static char LOG_TAG[] = "SampleServer";

#define LED_BUILTIN 2

#define BLINK_GPIO (gpio_num_t)LED_BUILTIN


/* Define callback for a received signature action.
 * Since we always receive 65 bytes of data
 * we receive data in 20 byte segments. Assuming
 * that the length of the fourth packet is always 5
 * we can use this to know when we're done receiving
 */

BLECharacteristic *idid;
Payment *pPymt;
class CB_SignedWrite: public BLECharacteristicCallbacks
{
public:
	void onWrite(BLECharacteristic *pCharacteristic){

		static std::string signed_message;
		static uint8_t message_head = 0;

		std::string rxString = pCharacteristic->getValue();

		if (rxString.length() == 0)
			return;

		ESP_LOGD(LOG_TAG, "Callback for Signed Write called, head at %u, signed msg len %u \n ", message_head, signed_message.length());

		/* Check for first/second/third bytes */
		if (rxString.length() == 20){
			if (message_head < 4) {

				message_head++;
				signed_message += rxString;
				ESP_LOGD(LOG_TAG, "Signed string len now: %u\n ", signed_message.length());
			}
			else {
				signed_message.clear();
				signed_message = "";
				message_head = 0;
			}
		}

		/* Check if fourth byte */
		else if (rxString.length() == 5 && message_head == 3) {
			ESP_LOGD(LOG_TAG, "Received fourth packet, correct length!\n ");

			signed_message += rxString;
			ESP_LOGD(LOG_TAG, "Signed message now %u long\n", signed_message.length());

			if (pPymt->confirm((uint8_t *)signed_message.c_str(), signed_message.length()))
			{
				ESP_LOGD(LOG_TAG, "Succesfully confirmed payment!\n ");
				gpio_set_level(BLINK_GPIO, 1);
			}
			else
				ESP_LOGD(LOG_TAG, "Payment failed...\n ");

			/* Reset */
			signed_message.clear();
			signed_message = "";
			message_head = 0;
		} else {
			/* Reset, don't know what to do */
			signed_message.clear();
			signed_message = "";
			message_head = 0;
		}

	}
};


class MainBLEServer: public Task {
	void run(void *data) {
		/* Config LED GPIO */
		gpio_pad_select_gpio(BLINK_GPIO);
	    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
	    gpio_set_level(BLINK_GPIO, 0);

		ESP_LOGD(LOG_TAG, "Starting BLE work!");

		Payment pymt;
		pPymt = &pymt;

		/* Create our server with charesteristics */
		BLEDevice::init("Pay here!");
		BLEServer *pServer = BLEDevice::createServer();
		BLEService *pService = pServer->createService("d4634412-4590-4776-a7e0-7df5230e4a72");

		/* This is to be read by master (user) */
		BLECharacteristic *signThis = pService->createCharacteristic(
			BLEUUID("9a5effb1-5210-4a71-a7a3-cc0cbe2ca0b7"),
			BLECharacteristic::PROPERTY_READ
		);

		/* Set global pointer value once we have created the char. */
		idid = signThis;

		/* Assign charesteristic value using 4 bytes */
		unsigned char bytes[4];
		unsigned long n = pymt.msg;
		ESP_LOGD(LOG_TAG, "Payment input number is: %u", pymt.msg);

		bytes[0] = (n >> 24) & 0xFF;
		bytes[1] = (n >> 16) & 0xFF;
		bytes[2] = (n >> 8) & 0xFF;
		bytes[3] = n & 0xFF;

		signThis->setValue(bytes, 4);

		/* This is where to the user can write the signed response */
		BLECharacteristic *pSignedCharesteristic = pService->createCharacteristic(
			BLEUUID("fa4dee4d-4086-4372-8de4-d96339451dc7"),
			BLECharacteristic::PROPERTY_WRITE
		);

		/* Function to change some value when written to */
		CB_SignedWrite sw_cb;
		pSignedCharesteristic->setCallbacks(&sw_cb);

		//BLE2902* p2902Descriptor = new BLE2902();
		//p2902Descriptor->setNotifications(true);
		//pCharacteristic->addDescriptor(p2902Descriptor);

		pService->start();

		BLEAdvertising* pAdvertising = pServer->getAdvertising();
		pAdvertising->addServiceUUID(BLEUUID(pService->getUUID()));
		pAdvertising->start();

		ESP_LOGD(LOG_TAG, "Advertising started!");
		delay(1000000);
	}
};


void BLE_Server(void)
{
	MainBLEServer* pMainBleServer = new MainBLEServer();
	pMainBleServer->setStackSize(20000);
	pMainBleServer->start();

} // app_main
