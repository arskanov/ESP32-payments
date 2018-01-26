
class Paymenthub {

  constructor() {
    this.device = null;
    this.onDisconnected = this.onDisconnected.bind(this);
  }

  request() {
    let options = {
      "filters": [{
        "name": "Pay here!"
      }],
      "optionalServices": ["d4634412-4590-4776-a7e0-7df5230e4a72"]
    };
    return navigator.bluetooth.requestDevice(options)
      .then(device => {
        this.device = device;
        this.device.addEventListener('gattserverdisconnected', this.onDisconnected);
      });
  }

  connect() {
    if (!this.device) {
      return Promise.reject('Device is not connected.');
    }
    return this.device.gatt.connect();
  }

  readColor() {
    return this.device.gatt.getPrimaryService("d4634412-4590-4776-a7e0-7df5230e4a72")
      .then(service => service.getCharacteristic("9a5effb1-5210-4a71-a7a3-cc0cbe2ca0b7"))
      .then(characteristic => characteristic.readValue());
  }

  writeColor(data) {
    return this.device.gatt.getPrimaryService("d4634412-4590-4776-a7e0-7df5230e4a72")
      .then(service => service.getCharacteristic("fa4dee4d-4086-4372-8de4-d96339451dc7"))
      .then(characteristic => characteristic.writeValue(data));
  }

  disconnect() {
    if (!this.device) {
      return Promise.reject('Device is not connected.');
    }
    return this.device.gatt.disconnect();
  }

  onDisconnected() {
    console.log('Device is disconnected.');
  }
}

var paymenthub = new Paymenthub();

document.getElementById("readbutton").addEventListener('click', event => {
  let options = {
    "filters": [{
      "name": "Pay here!"
    }],
    "optionalServices": ["d4634412-4590-4776-a7e0-7df5230e4a72"]
  };
  navigator.bluetooth.requestDevice(options)
    .then(device => device.gatt.connect())
    .then(server => {

      /* Start of read + signing service */

      // Getting Service...
      let signed_hash = server.getPrimaryService("d4634412-4590-4776-a7e0-7df5230e4a72")
        .then(service => {
          // Getting Characteristic...
          return service.getCharacteristic("9a5effb1-5210-4a71-a7a3-cc0cbe2ca0b7");
        })
        .then(characteristic => {
          // Reading payment ID
          return characteristic.readValue();
        })
        .then(buffer => {

          console.log('Id read is ' + buffer.getUint32(0));
          console.log('Buffer length is ' + buffer.byteLength);
        }).then(function (buffer) {
          // Here we would use bufferuint32 as ID
          fetch('addr.php?id=1337', { credentials: "include" })
            .then(r => r.json())
            .then(data => {
              console.log(data);
              console.log('Received JSON data, now request sign.php');
              return data;
            })
        }).then(function (data) {
          fetch('sign.php', { credentials: "include" }).then(r => r.json())
            .then(signed => {
              /* Now we have the signature, let's use it */
              console.log('Sign output:');
              console.log(signed);
              let s = signed.signed_id.split("-----");
              console.log("ID: " + s[2] + " Signature: " + s[4]);

              /* Return out of the first (read)  promise*/
              return window.btoa(s[4]);
            })
        })
        .catch(error => { console.log(error); });

      /* Start write promise */
      server.getPrimaryService("d4634412-4590-4776-a7e0-7df5230e4a72")
        .then(service => {
          return service.getCharacteristic("fa4dee4d-4086-4372-8de4-d96339451dc7");
        })
        .then(characteristic => {
          var enc = new TextEncoder("utf-8");
          let hash_as_uint8_buffer = enc.encode(signed_hash);
          characteristic.writeValue(hash_as_uint8_buffer)
        })
        .catch(error => { console.log(error); });
    });
});

document.getElementById("writebutton").addEventListener('click', event => {
  paymenthub.request()
    .then(_ => paymenthub.connect())
    .then(_ => {
      var enc = new TextEncoder("utf-8");
      var t = document.getElementById("tin").innerHTML
      paymenthub.writeColor(enc.encode(t))
    })
    .catch(function (err) {
      console.log(err.message);
      console.log(err.stack);
    });
});
