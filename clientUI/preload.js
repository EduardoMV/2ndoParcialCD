const { contextBridge, ipcRenderer } = require('electron/renderer')

contextBridge.exposeInMainWorld('connection', {
    onLoginStatus: (callback) => ipcRenderer.on('login-status', (_evt, value) => callback(value)),
    login: (user, pass) => ipcRenderer.invoke('loginTCP', user, pass)
})