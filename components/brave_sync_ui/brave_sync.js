console.log('brave_sync.js loaded')

//cr.define('hello_world', function() {
window.cr.define('hello_world', function () {
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

    chrome.send('addNumbers', [2, 2]);//TODO, AB: remove addResult, this is from Chromium example

    // $('btn_new_to_sync').onclick = function () {
    //   console.log("Handler for btn_new_to_sync.click() called." );
    // }

     //'area_header' always shown
    $('btn_new_to_sync').onclick = btn_new_to_sync_onclick;
    $('btn_have_sync_code').onclick = btn_have_sync_code_onclick;
    $('btn_setup_sync_have_code').onclick = btn_setup_sync_have_code_onclick;
    $('btn_setup_sync_new_to_sync').onclick = btn_setup_sync_new_to_sync_onclick;

    $('btn_reset_sync').onclick = btn_reset_sync_onclick;
    $('btn_sync_new_device').onclick = btn_sync_new_device_onclick;

    $('btn_add_mobile_device').onclick = btn_add_mobile_device_onclick;
    $('btn_add_computer').onclick = btn_add_computer_onclick;
    $('btn_add_mobile_device_done').onclick = btn_add_mobile_device_done_onclick;
    $('btn_add_computer_copy_to_clipboard').onclick = btn_add_computer_copy_to_clipboard_onclick;
    $('btn_add_computer_done').onclick = btn_add_computer_done_onclick;


    $('area_new_to_sync').style.display = 'none';
    $('area_have_sync_code').style.display = 'none';
    $('area_buttons_start').style.display = 'none';
    $('area_sync_this_device').style.display = 'none';
    $('area_devices_list').style.display = 'none';
    $('area_sync_data_types').style.display = 'none';
    $('area_clear_data').style.display = 'none';

    $('area_add_another_device').style.display = 'none';
    $('area_add_another_device_type').style.display = 'none';
    $('area_add_mobile_device').style.display = 'none';
    $('area_add_computer').style.display = 'none';

    chrome.send('pageLoaded');
  }

  function addResult(result) {
//TODO, AB: remove addResult, this is from Chromium example
console.log('brave_sync.js addResult result=',result)
     //alert('The result of our C++ arithmetic: 2 + 2 = ' + result);
   }

   function showSettings(settings, devices) {
console.log('brave_sync.js showSettings settings=',settings);
console.log('brave_sync.js showSettings devices=',devices);
//alert('brave_sync.js showSettings= ' + settings);
//alert('brave_sync.js showSettings devices=' + devices);
    //{sync_bookmarks: false, sync_history: false, sync_settings: false, sync_this_device: false, this_device_name: "Chromium Fork Sync Device"}
console.log('brave_sync.js showSettings settings.this_device_name='+settings.this_device_name);
console.log('brave_sync.js showSettings settings.sync_this_device='+settings.sync_this_device);
console.log('brave_sync.js showSettings settings.sync_settings='+settings.sync_settings);
console.log('brave_sync.js showSettings settings.sync_history='+settings.sync_history);
console.log('brave_sync.js showSettings settings.sync_bookmarks='+settings.sync_bookmarks);

console.log('brave_sync.js showSettings settings.sync_configured='+settings.sync_configured);

      if (settings.sync_configured) {
        showSettingsConfigured(settings, devices);
      } else {
        showSettingsNotConfigured(settings, devices);
      }

   } // function showSettings(settings, devices)

   function showSettingsNotConfigured(settings, devices) {
console.log('brave_sync.js showSettingsForNotConfigured');
     $('area_buttons_start').style.display = 'block';
     $('area_new_to_sync').style.display = 'none';
     $('area_have_sync_code').style.display = 'none';
     $('area_sync_this_device').style.display = 'none';
     $('area_devices_list').style.display = 'none';
     $('area_sync_data_types').style.display = 'none';
     $('area_clear_data').style.display = 'none';
   }

   function showSettingsConfigured(settings, devices) {
console.log('brave_sync.js showSettingsConfigured');
     $('area_buttons_start').style.display = 'none';
     $('area_new_to_sync').style.display = 'none';
     $('area_have_sync_code').style.display = 'none';

     $('area_sync_this_device').style.display = 'block';
     $('area_devices_list').style.display = 'block';
     $('area_sync_data_types').style.display = 'block';
     $('area_clear_data').style.display = 'block';

     $('checkbox_sync_this_device').checked = settings.sync_this_device;
     $('text_this_device_name').textContent = settings.this_device_name;
     $('checkbox_bookmarks').checked = settings.sync_bookmarks;
     $('checkbox_history').checked = settings.sync_history;
     $('checkbox_site_settings').checked = settings.sync_settings;

     //apply devices
     //Dynamically work with tables with JS
     var table = $('table_devices');
///////////////////////////////////////////////
     //show existing
     console.log('brave_sync.js showSettingsConfigured table.rows.length=', table.rows.length);
     var sum = '';
     ////var table = document.getElementById("myTable");
     var rows = table.rows;
     for (var i = 0; i < rows.length; ++i ) {
       var row = rows[i];
         var cells = row.cells;
         for (var j = 0; j < cells.length; ++j ) {
           var cell = cells[j];
             sum += cell.innerHTML;
             sum += '~';
         }
         sum += '~';
     }
     console.log('brave_sync.js showSettingsConfigured existing table', sum);
///////////////////////////////////////////////
     //brave_sync.js showSettings existing table id~Device name~~0~Linux Laptop Chromium Fork~~
     var rows = table.rows;
     for (; rows.length > 1; ) {
       table.deleteRow(rows.length - 1);
     }
////////////////////////////////
/*
(2) [{…}, {…}]
0:{device_id: "0", last_active: "", name: "Akexey Cube tab", object_id: "241, 236, 67, 241, 34, 104, 69, 151, 255, 186, 133, 224, 22, 96, 213, 67"}
1:{device_id: "2", last_active: "", name: "mi4c2", object_id: "131, 167, 121, 234, 248, 73, 7, 51, 141, 194, 173, 96, 73, 73, 70, 67"}
*/
     // add elements according to devices
     for (var i = 0; i < devices.length; ++i){
       var device = devices[i];
       var row = table.insertRow(table.rows.length);
       var cell_id = row.insertCell(0);
       var device_id = device.device_id;
       cell_id.innerHTML = device_id;
       var cell_name = row.insertCell(1);
       cell_name.innerHTML = device.name;
       var cell_last_active = row.insertCell(2);
       var la_str = (new Date(device.last_active)).toString();
       cell_last_active.innerHTML = la_str;

       var cell_delete = row.insertCell(3);
       //cell_delete.innerHTML = '<a href="javascript:deleteDevice('+ device.device_id + ')" >DELETE</a>';
       //^Refused to execute JavaScript URL because it violates the following Content Security Policy directive:
       // "script-src chrome://resources 'self' 'unsafe-eval'". Either the 'unsafe-inline' keyword,
       //a hash ('sha256-...'), or a nonce ('nonce-...') is required to enable inline execution.

       //$('btn_add_computer_done').onclick = btn_add_computer_done_onclick;

       cell_delete.innerHTML = '<a href="#" id="link_delete_device_' + device_id + '" >DELETE</a>';
       // $('link_delete_device_' + device_id).onclick = function () {
       //    deleteDevice(device_id);
       // }
       function makeDeleteFunction(device_id_) {
         return function () {
           deleteDevice(device_id_);  };
       }
       $('link_delete_device_' + device_id).onclick = makeDeleteFunction(device_id);
     }
   }

   function haveSyncWords(sync_words) {
     console.log('brave_sync.js haveSyncWords sync_words=',sync_words);
     $('edit_add_computer_sync_phrase').value = sync_words;
   }

   // function haveSyncQrCode(sync_qr_base64) {
   //   console.log('brave_sync.js haveSyncQrCode=', sync_qr_base64);
   //   var image = new Image();
   //   image.src = sync_qr_base64;
   //   $('area_add_mobile_device_qrcode').appendChild(image);
   //
   //  var typeNumber = 4;
   //  var errorCorrectionLevel = 'L';
   //  var qr = qrcode(typeNumber, errorCorrectionLevel);
   //  qr.addData('Hi!');
   //  qr.make();
   //  $('area_add_mobile_device_qrcode2').innerHTML = qr.createImgTag();;
   // }

   function toHexString(byteArray) {
      return Array.from(byteArray, function(byte) {
        return ('0' + (byte & 0xFF).toString(16)).slice(-2);
      }).join('')
    }
   function haveSeedForQrCode(seed) {
     console.log('brave_sync.js haveSeedForQrCode=', seed);
     // split -> hex -> qr
     var arr_int = seed.split(',').map(Number);
     var s = toHexString(arr_int);
      var qr = qrcode(4, 'L');  //TODO, AB: `qr-image` instead of `qrcode-generator` ?
      qr.addData(s);
      qr.make();
      var cellSize = 8;
      $('area_add_mobile_device_qrcode').innerHTML = qr.createImgTag(cellSize);
   }

  // Return an object with all of the exports.
  return {
    initialize: initialize,
    addResult: addResult,   //TODO, AB: remove addResult, this is from Chromium example
    showSettings: showSettings,
    haveSyncWords: haveSyncWords,
    //haveSyncQrCode: haveSyncQrCode,
    haveSeedForQrCode: haveSeedForQrCode
  };
});

