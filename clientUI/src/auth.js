const { startTCPConnection } = require('./connection');

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
        data = data.toString();

        if (data !== "null") {
            const [username, password, ...info] = data.split(',');
            const user = { username, password, info };
            data = JSON.stringify(user);
        }

        updateStatus(data.toString());

    })
}

function signup(user, pass, updateStatus) {

    const client = startTCPConnection("127.0.0.1", 5000);
    client.write(`signup:user=${user}&pass=${pass}`);

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
        data = data.toString();
        console.log(data);
        updateStatus(data.toString());
    })
}

module.exports = {
    login,
    signup
}