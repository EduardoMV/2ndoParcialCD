const { app, ipcMain, Notification } = require('electron/main')
const createWindow = require('./src/window');
const auth = require('./src/auth')
const chat = require('./src/chat')


app.whenReady().then(() => {
    const win = createWindow();
    let userInfo = "";

    const updateLoginStatus = (status) => win.webContents.send('login-status', status);
    const updateSignupStatus = (status) => win.webContents.send('signup-status', status);

    const updateGameStream = (messages) => win.webContents.send('game-stream', messages);
    const updateChatStream = (messages) => win.webContents.send('chat-stream', messages);

    const updateUserInfo = (data) => { userInfo = data; }


    ipcMain.handle('loginTCP', (_evt, user, pass) => { auth.login(user, pass, updateLoginStatus, updateUserInfo) });
    ipcMain.handle('signupTCP', (_evt, user, pass) => { auth.signup(user, pass, updateSignupStatus) });
    ipcMain.handle('sendNotification', (_evt, title, body) => { new Notification({ title, body }).show() })

    ipcMain.handle('connectGame', () => {
        const usr = JSON.parse(userInfo);
        chat.connectToChat((data) => {
            const [cmd, ..._args] = data;
            if (cmd === "chat") updateChatStream(data);
            else if (cmd === "game") updateGameStream(data);

        }, usr.username, usr.password);
    });

    ipcMain.handle('sendMsg', (_evt, user, to, msg) => chat.sendMsg(user, to, msg));
    ipcMain.handle('sendCmd', (_evt, user, action, data = "null") => chat.sendMsg(user, action, data));

    ipcMain.on('user-info', (_evt, val) => {
        userInfo = val
    })
    ipcMain.handle('getUserData', () => {
        return userInfo
    })

    app.on('activate', function () {
        if (BrowserWindow.getAllWindows().length === 0) createWindow()
    })
})

app.on('window-all-closed', function () {
    if (process.platform !== 'darwin') app.quit()
})