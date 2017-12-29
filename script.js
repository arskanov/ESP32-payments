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
    .then(service => service.getCharacteristic("9A5EFFB1-5210-4A71-A7A3-CC0CBE2CA0B7"))
    .then(characteristic => characteristic.readValue());
  }

  writeColor(data) {
    return this.device.gatt.getPrimaryService("d4634412-4590-4776-a7e0-7df5230e4a72")
    .then(service => service.getCharacteristic("FA4DEE4D-4086-4372-8DE4-D96339451DC7"))
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
  .then(_ => { document.getElementById("tout").innerHTML = paymenthub.readColor()})
  .catch(error => { console.log(error) });
});

document.getElementById("writebutton").addEventListener('click', event => {
  paymenthub.request()
  .then(_ => paymenthub.connect())
  .then(_ => { paymenthub.writeColor(document.getElementById("tin").innerHTML )})
  .catch(error => { console.log(error) });
});


