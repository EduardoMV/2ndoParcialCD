const { ipcMain } = require('electron')
const { contextBridge, ipcRenderer } = require('electron/renderer')

contextBridge.exposeInMainWorld('connection', {
    connectTCP: (addr, port) => ipcRenderer.invoke('connectTCP', addr, port),
    sendTCP: (value) => ipcRenderer.invoke('sendTCP', value),
    onLoginStatus: (callback) => ipcRenderer.on('login-status', (_evt, value) => callback(value)),
    login: (user, pass) => ipcRenderer.invoke('loginTCP', user, pass)
})