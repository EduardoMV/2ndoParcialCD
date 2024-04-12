const net = require("node:net");

const { SERVER_PORT, SERVER_IP } = process.env;

const client = net.createConnection(SERVER_PORT, SERVER_IP, () => {
    console.log("connected");
});
