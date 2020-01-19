const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

const onOpen = () => console.log('Open connection');
const onData = (data) => console.log('on Data' + data);

const portName = process.argv[2];

const myPort = new SerialPort(portName, {
    baudRate: 57600,
    parser: new Readline({ delimiter: '\r\n' })
});

// Autre version
// Require the serialport node module
// let SerialPort  = require('serialport');
// let port = new SerialPort ('COM7', {
//     baudRate: 57600,
// }); // Read the port data
// port.on('open', function () {
//     console.log('open');
//     port.on('data', function(data) {
//         console.log(data);
//     });
// });