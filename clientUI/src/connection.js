const net = require('node:net')

function startTCPConnection(addr, port) {
    const client = new net.Socket();

    client.connect(port, addr, () => {
        console.log('connected');
    })

    return client;
}

module.exports = {
    startTCPConnection,
}