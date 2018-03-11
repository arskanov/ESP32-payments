<?php
require __DIR__ . "/vendor/autoload.php";
use BitWasp\Bitcoin\Address\PayToPubKeyHashAddress;
use BitWasp\Bitcoin\Bitcoin;
use BitWasp\Bitcoin\Key\PrivateKeyFactory;
use BitWasp\Bitcoin\MessageSigner\MessageSigner;

// Load key
require_once('key.ini.php');

// Load session
session_start();

// We'll return data as JSON
header('Content-type: application/json');

//// Check balance here
$address = $_SESSION['address']; 

//echo sprintf("I should check balance of %s, but I won't.<br>", $address->getAddress());

// Signing part begin, use xprv9s21ZrQH143K2qU8wyQDydJ8s2PCzTZ9aKoKPCEVqbksZjpodm9VZjEYWqrGipmyknXdz9K1Pyro8zgWYTgpWDZ8EZPLifVHsXorvGLiAVa
$ec = Bitcoin::getEcAdapter();

//This is the signing private key used!
$privateKey = PrivateKeyFactory::fromWif($pkWif);
// Pubkey of this to verify against is 13s3MxSMsTwpxGC58KwPXaKTYMUXYJq3xj
// or
// 001F670551A387DACEF2F1BC4C1BC76D02C01F90F85169A3D8 in hex.

if (isset($_SESSION['id'])) {
    $message = $_SESSION['id'];
    $signer = new MessageSigner($ec);
    $signed = $signer->sign($message, $privateKey);
    $address = new PayToPubKeyHashAddress($privateKey->getPublicKey()->getPubKeyHash());
    $ret = array(
        "pubkey" => $address->getAddress(),
        "signed_id" => $signed->getBuffer()->getBinary(),
        "error" => 0
    );
} else {
    $ret = array(
        "signed_id" => "",
        "error" => "nosession"
    );
}

//$_SESSION = array();
//session_unset();
//session_destroy();

echo json_encode($ret);


