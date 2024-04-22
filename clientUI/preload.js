const { contextBridge, ipcRenderer } = require('electron/renderer')

contextBridge.exposeInMainWorld('connection', {
    onLoginStatus: (callback) => ipcRenderer.on('login-status', (_evt, value) => callback(value)),
    onSignupStatus: (callback) => ipcRenderer.on('signup-status', (_evt, value) => callback(value)),
    login: (user, pass) => ipcRenderer.invoke('loginTCP', user, pass),
    signup: (user, pass) => ipcRenderer.invoke('signupTCP', user, pass),
})

contextBridge.exposeInMainWorld('userData', {
    getUserData: () => ipcRenderer.invoke('getUserData'),
})

contextBridge.exposeInMainWorld('game', {
    connect: () => ipcRenderer.invoke('connectGame'),
    onMessage: (callback) => ipcRenderer.on('chat-stream', (_evt, value) => callback(value)),
    send: (cmd, user, to, msg) => ipcRenderer.invoke('sendMsg', cmd, user, to, msg)
})