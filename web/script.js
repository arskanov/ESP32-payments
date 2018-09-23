function _base64ToArrayBuffer(base64) {
  var binary_string = window.atob(base64);
  var len = binary_string.length;
  var bytes = new Uint8Array(len);
  for (var i = 0; i < len; i++) {
    bytes[i] = binary_string.charCodeAt(i);
  }
  return bytes.buffer;
}

function showAddress(address) {
  // Show payment address to user
  var emailLink = document.querySelector('.js-emaillink');
  emailLink.innerHTML = address;
  emailLink.href = "bitcoin:" + address;
  //Enable second button
  btn2.disabled = false;
  btn2.className = "button-primary";
}

function loadUi(state) {
  if (state == 1) {
    address = localStorage.getItem("paymentAddress")
    showAddress(address);
  } else if (state == 2) {
    // show play time start
    start = Date.parse(localStorage.getItem("playTimeStart"));
    console.log("Playing already started at" + start);
  }

}

btn1 = document.getElementById("reqpymtbutton");
btn2 = document.getElementById("verifybutton");
var addrData;
var service;

// Alert user if demo won't work
if (typeof navigator.bluetooth == "undefined") {
  alert("Your browser or device is not compatible with this demo. Please try a browser that supports web bluetooth");
}

// load old state
if (localStorage.getItem("state") !== undefined) {
  loadUi(localStorage.getItem("state"));
}

async function getService() {


  let serviceUuid = "d4634412-4590-4776-a7e0-7df5230e4a72";
  let options = {
    "filters": [{
      "name": "Pay here!"
    }],
    "optionalServices": [serviceUuid]
  };

  console.log('Requesting Payterminal...');
  const device = await navigator.bluetooth.requestDevice(options);

  console.log('Connecting to GATT Server...');
  const server = await device.gatt.connect();

  console.log('Getting Service...');
  service = await server.getPrimaryService(serviceUuid);
  return service;
}

async function paymentInfo(test_no_bt) {
  // Disable first button
  btn1.className = "button";
  btn1.disabled = true;


  let readCharacteristicUuid = "9a5effb1-5210-4a71-a7a3-cc0cbe2ca0b7";

  try {
    let nonce = 0;
    if (typeof test_no_bt !== 'undefined' && test_no_bt == true) {
      nonce = 1337;
    } else {
      service = await getService();

      console.log('Getting Read Characteristic...');
      const readCharacteristic = await service.getCharacteristic(readCharacteristicUuid);

      console.log('Getting Nonce...');
      const nonce_bytearray = await readCharacteristic.readValue();

      // Four bytes -> Integer conversion
      nonce = nonce_bytearray.getUint32(0);
    }

    console.log('Getting payment address for nonce' + nonce);
    addrData = await fetch('addr.php?id=' + nonce, { credentials: "include" }).then(r => r.json());


    if (addrData.error != null) {
      console.log("Pay to address: " + addrData.p2address);
      showAddress(addrData.p2address);

      // Save state to local storage
      localStorage.setItem("state", 1);
      localStorage.setItem("paymentAddress", addrData.p2address);
    }

    console.log('Finished payment request...');

  }
  catch (error) {
    console.log('Argh! ' + error);
    btn1.className = "button-primary";
    btn1.disabled = false;
  }
}


function timeout(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

async function getSignature() {
  let response = await fetch('sign.php', { credentials: "include" }).then(r => r.json());
  if (typeof response.error !== 'undefined' && response.error == "") {
    return response.signed_id;
  } else {
    return null;
  }
}

async function verifyPayment() {

  btn2.className = "button";
  btn2.disabled = true;

  let writeCharacteristicUuid = "fa4dee4d-4086-4372-8de4-d96339451dc7";

  try {

    console.log('Getting signature from server...');
    let waiting = 1;

    let signed_nonce = "";
    while (waiting) {
      // TODO: add await on Promise.all()
      await timeout(3000);
      signed_nonce = await getSignature();

      //console.log(signed_nonce);
      if (signed_nonce != null) {
        console.log("Payment confirmed by server...");
        waiting = 0;
      }
    }

    // Fourth index contains the signed nonce as base64 encoded string
    const signature = signed_nonce.split("-----")[4].trim();

    console.log('Getting Write Characteristic...');
    if (service === undefined) {
      service = await getService();
    }
    const writeCharacteristic = await service.getCharacteristic(writeCharacteristicUuid);
    const arrayBuf = await _base64ToArrayBuffer(signature);

    console.log('Writing signature to characteristic...');
    const writeResult = await writeCharacteristic.writeValue(arrayBuf);

    // Save state to local storage
    localStorage.setItem("state", 2);
    localStorage.removeItem("paymentAddress");
    localStorage.removeItem("nonce");

    var a = new Date();
    localStorage.setItem("playingStarted", )

    console.log("Done!");

  }
  catch (error) {
    console.log('Argh! ' + error);
    btn2.className = "button-primary";
    btn2.disabled = false;
  }
}

// Link buttons to functions
btn1.addEventListener('click', () => paymentInfo(false));
btn2.addEventListener('click', () => verifyPayment());

