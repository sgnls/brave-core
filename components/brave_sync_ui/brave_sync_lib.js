
console.log('brave_sync_lib.js loaded')

cr.define('hello_world', function() {
//window.cr.define('hello_world', function () {
  'use strict';

  /**
   * Be polite and insert translated hello world strings for the user on loading.
   */
  function initialize() {
console.log('brave_sync.js initialize()')
// var fName = arguments.callee.toString().match(/function ([^\(]+)/)[1]
// console.log('brave_sync.js fName=',fName)
// console.log('brave_sync.js arguments.callee.toString()=',arguments.callee.toString())

    // $('welcome-message').textContent = loadTimeData.getStringF('welcomeMessage',
    //     loadTimeData.getString('userName'));
    $('welcome-message').textContent = 'Welcome Here';
console.log('brave_sync_lib.js TAGAB (1) chrome.send=', chrome.send)
    chrome.send('addNumbers', [3, 3]);
  }

  //TODO, AB: remove addResult, this is from Chromium example
  function addResult(result) {
console.log('brave_sync.js addResult result=',result)
     //alert('The result of our C++ arithmetic (lib): 3 + 3 = ' + result);
   }

  // Return an object with all of the exports.
  return {
    initialize: initialize,
    addResult: addResult,//TODO, AB: remove addResult, this is from Chromium example
    initJsLib: InitJsLib,
    callJsLib: CallJsLib,
    callJsLibStr: CallJsLibStr
  };
});

document.addEventListener('DOMContentLoaded', hello_world.initialize);
//document.addEventListener('DOMContentLoaded', window.hello_world.initialize)

var g_savedDeviceId;
var g_savedSeed;

class InjectedObject {
  handleMessage(message, arg1, arg2, arg3, arg4) {
    console.log('brave_sync_lib.js TAGAB InjectedObject.handleMessage', message, arg1, arg2, arg3, arg4);
console.log('brave_sync_lib.js TAGAB message=' + message);
console.log('brave_sync_lib.js TAGAB arg1=' + arg1);
console.log('brave_sync_lib.js TAGAB arg2=' + arg2);
console.log('brave_sync_lib.js TAGAB arg3=' + arg3);
console.log('brave_sync_lib.js TAGAB arg4=' + arg4);

if (false){
    switch(message) {
      case 'get-init-data':
        //messages.GET_INIT_DATA:
console.log('brave_sync_lib.js TAGAB GET_INIT_DATA');
      break;
      case 'got-init-data':
console.log('brave_sync_lib.js TAGAB GOT_INIT_DATA');
        //should answer
        {
          //words:
          var words = 'hazard soul about talent core load ensure effort brave chat canoe hybrid found split cable ball neglect brown column end decorate track west drink'
          var bytes = module.exports.passphrase.toBytes32(words);
console.log('brave_sync_lib.js TAGAB GOT_INIT_DATA bytes=', bytes);
console.log('brave_sync_lib.js TAGAB GOT_INIT_DATA callbackList=', callbackList);
          callbackList['got-init-data'](
              null,
              /*null,*/bytes,  //seed       filled for new persona
              /*4,*/null,   //deviceId   filled for existing devoce
              {apiVersion: '0', serverUrl: 'https://sync-staging.brave.com', debug: true} //config
          );
        }
      break;
      case 'save-init-data':
console.log('brave_sync_lib.js TAGAB SAVE_INIT_DATA');
        var seed = arg1;
        var deviceId = arg2;
        g_savedSeed = seed;
        g_savedDeviceId = deviceId;
console.log('brave_sync_lib.js TAGAB SAVE_INIT_DATA g_savedDeviceId='+g_savedDeviceId);
console.log('brave_sync_lib.js TAGAB SAVE_INIT_DATA g_savedSeed    ='+g_savedSeed);
      break;

      case 'sync-ready':
console.log('brave_sync_lib.js TAGAB SYNC_READY');
/**
* webview -> browser
* sent when sync has finished initialization
*/
        //can fetch records now
        /**
         * browser -> webview
         * sent to fetch sync records after a given start time from the sync server.
         * we perform an S3 ListObjectsV2 request per category. for each category
         * with new records, do
         * GET_EXISTING_OBJECTS -> RESOLVE_SYNC_RECORDS -> RESOLVED_SYNC_RECORDS
         */
        //FETCH_SYNC_RECORDS: _, /* @param Array.<string> categoryNames, @param {number} startAt (in seconds or milliseconds), @param {number=} maxRecords limit response to a given max number of records. set to 0 or falsey to not limit the response */
        callbackList['fetch-sync-records'](
            null,
            ['BOOKMARKS', 'PREFERENCES'],
            0,//mLatestRecordTimeStampt,
            300);
      break;

      case 'get-existing-objects':
console.log('brave_sync_lib.js TAGAB GET_EXISTING_OBJECTS');
/**
 * webview -> browser
 * after sync gets records, it requests the browser's existing objects so sync
 * can perform conflict resolution.
 * isTruncated is true if maxRecords was used and the total number of
 * records exceeds the limit.
 */
//GET_EXISTING_OBJECTS: _, /* @param {string} categoryName, @param {Array.<Object>} records, @param {lastRecordTimeStamp} number, @param {boolean} isTruncated */
      var categoryName = arg1;
      var records = arg2;
      var lastRecordTimeStamp = arg3;
      var isTruncated = arg4;
console.log('categoryName=', categoryName);
console.log('lastRecordTimeStamp=', lastRecordTimeStamp);
console.log('isTruncated=', isTruncated);
      var parsedRecords = JSON.parse(records);
//this is an array of strings
console.log('-------------------');
      for(var i = 0; i < parsedRecords.length; ++i) {
        var record = parsedRecords[i];
console.log('i=', i);
console.log('record=', record);
      }
console.log('-------------------');
      break;
    }
}

    //
    //''
    //chrome.send('handleMessage', message, arg1, arg2, arg3, arg4); //=>SyncJsLayer::handleMessage
    //chrome.send('addNumbers', [3, 3]);
    chrome.send('HandleMessage', [message, arg1, arg2, arg3, arg4]);
  }
}
injectedObject = new InjectedObject()


