<?php
require __DIR__ . "/vendor/autoload.php";
use BitWasp\Bitcoin\Address\PayToPubKeyHashAddress;
use BitWasp\Bitcoin\Bitcoin;
use BitWasp\Bitcoin\Key\PrivateKeyFactory;
use BitWasp\Bitcoin\MessageSigner\MessageSigner;

header('Content-type: application/json');
session_start();

// Generate bitoin address (should be from something I have the private key to...)

$ec = Bitcoin::getEcAdapter();
$privateKey = PrivateKeyFactory::create(true);
$address = new PayToPubKeyHashAddress($privateKey->getPublicKey()->getPubKeyHash());

// Check that we've received a parameter
if (empty($_GET)) {
    $ret = array(
        "id" => "",
        "p2address" => "",
        "error" => "noget",
    );
}

else {
    // Return it to user
    session_start();
    $_SESSION['address'] = $address;
    $_SESSION['id'] = $_GET['id'];

    $ret = array(
        "id" => $_GET['id'],
        "p2address" => $address->getAddress(),
        "error" => "",
    );
}

echo json_encode($ret);
