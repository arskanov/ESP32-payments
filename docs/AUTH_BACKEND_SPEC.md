### Backend authentication
##### v0.1

The software backend implements two endpoints: `addr` and `sign`, and currently supports `bitcoin testnet` as payment system.

The `addr` endpoint is used to generate the details for receiving a payment, including
- Payment address
- Payment amount (currently missing).

Inputs for the `addr` endpoint are:
- the payment nonce (id)

The payment addresses can be generated from a master public key in order.
The payment details must be saved once generated for checking payments and signing the nonce in the `sign` endpoint.