function CallJsLib(message, arg1, arg2, arg3, arg4) {
console.log('brave_sync_lib.js TAGAB CallJsLib ----');
  var arg2JSON = JSON.stringify(arg2);
console.log('brave_sync_lib.js TAGAB CallJsLib, arg2JSON=',arg2JSON);
  callbackList[message](arg1, arg2, arg3, arg4);
}

function CallJsLibStr(message, arg1, arg2, arg3, arg4) {
console.log('brave_sync_lib.js TAGAB CallJsLibStr ---- message=', message);
console.log('brave_sync_lib.js TAGAB CallJsLibStr ---- arg1=', arg1);
console.log('brave_sync_lib.js TAGAB CallJsLibStr ---- arg2=', arg2);
console.log('brave_sync_lib.js TAGAB CallJsLibStr ---- (typeof arg2)=', (typeof arg2));
console.log('brave_sync_lib.js TAGAB CallJsLibStr ---- arg3=', arg3);
console.log('brave_sync_lib.js TAGAB CallJsLibStr ---- arg4=', arg4);
//  debugger;
  var obj_arg1 = arg1 ? JSON.parse(arg1) : null;
  // var obj_arg2;
  // if (message == 'fetch-sync-records') {
  //   obj_arg2 = arg2 ? JSON.parse(arg2) : null;
  // } else {
  //   obj_arg2 = arg2;
  // }
  var obj_arg2;
  if (message == 'send-sync-records') {
    obj_arg2 = arg2;
  } else {
    obj_arg2 = arg2 ? JSON.parse(arg2) : null;
  }
console.log('brave_sync_lib.js TAGAB CallJsLibStr ---- (typeof obj_arg2)=', (typeof obj_arg2));

  var obj_arg3 = arg3 ? JSON.parse(arg3) : null;
  var obj_arg4 = arg4 ? JSON.parse(arg4) : null;

  if (message == 'words_to_bytes') {
console.log('brave_sync_lib.js TAGAB CallJsLibStr ---- words_to_bytes obj_arg1=', obj_arg1);
    var bytes = module.exports.passphrase.toBytes32(obj_arg1);
    //var jsonBytes = JSON.stringify(bytes);
    chrome.send('HandleMessage', ['words_to_bytes_done', bytes]);
    return;
  } else if (message == 'bytes_to_words') {
console.log('brave_sync_lib.js TAGAB CallJsLibStr ---- words_to_bytes bytes_to_words=', obj_arg1);
    var arr_int = obj_arg1.split(',').map(Number);
console.log('brave_sync_lib.js TAGAB CallJsLibStr ---- words_to_bytes arr_int=', arr_int);
    var buffer = new Uint8Array(arr_int);
console.log('brave_sync_lib.js TAGAB CallJsLibStr ---- words_to_bytes buffer=', buffer);
    var words = module.exports.passphrase.fromBytesOrHex(buffer);
    chrome.send('HandleMessage', ['bytes_to_words_done', words]);
    return;
  }

  callbackList[message](obj_arg1, obj_arg2, obj_arg3, obj_arg4);
}

//globals
// var callbackList = {}; /* message name to callback function */
// var ipc = {};

//AB cannot have initialization inside of dedicated function, because
//chrome.ipcRenderer should be set before sync lib main(){}() call
//SO code
function dynamicallyLoadScript(url) {
    var script = document.createElement("script"); // Make a script DOM node
    script.src = url; // Set it's src to the provided URL
    document.head.appendChild(script); // Add it to the end of the head section of the page (could change 'head' to 'body' to add it to the end of the body section instead)
}
function InitJsLib() {
console.log('brave_sync_lib.js TAGAB InitJsLib ----');
///<script src="chrome://bravesynclib/bundle.js"></script>
dynamicallyLoadScript('chrome://bravesynclib/bundle.js');
}

console.log('brave_sync_lib.js TAGAB doing the call----')

var callbackList = {}; /* message name to callback function */

console.log('brave_sync_lib.js TAGAB (2) chrome.send=', chrome.send)
if (!self.chrome) {
  self.chrome = {};
}

if(!self.chrome.ipc) {
console.log('brave_sync_lib.js TAGAB (3) chrome.send=', chrome.send)
    var ipc = {};

    ipc.once = function (message, cb) {
console.log('brave_sync_lib.js TAGAB (3.5) ipc.once, message=', message);
        callbackList[message] = cb;
        injectedObject.handleMessage(message, '0', '0', '', false);
    };

    ipc.on = ipc.once;

    ipc.send = function (message, arg1, arg2, arg3, arg4) {
        var arg2ToPass = arg2;
        if (undefined != arg2 && typeof arg2 != 'string' && 'save-init-data' != message) {
            arg2ToPass = JSON.stringify(arg2);
        }
        injectedObject.handleMessage(message, undefined != arg1 ? arg1.toString() : '', undefined != arg2ToPass ? arg2ToPass.toString() : arg2ToPass, undefined != arg3 ? arg3.toString() : '', undefined != arg4 ? arg4 : false);
    };

    self.chrome.ipc = ipc;
console.log('brave_sync_lib.js TAGAB (4) chrome.send=', chrome.send)
    chrome.ipcRenderer = chrome.ipc;
}

console.log('brave_sync_lib.js TAGAB the sync call done----')

//}
