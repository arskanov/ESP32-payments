## ESP32-payments

Request and validate payments for an embedded device using an ESP32 and web bluetooth. WIP.

## Architecture
```
        ESP32                 +           Client's browser          +      Server
                              |                                     |
                              |                                     |
+-------------------------------------------------------------------------------------------------+
                              |                                     |
                              |                                     |
                              |                                     |
                              |   Start payment:                    |
                              |   -Request "id" for payment         |
                              |                  +                  |
                              |                  |                  |
                              |                  |                  |
                              |                  |                  |
 Generate random ID,          |                  |                  |
 return it to browser  <-------------------------+                  |
           +                  |                                     |
           |                  |                                     |
           |                  |                                     |
           +--------------------->  Request payment address,        |
                              |     for this new "id"               |
                              |                  +                  |
                              |                  |                  |
                              |                  |                  |
                              |                  |                  |
                              |                  |                  |     Generate receiving address,
                              |                  +-------------------->   save (it & id) to $session,
                              |                                     |     return address to client
                              |                                     |               +
                              |                                     |               |
                              |                                     |               |
                              |     Show address to user along      |               |
                              |     with amount, then wait       <------------------+
                              |                                     |
                              |                                     |
                              |             . . .                   |
                              |                                     |
                              |     Once user confirms that she     |
                              |     has paid, request confirmation  |
                              |     from server                     |
                              |                  +                  |
                              |                  |                  |
                              |                  |                  |      Verify transaction, if OK
                              |                  +-------------------->    sign id with signing
                              |                                     |      private key
                              |                                     |              +
                              |                                     |              |
                              |     Forward signed message to       |              |
                              |     the ESP32                     <----------------+
                              |                  +                  |
                              |                  |                  |
                              |                  |                  |
                              |                  |                  |
 Verify signed message        |                  |                  |
 against signer's public  <----------------------+                  |
 key, if OK do what the       |                                     |
 user paid for!               |                                     |
            +                 |                                     |
            |                 |                                     |
            |                 |                                     |
            |                 |                                     |
            v                 +                                     +
           Yay!
```


## How to use
`web` contains the code for the payment webpage, you can find the current version hosted on github pages here https://arskanov.github.io/ESP32-payments/web/. However, no server-side logic can be hosted there. You'll need to host the site locally to test it for now.

To work the PHP scripts need the private key used for signing the messages sent to the offline device. This should be as `key.ini.php`, and should follow the format 
```
<?php
$pkWif = "5HueCGU8rMjxEXxiPuD5BDku4MkFqeZyd4dZ1jvhTVqvbTLvyTJ";
?>
```
Key from en.bitcoin.it.

Visit the payment website with a mac/linux device with BT 4.0 and a recent Chrome or Opera browser, or use your phone and the same browsers to visit.

Tested on Android 6.0 + Chrome latest stable

### How to build for ESP32
`firmware` contains ESP32 firmware. You'll need to add these to the components subdir:
- https://github.com/nkolban/esp32-snippets
- https://github.com/arskanov/trezor-crypto

Now you should be able to build it with a `make` in the firmware directory, if you have done this https://esp-idf.readthedocs.io/en/v2.0/index.html . Make sure to get the newest xtensa toolchain! It's not the one linked on that page.

Flash it with `make flash`
