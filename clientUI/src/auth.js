const { Worker } = require('node:worker_threads')
const { startTCPConnection } = require('./connection');

const path = require('node:path');


function login(user, pass, updateStatus) {

    const client = startTCPConnection("127.0.0.1", 5000);
    client.write(`login:user=${user}&pass=${pass}`);

    updateStatus("pending");

    client.setTimeout(1000)
    client.on('timeout', () => {
        console.log('timeout')
        client.destroy();
    })

    client.on('close', () => {
        console.log('Login connection closed');
    })

    console.log("Starting Login")

    client.on('data', (data) => {
        console.log(data.toString())
        updateStatus(data.toString());

    })
}

module.exports = {
    login
}