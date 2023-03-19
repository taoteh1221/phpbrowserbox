//'use strict'
String.prototype.replaceAll = function (search, replacement) {
    return this.replace(new RegExp(search, 'g'), replacement);
};

import { app, BrowserWindow, dialog, ipcMain, shell } from 'electron';
import * as path from 'path';
import { format as formatUrl } from 'url';
const fs = require('fs');

const contextMenu = require('electron-context-menu');

const Config = require('electron-config');

let config = new Config();

const isDevelopment = false;

// global reference to mainWindow (necessary to prevent window from being garbage collected)
let mainWindow;

let win;

//get the basepath
const basePath = __dirname.includes('.asar')
    ? path.resolve(__dirname, '../../../..').replace(/\//g, '\\')
    : path.resolve(__dirname, '../../../../master').replace(/\//g, '\\');

const iconPath = path.join(basePath, '/assets/icon.png');
const appConfigPath = path.join(basePath, '/assets/app.json');
const webConfigPath = path.join(basePath, '/tmp/webkit.json');

//console.log({ basePath });

const loadJson = (opath) => {
    let result = {};

    try {
        const data = fs.readFileSync(opath, {
            encoding: 'utf8',
            flag: 'r',
        });
        result = JSON.parse(data);
    } catch (e) {
        console.log('error', e);
        dialog.showErrorBox('Config invalid', `Invalid config ${opath}`);
        app.exit(0);
    }
    return result;
};

let appConfig = loadJson(appConfigPath);
let webConfig = loadJson(webConfigPath);

//console.log(appConfig);

let modificationEnCours = true;

const defaultOpts = {
    autoHideMenuBar: true,
    webPreferences: {
        spellcheck: true,
        contextIsolation: false,
        nodeIntegration: true,
        nodeIntegrationInWorker: true,
        webSecurity: false,
        sandbox: false,
    },
};

let opts = {
    ...defaultOpts,
    ...appConfig.electron,
    icon: iconPath,
    show: false,
};
Object.assign(opts, config.get('winBounds'));
//console.log('opts', opts);

//activate context menu
if (appConfig.contextMenu) {
    contextMenu(appConfig.contextMenu);
}

const createWindow = () => {
    win = new BrowserWindow(opts);

    win.loadURL(webConfig.url).then(() => {
        try {
            win.show();
        } catch (e) {}
    });

    //worst case scenario, show after 10 seconds eh - sounds fair mate?
    setTimeout(() => {
        try {
            win.show();
        } catch (e) {}
    }, 10000);

    /*
    win.on('ready-to-show', () => {
        win.show()
    });
    */

    win.on('close', (e) => {
        config.set('winBounds', win.getBounds());
    });

    win.webContents.on('devtools-opened', () => {
        win.focus();
        setImmediate(() => {
            win.focus();
        });
    });

    return win;
};

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
    mainWindow = createWindow();

    app.on('activate', function () {
        // On macOS it's common to re-create a window in the app when the
        // dock icon is clicked and there are no other windows open.
        if (BrowserWindow.getAllWindows().length === 0) createWindow();
    });
});

//if a second instance is launched
app.on('second-instance', async (_event, _commandLine, _workingDirectory) => {
    console.log('Second Instance');
    try {
        win.focus();
    } catch (e) {}
});

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit();
    }
});
