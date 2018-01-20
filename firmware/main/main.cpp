/**
 * Main file for entry point.
 */

extern void BLE_Server();

extern "C" {
	void app_main();
}

void app_main() {
	BLE_Server();
} // app_main
