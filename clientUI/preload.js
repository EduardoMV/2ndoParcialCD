const { contextBridge, ipcRenderer } = require('electron/renderer')

contextBridge.exposeInMainWorld('connection', {
    onLoginStatus: (callback) => ipcRenderer.on('login-status', (_evt, value) => callback(value)),
    onSignupStatus: (callback) => ipcRenderer.on('signup-status', (_evt, value) => callback(value)),
    login: (user, pass) => ipcRenderer.invoke('loginTCP', user, pass),
    signup: (user, pass) => ipcRenderer.invoke('signupTCP', user, pass),
    sendNotification: (title, body) => ipcRenderer.invoke('sendNotification', title, body),
})

contextBridge.exposeInMainWorld('userData', {
    getUserData: () => ipcRenderer.invoke('getUserData'),
})

contextBridge.exposeInMainWorld('game', {
    connect: () => ipcRenderer.invoke('connectGame'),
    onMessage: (callback) => ipcRenderer.on('chat-stream', (_evt, value) => callback(value)),
    onCommand: (callback) => ipcRenderer.on('game-stream', (_evt, value) => callback(value)),
    send: (user, to, msg) => ipcRenderer.invoke('sendMsg', user, to, msg),
    cmd: (user, action, data = "null") => ipcRenderer.invoke('sendCmd', user, action, data),
})