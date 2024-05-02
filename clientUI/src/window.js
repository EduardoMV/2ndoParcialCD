const { BrowserWindow } = require('electron')
const path = require('node:path')

function createWindow() {
    const mainWindow = new BrowserWindow({
        width: 1200,
        height: 800,
        webPreferences: {
            preload: path.join(__dirname, '..', 'preload.js'),
            nodeIntegrationInWorker: true
        }
    })

    mainWindow.loadFile('./views/login.html')

    mainWindow.setMenu(null);

    // Open the DevTools.

    return mainWindow;
}

module.exports = createWindow;