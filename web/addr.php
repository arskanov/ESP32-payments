<?php

require __DIR__ . "/vendor/autoload.php";

use BitWasp\Bitcoin\Key\Deterministic\HierarchicalKeyFactory;
use BitWasp\Bitcoin\Address\AddressCreator;
use BitWasp\Bitcoin\Address\PayToPubKeyHashAddress;
use BitWasp\Bitcoin\Bitcoin;
use BitWasp\Bitcoin\Key\PrivateKeyFactory;
use BitWasp\Bitcoin\Network\NetworkFactory;
use BitWasp\Bitcoin\Transaction\Factory\Signer;
use BitWasp\Bitcoin\Transaction\TransactionFactory;

header('Content-type: application/json');

// Check that we've received a parameter
if (empty($_GET)) {
    $ret = array(
        "id" => "",
        "p2address" => "",
        "error" => "noget",
    );
    echo json_encode($ret);
    exit();
}

function init_db($conn, $nonce) {

    // Insert data we have now
    $sql = 'INSERT INTO "Attempts" ("Nonce", "RequestTime")
        VALUES ($1, $2) 
        RETURNING "Id"';

    $result = pg_prepare($conn, 'my_query',$sql);
    if( $result === FALSE) {
        return 0;
    }

    $date = date('Y-m-d H:i:s');
    $resource = pg_execute($conn, 'my_query', array($nonce, $date)) or die("Error with inserting.");

    $row = pg_fetch_array($resource);

    return $row['0'];
}

function add_address($conn, $addr,$id) {
    
    $sql = 'UPDATE "Attempts" SET "PaymentAddress" = $1 WHERE "Id" = $2';
    
    $result = pg_prepare($conn, 'my_query2',$sql);
    if( $result === FALSE) {
        die("Problem preparing statement");
    }

    pg_execute($conn, 'my_query2', array($addr, $id)) or die("Failed to write DB");

}

//
// Generate an address to receive payment
//

// Load public key to derive from
require_once('receive_pubkey.ini.php');

// Load database connection info
require_once('database_info.ini.php');

// Connecting, selecting database
$dbconn = pg_connect("host=" . $db_host . " port=" . $db_port . " dbname=" . $db_name . " user=" . $db_user . " password=" . $db_pass)
   or die('Could not connect: ' . pg_last_error());

$derivative_id = init_db($dbconn, $_GET['id']);


Bitcoin::setNetwork(NetworkFactory::bitcoinTestnet());
$network = Bitcoin::getNetwork();

// The key to m/0h/X
$xpub=$loaded_pubkey;

$master = HierarchicalKeyFactory::fromExtended($xpub);
$child = $master->deriveChild($derivative_id);
$address = (new PayToPubKeyHashAddress($child->getPublicKey()->getPubKeyHash()))->getAddress();

// Output to Database
add_address($dbconn, $address, $derivative_id);

// Output to client
session_start();
$_SESSION['address'] = $address;
$_SESSION['id'] = $_GET['id'];

$ret = array(
    "id" => $_GET['id'],
    "p2address" => $address,
    "error" => "",
);

echo json_encode($ret);
