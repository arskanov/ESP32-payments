function _base64ToArrayBuffer(base64) {
  var binary_string = window.atob(base64);
  var len = binary_string.length;
  var bytes = new Uint8Array(len);
  for (var i = 0; i < len; i++) {
    bytes[i] = binary_string.charCodeAt(i);
  }
  return bytes.buffer;
}

async function onEnableClick(test) {

  let serviceUuid = "d4634412-4590-4776-a7e0-7df5230e4a72";
  let readCharacteristicUuid = "9a5effb1-5210-4a71-a7a3-cc0cbe2ca0b7";
  let writeCharacteristicUuid = "fa4dee4d-4086-4372-8de4-d96339451dc7";

  let options = {
    "filters": [{
      "name": "Pay here!"
    }],
    "optionalServices": [serviceUuid]
  };


  try {
    console.log('Requesting Payterminal...');
    const device = await navigator.bluetooth.requestDevice(options);

    console.log('Connecting to GATT Server...');
    const server = await device.gatt.connect();

    console.log('Getting Service...');
    const service = await server.getPrimaryService(serviceUuid);

    console.log('Getting Read Characteristic...');
    const readCharacteristic = await service.getCharacteristic(readCharacteristicUuid);

    console.log('Getting Nonce...');
    const nonce_bytearray = await readCharacteristic.readValue();

    // Four bytes -> Integer conversion
    const nonce = nonce_bytearray.getUint32(0);

    console.log('Getting payment address for nonce', nonce);
    const addrJson = await fetch('addr.php?id=' + nonce, { credentials: "include" }).then(r => r.json());

    // Simulate two-seconds to confirm payment
    const paymentSuccess = await setTimeout(function () {
      console.log('Payment "Succesful"...');
      return true;
    }, 0);

    console.log('Getting signed nonce from server...');
    const signed_nonce = await fetch('sign.php', { credentials: "include" }).then(r => r.json());

    // Fourth index contains the signed nonce as base64 encoded string
    const signature = signed_nonce.signed_id.split("-----")[4].trim();

    console.log('Getting Write Characteristic...');
    const writeCharacteristic = await service.getCharacteristic(writeCharacteristicUuid);
    const arrayBuf = await _base64ToArrayBuffer(signature);

    console.log('Writing signature to characteristic...');
    const writeResult = await writeCharacteristic.writeValue(arrayBuf);

    console.log("Done!");

  } catch (error) {
    console.log('Argh! ' + error);
  }
}


document.getElementById("reqpymtbutton").addEventListener('click', () => onEnableClick('test'));

