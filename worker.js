// A web worker is a JavaScript running in the background, without affecting the performance of the page.
// Address of native addon
const {loadLastProfile} = require('./cpp-backend/build/Release/addon.node');

// Calling functions of native addon
var result = loadLastProfile();

// Communicating with main process of Electron App
postMessage(result);
