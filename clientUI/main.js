const { app, ipcMain } = require('electron/main')
const createWindow = require('./src/window');
const auth = require('./src/auth')


function sendLogin(user, pass) {
    client.write(`user: ${user}`);
    client.write(`pass: ${pass}`);

}

app.whenReady().then(() => {
    const win = createWindow()

    const updateLoginStatus = (status) => win.webContents.send('login-status', status);

    ipcMain.handle('loginTCP', (_evt, user, pass) => { auth.login(user, pass, updateLoginStatus) });

    app.on('activate', function () {
        if (BrowserWindow.getAllWindows().length === 0) createWindow()
    })
})

app.on('window-all-closed', function () {
    if (process.platform !== 'darwin') app.quit()
})