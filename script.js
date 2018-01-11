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
  paymenthub.request()
  .then(_ => paymenthub.connect())
  .then(_ => {
    var t = document.getElementById("tout").innerHTML;
    //console.log("Tout is: ", t);
    r = paymenthub.readColor();
    var enc = new TextDecoder();
    //console.log("Read gave ", enc.decode(r))
    document.getElementById("tout").innerHTML = enc.decode(r)
  })
  .catch(error => function (err) {
                    console.log(err.message);
                    console.log(err.stack);
  });
});

document.getElementById("writebutton").addEventListener('click', event => {
  paymenthub.request()
  .then(_ => paymenthub.connect())
  .then(_ => { 
    var enc = new TextEncoder("utf-8");
    var t = document.getElementById("tin").innerHTML
    //console.log("Connected to device, writing ", t, " to device")
    paymenthub.writeColor(enc.encode(t))
  })
  .catch(error => { function (err) {
                    console.log(err.message);
                    console.log(err.stack);
  });
});


