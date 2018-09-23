## ESP32-payments

Request and validate payments for an embedded device using an ESP32 and web bluetooth. Demo.

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

### Website
`web` contains the code for the payment webpage.

You'll need to host the site locally.

To work the PHP scripts need:
- Private key used for signing the messages sent to the offline device. This should be as `key.ini.php`.
- Running PSQL instance, with a table called Attempts, schema at the end of this file
- Database info for connection, check addr.php for required `$db_*` variables`database_info.ini.php`
- Public key for deriving the payment receive addresses

Configuration files should follow the format 
```
<?php
$pkWif = "5HueCGU8rMjxEXxiPuD5BDku4MkFqeZyd4dZ1jvhTVqvbTLvyTJ";
?>
```
Key from en.bitcoin.it.

Visit the payment website with a mac/linux device with BT 4.0 and a recent Chrome or Opera browser, or use your phone and the same browsers to visit.

Tested on Android 8.1 + Chrome latest stable

### How to build for ESP32
`firmware` contains ESP32 firmware. You'll need to add these to the components subdir:
- https://github.com/nkolban/esp32-snippets
- https://github.com/arskanov/trezor-crypto

Now you should be able to build it with a `make` in the firmware directory, if you have done this https://esp-idf.readthedocs.io/en/v2.0/index.html . Make sure to get the newest xtensa toolchain! It's not the one linked on that page.

Change the signing public key to veridy against to match the private key in `key.ini.php`

Flash it with `make flash`

### PSQL Schema
```
-- Table: public."Attempts"

-- DROP TABLE public."Attempts";

CREATE TABLE public."Attempts"
(
  "RequestTime" timestamp with time zone NOT NULL,
  "ConfirmTime" timestamp with time zone,
  "Nonce" bigint,
  "PaymentAddress" text,
  "SignAttempts" integer DEFAULT 0,
  "State" text,
  "Id" integer NOT NULL DEFAULT nextval('"Attempts_Id_seq"'::regclass),
  CONSTRAINT "Key" PRIMARY KEY ("Id")
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public."Attempts"
  OWNER TO postgres;


```
