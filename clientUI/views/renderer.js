const msg = document.querySelector("#serverMsg");

window.electronAPI.connectTCP('127.0.0.1', 5000);

const input = document.querySelector("#clientInput");
const button = document.querySelector("#clientSend");

window.electronAPI.onConnection((value) => {
    msg.innerHTML = value;
})

button.addEventListener('click', () => {
    window.electronAPI.sendTCP(input.value);
})