//document.addEventListener('DOMContentLoaded', hello_world.initialize);
document.addEventListener('DOMContentLoaded', window.hello_world.initialize)

// $('btn_new_to_sync').click(function() {
//   console.log("Handler for btn_new_to_sync.click() called." );
// });

function deleteDevice(device_id) {
  //alert('deleteDevice '+device_id);
  chrome.send('deleteDevice', [device_id]);
}

function btn_new_to_sync_onclick() {
console.log("Handler btn_new_to_sync_onclick() called." );
  $('area_buttons_start').style.display = 'none';
  $('area_new_to_sync').style.display = 'block';
}

function btn_have_sync_code_onclick() {
console.log("Handler btn_have_sync_code_onclick() called." );
  $('area_buttons_start').style.display = 'none';
  $('area_have_sync_code').style.display = 'block';
}

function btn_setup_sync_have_code_onclick() {
console.log("Handler btn_setup_sync_have_code_onclick() called." );
  var sync_words = $('edit_sync_words').value;
  var device_name = $('edit_device_name_have_code').value;
console.log("sync_words=",sync_words);
console.log("device_name=",device_name);

  chrome.send('setupSyncHaveCode', [sync_words, device_name]);
}

function btn_setup_sync_new_to_sync_onclick() {
console.log("Handler btn_setup_sync_new_to_sync_onclick() called." );
  var device_name = $('edit_device_name_new_to_sync').value;
console.log("device_name=",device_name);
  chrome.send('setupSyncNewToSync', [device_name]);
}

