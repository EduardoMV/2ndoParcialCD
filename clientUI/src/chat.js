const udp = require('dgram');
const buffer = require('buffer');
const { login } = require('./auth');

let client;

const server_addr = "127.0.0.1";
const server_port = 8080;


function connectToChat(callback, user, pass) {
    client = udp.createSocket('udp4')

    console.log("socket created");


    client.on('message', (data, info) => {
        data = data.toString();
        console.log(`socket: ${data}`);
        callback(data);
    })

    sendMsg("server", "everyone", `${user} is connected to chat`)
}

function sendMsg(user, to, msg) {
    const data = Buffer.from(`chat:user=${user}&to=${to}&msg=\"${msg}\"`);
    sendToServer(data);
}

function sendGameCmd(user, action, data = "null") {
    const msg = Buffer.from(`game:user=${user}&action=${action}&data=${data}`);
    sendToServer(msg);
}

function sendToServer(buffer) {
    client.send(buffer, server_port, server_addr, (err) => {
        if (err) {
            console.log(err);
        }
    })
}

module.exports = {
    connectToChat,
    sendMsg,
    sendGameCmd
}