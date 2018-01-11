/**
 * Main file for running the BLE samples.
 */


extern "C" {
	void app_main(void);
}


void SampleServer(void);

//
// Un-comment ONE of the following
//            ---
void app_main(void) {

	SampleServer();
} // app_main
