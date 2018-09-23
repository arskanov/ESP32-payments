<?php
require __DIR__ . "/vendor/autoload.php";
use BitWasp\Bitcoin\Address\PayToPubKeyHashAddress;
use BitWasp\Bitcoin\Bitcoin;
use BitWasp\Bitcoin\Key\PrivateKeyFactory;
use BitWasp\Bitcoin\MessageSigner\MessageSigner;
use BitWasp\Bitcoin\Network\NetworkFactory;

// Load key
require_once('key.ini.php');

// Load session
session_start();

// We'll return data as JSON
header('Content-type: application/json');

// Check session is alive
if ( ! isset($_SESSION['id'])) {
    http_response_code(400);
    $ret = array(
        "signed_id" => "",
        "error" => "nosession"
    );
} 
else
{
    /* Check balance */
    $address = $_SESSION['address']; 
    $url = "https://testnet.blockchain.info/q/addressbalance/" . $address;

    $value_in_sat_string = file_get_contents($url);

    $sat = intval ($value_in_sat_string, 10);

    /* Check for a hardcoded > 2000 satoshi balance */
    if ($sat >= 2000) {
        /* Start signing! */
        Bitcoin::setNetwork(NetworkFactory::bitcoin());
        $network = Bitcoin::getNetwork();

        $ec = Bitcoin::getEcAdapter();

        /* This is the signing private key used! */
        $privateKey = PrivateKeyFactory::fromWif($pkWif);

        $message = $_SESSION['id'];
        $signer = new MessageSigner($ec);
        $signed = $signer->sign($message, $privateKey);
        $address = new PayToPubKeyHashAddress($privateKey->getPublicKey()->getPubKeyHash());
        $ret = array(
            "pubkey" => $address->getAddress(),
            "signed_id" => $signed->getBuffer()->getBinary(),
            "error" => ""
        );
        
    }
    else
    {
        /* You really didn't pay */
        http_response_code(400);
        $ret = array(
            "pubkey" => "",
            "signed_id" => "",
            "error" => "not-paid"
        );
    }
}

echo json_encode($ret);


