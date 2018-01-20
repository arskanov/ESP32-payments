/**
 * Create a new BLE server.
 */
#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"
extern "C" {
	#include "esp_log.h"
}
#include <string>
#include <Task.h>
#include "payment.h"


#include "sdkconfig.h"


static char LOG_TAG[] = "SampleServer";


/* Define callback for a received signature action */

BLECharacteristic *idid;
Payment *pPymt;
class CB_SignedWrite: public BLECharacteristicCallbacks
{
public:
	void onWrite(BLECharacteristic *pCharacteristic){
		//idid->setValue( pCharacteristic->getValue());
		ESP_LOGD(LOG_TAG, "Callback for Signed Write called: %s\n ", pCharacteristic->getValue().c_str());
		if (pPymt->confirm(pCharacteristic->getValue()))
		{
			ESP_LOGD(LOG_TAG, "Succesfully confirmed payment!\n ");
		}


	}
};


class MainBLEServer: public Task {
	void run(void *data) {
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
