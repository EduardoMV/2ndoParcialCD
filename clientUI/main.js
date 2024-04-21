const { app, ipcMain } = require('electron/main')
const createWindow = require('./src/window');
const auth = require('./src/auth')

app.whenReady().then(() => {
    const win = createWindow();

    const updateLoginStatus = (status) => win.webContents.send('login-status', status);
    const updateSignupStatus = (status) => win.webContents.send('signup-status', status);


    ipcMain.handle('loginTCP', (_evt, user, pass) => { auth.login(user, pass, updateLoginStatus) });
    ipcMain.handle('signupTCP', (_evt, user, pass) => { auth.signup(user, pass, updateSignupStatus) })

    app.on('activate', function () {
        if (BrowserWindow.getAllWindows().length === 0) createWindow()
    })
})

app.on('window-all-closed', function () {
    if (process.platform !== 'darwin') app.quit()
})