function btn_reset_sync_onclick() {
  //alert('Reset sync...');
  console.log("Handler btn_reset_sync_onclick() called." );
  chrome.send('resetSync');
}

///////////////////////////////
function btn_sync_new_device_onclick() {
  //alert('Sync a new device...');

  $('area_add_another_device').style.display = 'block';
  $('area_add_another_device_type').style.display = 'block';
  // $('area_add_mobile_device').style.display = 'none';
  // $('area_add_computer').style.display = 'none';
}

function btn_add_mobile_device_onclick() {
  //alert('Add mobile device');
  $('area_add_another_device_type').style.display = 'none';
  $('area_add_mobile_device').style.display = 'block';
  $('area_add_computer').style.display = 'none';

  chrome.send('needSyncQRcode');
}

function btn_add_computer_onclick() {
  //alert('Add computer');
  $('area_add_another_device_type').style.display = 'none';
  $('area_add_mobile_device').style.display = 'none';
  $('area_add_computer').style.display = 'block';

  //chrome.send('setupSyncHaveCode', [sync_words, device_name]);
  chrome.send('needSyncWords');
}

function btn_add_mobile_device_done_onclick() {
  alert('Add mobile device - Done');
}

function btn_add_computer_copy_to_clipboard_onclick() {
  //alert('Copy to clipboard');
  var copyText = $('edit_add_computer_sync_phrase');
  copyText.select();
  document.execCommand('copy');
  alert("Copied the text: " + copyText.value);
}

function btn_add_computer_done_onclick() {
  alert('Add computer - Done');
}
