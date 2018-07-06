/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_sync/brave_sync_controller.h"

#include <sstream>

#include "base/values.h"
#include "base/strings/string_util.h"
#include "base/debug/stack_trace.h"
#include "base/strings/string_number_conversions.h"
#include "base/json/json_reader.h"

#include "brave/components/brave_sync/brave_sync_profile_prefs.h"
#include "brave/components/brave_sync/brave_sync_settings.h"
#include "brave/browser/ui/webui/sync/i_sync_ui.h"
#include "base/debug/stack_trace.h"
#include "brave/components/brave_sync/brave_sync_devices.h"
#include "brave/components/brave_sync/brave_sync_jslib_const.h"
#include "brave/components/brave_sync/brave_sync_obj_map.h"
#include "brave/components/brave_sync/value_debug.h"
#include "brave/components/brave_sync/brave_sync_tools.h"
#include "base/numerics/safe_conversions.h"
#include "base/time/time.h"
#include "brave/components/brave_sync/value_debug.h"

//TODO, AB: maybe namespace? bravesync{}
#include "brave/browser/ui/brave_pages.h"
#include "chrome/browser/profiles/profile_manager.h"

#include "chrome/browser/ui/browser_list.h"

#include "base/task_runner.h"

#include "base/task_scheduler/scheduler_worker_pool.h"
#include "base/task_scheduler/post_task.h"

#include "content/public/browser/browser_thread.h"

char DEVICES_NAMES[] = "devicesNames"; // TODO, AB: move to const

 const char CREATE_RECORD[] = "0";
 const char UPDATE_RECORD[] = "1";
 const char DELETE_RECORD[] = "2";


 // public static class SyncObjectData {
 //     public static final String BOOKMARK = "bookmark";
 //     public static final String HISTORY_SITE = "historySite";
 //     public static final String SITE_SETTING = "siteSetting";
 //     public static final String DEVICE = "device";
 // }

const char SyncObjectData_BOOKMARK[]       = "bookmark";
const char SyncObjectData_HISTORY_SITE[]   = "historySite";
const char SyncObjectData_SITE_SETTING[]   = "siteSetting";
const char SyncObjectData_DEVICE[]         = "device";

// public static class SyncRecordType {
//     public static final String BOOKMARKS = "BOOKMARKS";
//     public static final String HISTORY = "HISTORY_SITES";
//     public static final String PREFERENCES = "PREFERENCES";

const char SyncRecordType_BOOKMARKS[] = "BOOKMARKS";
const char SyncRecordType_HISTORY[] = "HISTORY_SITES";
const char SyncRecordType_PREFERENCES[] = "PREFERENCES";

BraveSyncControllerFactory* BraveSyncControllerFactory::GetInstance() {
  return base::Singleton<BraveSyncControllerFactory>::get();
}

// BraveSyncController *BraveSyncControllerFactory::CreateController() {
//   return base::Singleton<BraveSyncController>::get();
// }

BraveSyncController::BraveSyncController() :
  sync_ui_(nullptr),
  sync_js_layer_(nullptr),
  sync_initialized_(false),
  device_id_(0) {
  LOG(ERROR) << "TAGAB BraveSyncController::BraveSyncController CTOR";
LOG(ERROR) << UPDATE_RECORD << DELETE_RECORD <<
SyncObjectData_BOOKMARK <<  SyncObjectData_HISTORY_SITE <<  SyncObjectData_SITE_SETTING <<
SyncRecordType_BOOKMARKS << SyncRecordType_HISTORY << SyncRecordType_PREFERENCES;

//  LOG(ERROR) << base::debug::StackTrace().ToString();
  //DCHECK(false);
  // char *p =  nullptr;
  // *p = 42;

  sync_prefs_.reset(new bravesync::prefs::BraveSyncPrefs(nullptr)); //this is wrong. TODO, AB: pass the pointer

  std::unique_ptr<BraveSyncSettings> settings_ = std::make_unique<BraveSyncSettings>();

  std::unique_ptr<BraveSyncSettings> settingsTest = sync_prefs_->GetBraveSyncSettings();
LOG(ERROR) << "TAGAB BraveSyncController::BraveSyncController settingsTest";
LOG(ERROR) << "TAGAB BraveSyncController::BraveSyncController settingsTest->this_device_name_=" << settingsTest->this_device_name_;
LOG(ERROR) << "TAGAB BraveSyncController::BraveSyncController settingsTest->sync_this_device_=" << settingsTest->sync_this_device_;

LOG(ERROR) << "TAGAB BraveSyncController::BraveSyncController sync_prefs_->GetSeed()=<" << sync_prefs_->GetSeed() <<">";
LOG(ERROR) << "TAGAB BraveSyncController::BraveSyncController sync_prefs_->GetThisDeviceName()=<" << sync_prefs_->GetThisDeviceName() <<">";

// volatile bool b_DBG_clear_on_start = true;
// if (b_DBG_clear_on_start) {
//   LOG(ERROR) << "TAGAB BraveSyncController::BraveSyncController clearing prefs";
//   sync_prefs_->Clear();
// }

// volatile bool b_DBG_setname_on_start = true;
// if (b_DBG_setname_on_start) {
//   LOG(ERROR) << "TAGAB BraveSyncController::BraveSyncController set name to cf56";
//   sync_prefs_->SetDeviceName("cf56");
// LOG(ERROR) << "TAGAB BraveSyncController::BraveSyncController sync_prefs_->GetThisDeviceName()=" << sync_prefs_->GetThisDeviceName();
//   DCHECK(false);
// }

BrowserList::GetInstance()->AddObserver(this); // I cannot add observer twice
  // Can I call remove observer without call for add?
  // src/base/observer_list.h:249, Does nothing if this observer is
  // not in this list


  if (!sync_prefs_->GetSeed().empty() && !sync_prefs_->GetThisDeviceName().empty()) {
LOG(ERROR) << "TAGAB BraveSyncController::BraveSyncController sync is configured";
    //InitJsLib();

    scoped_refptr<base::SequencedTaskRunner> task_runner =
      base::CreateSequencedTaskRunnerWithTraits(
        //{base::MayBlock(), base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN}
        {base::MayBlock(), base::TaskPriority::BACKGROUND/*USER_BLOCKING*/,
         base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN}
      );

    task_runner->PostTask(FROM_HERE,
    base::Bind(&BraveSyncController::InitJsLib,
        base::Unretained(this), false));

  } else {
LOG(ERROR) << "TAGAB BraveSyncController::BraveSyncController sync is NOT configured";
  }

  //should be invoked from the UI when it is loaded
  // else {
  //   sync_ui_->ShowInitialControls();
  // }
}

BraveSyncController::~BraveSyncController() {
  LOG(ERROR) << "TAGAB BraveSyncController::~BraveSyncController DTOR";
  BrowserList::GetInstance()->RemoveObserver(this);
}

BraveSyncController* BraveSyncController::GetInstance() {
LOG(ERROR) << "TAGAB BraveSyncController::GetInstance";
//LOG(ERROR) << base::debug::StackTrace().ToString();

  return base::Singleton<BraveSyncController>::get();
}

void BraveSyncController::OnSetupSyncHaveCode(const std::string &sync_words,
  const std::string &device_name) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnSetupSyncHaveCode";
  LOG(ERROR) << "TAGAB sync_words=" << sync_words;
  LOG(ERROR) << "TAGAB device_name=" << device_name;

  device_name_ = device_name; //fill member here, but save in OnSaveInitData

  //InitJsLib();-now this is pointless
  //call JS Lib
  //CallJsLib("test"); //
  //sync_words_from_ui_ = sync_words;
  //OnGotInitData(nullptr);
  std::string arg1;
  //std::replace(arg1.begin(), arg1.end(), " ", ", ");
  //bool bRet = base::ReplaceChars(sync_words, " ", "\", \"", &arg1);
  //DCHECK(bRet);

  //arg1 = "[\"" + sync_words + "\"]";
  arg1= "\"" + sync_words + "\"";
  CallJsLibStr("words_to_bytes", arg1, "", "", "");
}

void BraveSyncController::OnSetupSyncNewToSync(const std::string &device_name) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnSetupSyncNewToSync";
  LOG(ERROR) << "TAGAB device_name="<<device_name;

  device_name_ = device_name; //fill member here, but save in OnSaveInitData

  InitJsLib(true);//Init will cause load of the Script
  //then we will got GOT_INIT_DATA and SAVE_INIT_DATA, where we will save the seed and device id

  //then when we will receive sync_ready, we should display web page with sync settings
}

void BraveSyncController::OnDeleteDevice(const std::string &device_id) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnDeleteDevice";
  LOG(ERROR) << "TAGAB device_id="<<device_id;

  CHECK(sync_js_layer_ != nullptr);
  CHECK(sync_initialized_);

  std::string json = sync_obj_map_->GetObjectIdByLocalId(DEVICES_NAMES);
  SyncDevices syncDevices;
  syncDevices.FromJson(json);
LOG(ERROR) << "TAGAB BraveSyncController::OnDeleteDevice json="<<json;

  //DCHECK(false);

  const SyncDevice *device = syncDevices.GetByDeviceId(device_id);
LOG(ERROR) << "TAGAB BraveSyncController::OnDeleteDevice device="<<device;
  DCHECK(device);
  if (device) {
    const std::string device_name = device->name_;
    const std::string object_id = device->object_id_;
    LOG(ERROR) << "TAGAB BraveSyncController::OnDeleteDevice device_name="<<device_name;
    LOG(ERROR) << "TAGAB BraveSyncController::OnDeleteDevice object_id="<<object_id;

    SetUpdateDeleteDeviceName(
      DELETE_RECORD,
      device_name,
      device_id,
      object_id);
  }
}

void BraveSyncController::OnResetSync() {
  LOG(ERROR) << "TAGAB  BraveSyncController::OnResetSync";
  CHECK(sync_js_layer_ != nullptr);
  CHECK(sync_initialized_);

  // const std::string device_name = sync_prefs_->GetThisName();
  // const std::string object_id;
  const std::string device_id = sync_prefs_->GetThisDeviceId();
  LOG(ERROR) << "TAGAB BraveSyncController::OnResetSync device_id="<<device_id;
  OnDeleteDevice(device_id);

  sync_prefs_->Clear();

  if (sync_ui_) {
    sync_ui_->OnSyncStateChanged();
  }
}

void BraveSyncController::GetSettings(BraveSyncSettings &settings) {
  LOG(ERROR) << "TAGAB BraveSyncController::GetSettings";

//
// std::string this_device_name_;
// bool sync_this_device_;
// bool sync_bookmarks_;
// bool sync_settings_;
// bool sync_history_;

//or query them from prefs
  //TODO, AB: move these to BraveSyncPrefs:: ?
  settings.this_device_name_ = sync_prefs_->GetThisDeviceName();
  settings.sync_this_device_ = sync_prefs_->GetSyncThisDevice();
  settings.sync_bookmarks_ = sync_prefs_->GetSyncBookmarksEnabled();
  settings.sync_settings_ = sync_prefs_->GetSyncSiteSettingsEnabled();
  settings.sync_history_ = sync_prefs_->GetSyncHistoryEnabled();

LOG(ERROR) << "TAGAB settings.this_device_name_=<" << settings.this_device_name_ << ">";
LOG(ERROR) << "TAGAB settings.sync_this_device_=<" << settings.sync_this_device_ << ">";
LOG(ERROR) << "TAGAB sync_prefs_->GetSeed()=<" << sync_prefs_->GetSeed() << ">";
LOG(ERROR) << "TAGAB sync_prefs_->GetThisDeviceName()=<" << sync_prefs_->GetThisDeviceName() << ">";

  settings.sync_configured_ = !sync_prefs_->GetSeed().empty() &&
    !sync_prefs_->GetThisDeviceName().empty();

LOG(ERROR) << "TAGAB settings.sync_configured_=<" << settings.sync_configured_ << ">";
  ///settings = *settings_;
  //std::unique_ptr<BraveSyncSettings> settings_here = sync_prefs_->GetBraveSyncSettings();
}

// void BraveSyncController::GetDevices(std::vector<DeviceInChain> &devices) {
//   devices = devices_;
//
//   // test of UI
//   //DeviceInChain device = {"id", "name", "last_active"};
//   //devices = {DeviceInChain(), DeviceInChain(), DeviceInChain()};
//   devices = {
//     {"id1", "name1", "last_active1"},
//     {"id2", "name2", "last_active2"},
//     {"id3", "name3", "last_active3"}
//   };
// }

void BraveSyncController::GetDevices(SyncDevices &devices) {
LOG(ERROR) << "TAGAB BraveSyncController::GetDevices";

//   // fetch devices
//   FetchSyncRecords(false, false, true, 0, 300);
// LOG(ERROR) << "TAGAB BraveSyncController::GetDevices, request FetchSyncRecords is done";
// ^ causes recursion

  std::string json = sync_obj_map_->GetObjectIdByLocalId(DEVICES_NAMES);
  SyncDevices syncDevices;
  syncDevices.FromJson(json);
LOG(ERROR) << "TAGAB BraveSyncController::GetDevices json="<<json;
  //devices = devices_;
  devices = syncDevices; //std::move ?

  // test of UI
  //DeviceInChain device = {"id", "name", "last_active"};
  //devices = {DeviceInChain(), DeviceInChain(), DeviceInChain()};
  // devices = {
  //   {"id1", "name1", "last_active1"},
  //   {"id2", "name2", "last_active2"},
  //   {"id3", "name3", "last_active3"}
  // };
}

void BraveSyncController::GetSyncWords() {
LOG(ERROR) << "TAGAB BraveSyncController::GetSyncWords";
  //return "SUPER PUPER SYNC WORDS";

  // Ask JS library
  std::string seed = sync_prefs_->GetSeed();
  std::string arg1= "\"" + seed + "\"";
  CallJsLibStr("bytes_to_words", arg1, "", "", "");
}

// std::string BraveSyncController::GetSyncQrCodeBase64() {
// /*
//   return    "data:image/png;base64,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";
// */
//    // Ask JS library
//    //
//    //#error 2
//    // CHECK(false);
//    // return "AAAA";
// }

std::string BraveSyncController::GetSeed() {
  LOG(ERROR) << "TAGAB BraveSyncController::GetSeed";
  std::string seed = sync_prefs_->GetSeed();
  return seed;
}

void BraveSyncController::LoadJsLibPseudoTab() {
  LOG(ERROR) << "TAGAB BraveSyncController::LoadJsLibPseudoTab";

  // TODO, AB: this is not good.
  // Possible situation:
  // 1) open browser A
  // 2) create tab with js lib in tab A
  // 3) create browser B
  // 4) close browser B
  // either to move js lib into V8 or subscribe on BrowserListObserver events
  // so during BrowserListObserver::OnBrowserRemoved do re-init of sync lib
  Browser* browser = BrowserList::GetInstance()->GetLastActive();

  LOG(ERROR) << "TAGAB BraveSyncController::LoadJsLibPseudoTab browser=" << browser;

  if (browser) {
    brave::LoadBraveSyncJsLib(browser);
  } else {
    // Well, wait for the browser to be loaded, do work in BraveSyncController::OnBrowserAdded
    LOG(ERROR) << "TAGAB BraveSyncController::LoadJsLibPseudoTab browser=(NIL)!";
  }
}

void BraveSyncController::OnBrowserAdded(Browser* browser) {
  LOG(ERROR) << "TAGAB  BraveSyncController::OnBrowserAdded browser="<<browser;
}

void BraveSyncController::OnBrowserSetLastActive(Browser* browser) {
  LOG(ERROR) << "TAGAB  BraveSyncController::OnBrowserSetLastActive browser="<<browser;
  LOG(ERROR) << "TAGAB  BraveSyncController::OnBrowserSetLastActive sync_js_layer_="<<sync_js_layer_;
  if (sync_js_layer_) {
    return;
  }
  // Browser* browser = BrowserList::GetInstance()->GetLastActive();
  //
  // DCHECK(browser);

  //brave::LoadBraveSyncJsLib(browser);

  // scoped_refptr<base::SequencedTaskRunner> task_runner =
  //   base::CreateSequencedTaskRunnerWithTraits(
  //     //{base::MayBlock(), base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN}
  //     {base::MayBlock(), base::TaskPriority::BACKGROUND/*USER_BLOCKING*/,
  //      base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN}
  //   );
  // task_runner->PostDelayedTask(FROM_HERE,
  // base::Bind(&BraveSyncController::InitJsLib,
  //     base::Unretained(this)),
  //     base::TimeDelta::FromSeconds(30));

  content::BrowserThread::GetTaskRunnerForThread(content::BrowserThread::UI)
    ->PostTask(FROM_HERE, base::Bind(&BraveSyncController::InitJsLib,
         base::Unretained(this), false));

/*
^--
[15967:15987:0628/202905.331219:FATAL:pref_service.cc(281)] Check failed: (sequence_checker_).CalledOnValidSequence().
#0 0x7f6d2695346d base::debug::StackTrace::StackTrace()
#1 0x7f6d26951a2c base::debug::StackTrace::StackTrace()
#2 0x7f6d269dcc0a logging::LogMessage::~LogMessage()
#3 0x7f6d1a1ba848 PrefService::GetDictionary()
#4 0x56351413f998 ExtensionWebUI::HandleChromeURLOverride()
#5 0x7f6d20545df9 content::BrowserURLHandlerImpl::RewriteURLIfNecessary()
#6 0x5635154aaf9b GetIndexOfExistingTab()
#7 0x5635154aae68 ShowSingletonTabOverwritingNTP()
*/
}

void BraveSyncController::InitJsLib(const bool &setup_new_sync) {
LOG(ERROR) << "TAGAB BraveSyncController::InitJsLib";
//  DCHECK(nullptr != sync_js_layer_);
  if (!sync_js_layer_) {
    LoadJsLibPseudoTab();
    return;
  }

if ( (!sync_prefs_->GetSeed().empty() && !sync_prefs_->GetThisDeviceName().empty()) ||
    setup_new_sync) {
  LOG(ERROR) << "TAGAB BraveSyncController::InitJsLib sync is active or setup_new_sync";
  sync_js_layer_->InitJsLib();
} else {
  LOG(ERROR) << "TAGAB BraveSyncController::InitJsLib sync is NOT active";
}

}

// void BraveSyncController::CallJsLib(const std::string &command) {
// LOG(ERROR) << "TAGAB BraveSyncController::CallJsLib";
//   DCHECK(nullptr != sync_js_layer_);
//   if (!sync_js_layer_) {
//     return;
//   }
// DCHECK(false);
//   sync_js_layer_->RunCommandStr(command, "", "", "", "");
// }

void BraveSyncController::CallJsLibBV(const base::Value &command,
  const base::Value &arg1, const base::Value &arg2, const base::Value &arg3,
  const base::Value &arg4) {
//BV means with base::Value
LOG(ERROR) << "TAGAB BraveSyncController::CallJsLibBV";
  DCHECK(nullptr != sync_js_layer_);
  if (!sync_js_layer_) {
    return;
  }

  const std::vector<const base::Value*> args = {&command, &arg1, &arg2, &arg3, &arg4};
  sync_js_layer_->RunCommandBV(args);
}

void BraveSyncController::CallJsLibStr(const std::string &command,
  const std::string &arg1, const std::string &arg2, const std::string &arg3,
  const std::string &arg4) {
//BV means with base::Value
LOG(ERROR) << "TAGAB BraveSyncController::CallJsLibStr";
  DCHECK(nullptr != sync_js_layer_);
  if (!sync_js_layer_) {
    return;
  }

  sync_js_layer_->RunCommandStr(command, arg1, arg2, arg3, arg4);
}

void BraveSyncController::SetupJsLayer(ISyncJsLayer *sync_js_layer) {
  LOG(ERROR) << "TAGAB BraveSyncController::SetupJsLayer sync_js_layer=" << sync_js_layer;
  LOG(ERROR) << "TAGAB BraveSyncController::SetupJsLayer this->sync_js_layer_=" << this->sync_js_layer_;
  DCHECK(sync_js_layer);
  DCHECK(sync_js_layer_ == nullptr);

  sync_js_layer_ = sync_js_layer;
}

void BraveSyncController::SetupUi(ISyncUI *sync_ui) {
  LOG(ERROR) << "TAGAB BraveSyncController::SetupUi sync_ui=" << sync_ui;

  DCHECK(sync_ui);
  DCHECK(sync_ui_ == nullptr);

  sync_ui_ = sync_ui;
}

void BraveSyncController::OnJsLibMessage(const std::string &message, const base::ListValue* args) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnJsLibMessage, message=" << message;

  // AB: Need all args data here

  if (message == "words_to_bytes_done") {
    OnWordsToBytesDone(args);
  } else if (message == "bytes_to_words_done") {
    OnBytesToWordsDone(args);
  }
  else if (message == "get-init-data") {
    ;
  } else if (message == "got-init-data") {
    OnGotInitData(args);
  } else if (message == "save-init-data") {
    OnSaveInitData(args);
  } else if (message == "sync-ready") {
    OnSyncReady(args);
  } else if (message == "get-existing-objects") {
    OnGetExistingObjects(args);
  } else if (message == "resolved-sync-records") {
    OnResolvedSyncRecords(args);
  }
}

namespace {
// void VecToListValue(const std::vector<char> &v, base::Value &lv) {
//   for (const char &i : v) {
//     lv.GetList().emplace_back(i);
//   }
// }

std::unique_ptr<base::Value>  VecToListValue2(const std::vector<char> &v) {
  if (v.empty()) {
    //return std::unique_ptr<base::Value>(new base::Value());
    return std::make_unique<base::Value>();
  }

  //std::unique_ptr<base::Value> lv(new base::Value(base::Value::Type::LIST));
  auto lv = std::make_unique<base::Value>(base::Value::Type::LIST);
  for (const char &i : v) {
    lv->GetList().emplace_back(i);
  }
  return lv;
}


// void VecToListValue(const std::vector<unsigned int> &v, base::Value &lv) {
//   for (const unsigned int &i : v) {
//     lv.GetList().emplace_back(i);
//   }
// }
}

void BraveSyncController::OnGotInitData(const base::ListValue* args) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnGotInitData";

  //should answer to the lib with words
  //const bool new_persona = true;

  //javascript:callbackList['got-init-data'](null,
  //[105,249,244,1,238,179,5,5,210,210,53,27,68,216,134,56,5,195,164,71,248,143,148,3,160,183,36,195,145,205,62,90],
  //null,
  //{apiVersion: '0', serverUrl: 'https://sync-staging.brave.com', debug: true})
  // config
  base::DictionaryValue config;
  config.SetKey("apiVersion", base::Value("0"));
  config.SetKey("serverUrl", base::Value("https://sync-staging.brave.com"));
  config.SetKey("debug", base::Value(true));

  base::Value command("got-init-data");
  // Have 3 options:
  //   1. Start new chain
  //   2. Connect to other sync chain
  //   3. Already connected (or started) to the chain

  std::unique_ptr<base::Value> lv_seed;
  //base::Value lv_seed(base::Value::Type::LIST);
  //VecToListValue(seed_, lv_seed);
  lv_seed = VecToListValue2(seed_);

  std::vector<char> deviceId;
  // base::Value lv_deviceId(base::Value::Type::LIST);
  // VecToListValue(deviceId, lv_deviceId);
  std::unique_ptr<base::Value> lv_deviceId = VecToListValue2(deviceId);

  CallJsLibBV(command, base::Value(), *lv_seed, *lv_deviceId, config);

  // if (new_persona) {
  //   base::Value lv_seed(base::Value::Type::LIST);
  //   VecToListValue(seed_, lv_seed);
  //   CallJsLibBV(command, base::Value(), lv_seed, base::Value(), config);
  // } else {
  //   std::vector<char> deviceId;
  //   base::Value lv_deviceId(base::Value::Type::LIST);
  //   VecToListValue(deviceId, lv_deviceId);
  //   CallJsLibBV(command, base::Value(), base::Value(), lv_deviceId, config);
  // }
}

void BraveSyncController::OnWordsToBytesDone(const base::ListValue* args) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnWordsToBytesDone";
  LOG(ERROR) << "TAGAB args->GetList().size()=" << args->GetList().size();

if (false) {
  DCHECK(args);
  DCHECK(args->GetList().size() == 1);
  DCHECK(args->GetList()[0].is_list());
  DCHECK(seed_.empty());

  const base::Value::ListStorage &bytes = args->GetList()[0].GetList();
  for(const auto &a : bytes) {
    seed_.push_back(a.GetInt());
  }
}

  DCHECK(seed_str_.empty());
  //Data is binary
  DCHECK(args->GetList()[1].is_blob());
  const base::Value::BlobStorage& bs = args->GetList()[1].GetBlob();
  //  using BlobStorage = std::vector<char>;
  seed_str_.clear();
  for (size_t i = 0; i < bs.size(); ++i) {
    seed_.push_back(bs[i]);

    seed_str_ += base::IntToString(/*base::checked_cast*/static_cast<uint8_t>(bs[i]));
    if (i != bs.size() - 1) {
      seed_str_ += ",";
    }
  }

  //OnGotInitData(nullptr);
  LOG(ERROR) << "TAGAB BraveSyncController::OnWordsToBytesDone: call InitJsLib";
  InitJsLib(true);//Init will cause load of the Script;
}

void BraveSyncController::OnBytesToWordsDone(const base::ListValue* args) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnWordsToBytesDone";
  LOG(ERROR) << "TAGAB args->GetList().size()=" << args->GetList().size();
  LOG(ERROR) << "TAGAB BraveSyncController::OnWordsToBytesDone" <<
    brave::debug::ToPrintableString(*args);

  CHECK(sync_ui_);

  DCHECK(args->GetList()[0].GetString() == "bytes_to_words_done");
  DCHECK(args->GetList()[1].is_string());

  std::string words = args->GetList()[1].GetString();

  sync_ui_->OnHaveSyncWords(words);
}

void BraveSyncController::OnSyncReady(const base::ListValue* args) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnSyncReady:";
  DCHECK(false == sync_initialized_);
  sync_initialized_ = true;


  if (sync_ui_) {
    LOG(ERROR) << "TAGAB BraveSyncController::OnSyncReady: have sync ui, inform state changed";
    // inform WebUI page that data is ready
    // changed this device name and id
    sync_ui_->OnSyncStateChanged();
  } else {
    // it can be ui page is not opened yet
    LOG(ERROR) << "TAGAB BraveSyncController::OnSyncReady: sync_ui_ is null";
  }

  // fetch the records
  RequestSyncData();
}

// Here we query sync lib for the records after initialization (or again later)
void BraveSyncController::RequestSyncData() {
  LOG(ERROR) << "TAGAB BraveSyncController::RequestSyncData:";

  LOG(ERROR) << "TAGAB BraveSyncController::RequestSyncData: sync_prefs_->GetSyncThisDevice()=" << sync_prefs_->GetSyncThisDevice();
  if (!sync_prefs_->GetSyncThisDevice()) {
    LOG(ERROR) << "TAGAB BraveSyncController::RequestSyncData: sync is not enabled for this device";
    return;
  }

  const bool bookmarks = sync_prefs_->GetSyncBookmarksEnabled();
  const bool history = sync_prefs_->GetSyncHistoryEnabled();
  const bool preferences = sync_prefs_->GetSyncSiteSettingsEnabled();

  LOG(ERROR) << "TAGAB BraveSyncController::RequestSyncData: bookmarks="<<bookmarks;
  LOG(ERROR) << "TAGAB BraveSyncController::RequestSyncData: history="<<history;
  LOG(ERROR) << "TAGAB BraveSyncController::RequestSyncData: preferences="<<preferences;

  if (!bookmarks && !history && !preferences) {
    LOG(ERROR) << "TAGAB BraveSyncController::RequestSyncData: none of option is enabled, abort";
    return;
  }

  const int64_t start_at = sync_prefs_->GetTimeLastFetch();
  const int max_records = 300;

  LOG(ERROR) << "TAGAB BraveSyncController::RequestSyncData: start_at="<<start_at;
//bool dbg_ignore_create_device = true; //the logic should rely upon sync_prefs_->GetTimeLastFetch() which is not saved yet
  if (0 == start_at /*&& !dbg_ignore_create_device*/) {
    //SetUpdateDeleteDeviceName(CREATE_RECORD, mDeviceName, mDeviceId, "");
    SendCreateDevice();
    SendAllLocalBookmarks();
    SendAllLocalHistorySites();
  }

  FetchSyncRecords(bookmarks, history, preferences, start_at, max_records);
}

void BraveSyncController::OnSaveInitData(const base::ListValue* args) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnSaveInitData:";
  DCHECK(false == sync_initialized_);

  LOG(ERROR) << "TAGAB: *args=" << brave::debug::ToPrintableString(*args);

  // var seed = arg1;
  // var deviceId = arg2;
  DCHECK(args->GetList()[1].is_string());
  DCHECK(args->GetList()[2].is_string());

  std::string seed = args->GetList()[1].GetString();
  std::string device_id = args->GetList()[2].GetString();

  LOG(ERROR) << "TAGAB BraveSyncController::OnSaveInitData: seed="<<seed;
  LOG(ERROR) << "TAGAB BraveSyncController::OnSaveInitData: device_id="<<device_id;

  if (seed_str_.empty() && !seed.empty()) {
    seed_str_ = seed;
  }

  if (device_id_str_.empty() && !device_id.empty()) {
    device_id_str_ = device_id;
  }

  bool convert_result = base::StringToInt(device_id, &device_id_);
  LOG(ERROR) << "TAGAB BraveSyncController::OnSaveInitData: convert_result="<<convert_result;
  LOG(ERROR) << "TAGAB BraveSyncController::OnSaveInitData: device_id_="<<device_id_;
  DCHECK(convert_result);
  DCHECK(device_id_ >= 0);

  // Check existing values
  LOG(ERROR) << "TAGAB BraveSyncController::OnSaveInitData: GetThisDeviceId()="<<sync_prefs_->GetThisDeviceId();
  LOG(ERROR) << "TAGAB BraveSyncController::OnSaveInitData: GetSeed()="<<sync_prefs_->GetSeed();
  LOG(ERROR) << "TAGAB BraveSyncController::OnSaveInitData: GetThisDeviceName()="<<sync_prefs_->GetThisDeviceName();

  LOG(ERROR) << "TAGAB BraveSyncController::OnSaveInitData: seed_str_="<<seed_str_;

  if (device_name_.empty()) {
    device_name_ = sync_prefs_->GetThisDeviceName();
  }

  //Save
  sync_prefs_->SetThisDeviceId(device_id);
  sync_prefs_->SetSeed(seed_str_);
  sync_prefs_->SetDeviceName(device_name_);//here I can have empty string, why ?
  LOG(ERROR) << "TAGAB BraveSyncController::OnSaveInitData: saved device_id="<<device_id;
  LOG(ERROR) << "TAGAB BraveSyncController::OnSaveInitData: saved seed="<<seed_str_;
  LOG(ERROR) << "TAGAB BraveSyncController::OnSaveInitData: saved device_name_="<<device_name_;

  sync_prefs_->SetSyncThisDevice(true);

  sync_prefs_->SetSyncBookmarksEnabled(true);
  sync_prefs_->SetSyncSiteSettingsEnabled(true);
  sync_prefs_->SetSyncHistoryEnabled(true);

  // //Data is binary
  // DCHECK(args->GetList()[1].is_blob());
  // const base::Value::BlobStorage& bs = args->GetList()[1].GetBlob();
  // //  using BlobStorage = std::vector<char>;
  // for (size_t i = 0; i < bs.size(); ++i) {
  //   seed_.push_back(bs[i]);
  // }
  //
  // seed_ = ;
}

void BraveSyncController::OnGetExistingObjects(const base::ListValue* args) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnGetExistingObjects:";
  /**
   * webview -> browser
   * after sync gets records, it requests the browser's existing objects so sync
   * can perform conflict resolution.
   * isTruncated is true if maxRecords was used and the total number of
   * records exceeds the limit.
   */
//  GET_EXISTING_OBJECTS: _, /* @param {string} categoryName, @param {Array.<Object>} records, @param {lastRecordTimeStamp} number, @param {boolean} isTruncated */

  std::string category_name = args->GetList()[1].GetString();
  std::string records_json = args->GetList()[2].GetString();
  std::string last_record_timestamp = args->GetList()[3].GetString();
  bool is_truncated = args->GetList()[4].GetBool();

  LOG(ERROR) << "TAGAB BraveSyncController::OnGetExistingObjects: category_name="<<category_name;
  LOG(ERROR) << "TAGAB BraveSyncController::OnGetExistingObjects: last_record_timestamp="<<last_record_timestamp;
  LOG(ERROR) << "TAGAB BraveSyncController::OnGetExistingObjects: is_truncated="<<is_truncated;

  //JSON ==> Value
  int error_code_out = 0;
  std::string error_msg_out;
  int error_line_out = 0;
  int error_column_out = 0;
  std::unique_ptr<base::Value> records_v = base::JSONReader::ReadAndReturnError(
    records_json,
    base::JSONParserOptions::JSON_PARSE_RFC,
    &error_code_out,
    &error_msg_out,
    &error_line_out,
    &error_column_out);

  LOG(ERROR) << "TAGAB BraveSyncController::OnGetExistingObjects: records_v.get()="<<records_v.get();
  LOG(ERROR) << "TAGAB BraveSyncController::OnGetExistingObjects: error_msg_out="<<error_msg_out;
  DCHECK(records_v);
  if (!records_v) {
    return;
  }

  LOG(ERROR) << "TAGAB BraveSyncController::OnGetExistingObjects records_v->type()="<< base::Value::GetTypeName(records_v->type());
  //records_v->type()=list
  DCHECK(records_v->is_list());

  // for (const auto &val : records_v->GetList() ) {
  //   LOG(ERROR) << "TAGAB val.type()="<< base::Value::GetTypeName(val.type());
  //   DCHECK(val.is_dict());
  //
  //   for (const auto iter : val.DictItems()) {
  //     LOG(ERROR) << "TAGAB name=" << iter.first;
  //     const auto & iter_val = iter.second;
  //     LOG(ERROR) << "TAGAB type="<< base::Value::GetTypeName(iter_val.type());
  //   }
  // }
  LOG(ERROR) << "TAGAB BraveSyncController::OnGetExistingObjects before ToPrintableString";
  //LOG(ERROR) << "TAGAB BraveSyncController::OnGetExistingObjects" << std::endl << ToPrintableString(*records_v);

  //should:
  // resolve
  // then send data with RESOLVE_SYNC_RECORDS
  // then receive RESOLVED_SYNC_RECORDS

  std::unique_ptr<base::Value> resolvedResponse;

  resolvedResponse = PrepareResolvedResponse(records_v);


  SendResolveSyncRecords(category_name, resolvedResponse.get());
}


void BraveSyncController::OnResolvedSyncRecords(const base::ListValue* args) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnResolvedSyncRecords:";
  /**
 * webview -> browser
 * browser must update its local values with the resolved sync records.
 */
 //RESOLVED_SYNC_RECORDS: _, /* @param {string} categoryName, @param {Array.<Object>} records */;

 std::string category_name = args->GetList()[1].GetString();
 std::string records_json = args->GetList()[2].GetString();

 LOG(ERROR) << "TAGAB BraveSyncController::OnResolvedSyncRecords: category_name="<<category_name;
 LOG(ERROR) << "TAGAB BraveSyncController::OnResolvedSyncRecords: records_json="<<records_json;

 //JSON ==> Value
 int error_code_out = 0;
 std::string error_msg_out;
 int error_line_out = 0;
 int error_column_out = 0;
 std::unique_ptr<base::Value> records_v = base::JSONReader::ReadAndReturnError(
   records_json,
   base::JSONParserOptions::JSON_PARSE_RFC,
   &error_code_out,
   &error_msg_out,
   &error_line_out,
   &error_column_out);

 LOG(ERROR) << "TAGAB BraveSyncController::OnResolvedSyncRecords: records_v.get()="<<records_v.get();
 LOG(ERROR) << "TAGAB BraveSyncController::OnResolvedSyncRecords: error_msg_out="<<error_msg_out;
 DCHECK(records_v);
 if (!records_v) {
   return;
 }

 LOG(ERROR) << "TAGAB BraveSyncController::OnResolvedSyncRecords: ToPrintableString=" << std::endl << brave::debug::ToPrintableString(*records_v);

 // Should apply all records
 /*
 PREFERENCES
 [

    {
       "action":0,
       "device":{
          "name":"Akexey Cube tab"
       },
       "deviceId":{
          "0":0
       },
       "objectData":"device",
       "objectId":{
          "0":241,
          "1":236,
          "2":67,
          "3":241,
          "4":34,
          "5":104,
          "6":69,
          "7":151,
          "8":255,
          "9":186,
          "10":133,
          "11":224,
          "12":22,
          "13":96,
          "14":213,
          "15":67
       },
       "syncTimestamp":1529338799495
    }
 ]
 */

  if (category_name == bravesync::jslib_const::kPreferences) {
    OnResolvedPreferences(category_name, std::move(records_v));
  } else if (category_name == bravesync::jslib_const::kBookmarks) {
    OnResolvedBookmarks(category_name, std::move(records_v));
  } else if (category_name == bravesync::jslib_const::kHistorySites) {
    OnResolvedHistorySites(category_name, std::move(records_v));
  }

} // endof BraveSyncController::OnResolvedSyncRecords

void BraveSyncController::OnResolvedPreferences(const std::string &category_name,
  std::unique_ptr<base::Value> records_v) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnResolvedPreferences:";
/*
TYPE=LIST
[
   TYPE=DICTIONARY
   [
      name=action
      TYPE=INTEGER VALUE=0
      name=device
      TYPE=DICTIONARY
      [
         name=name
         TYPE=STRING VALUE=<Akexey Cube tab>
      ]
      name=deviceId
      TYPE=DICTIONARY
      [
         name=0
         TYPE=INTEGER VALUE=0
      ]
      name=objectData
      TYPE=STRING VALUE=<device>
      name=objectId
      TYPE=DICTIONARY
      [
         name=0
         TYPE=INTEGER VALUE=241
         name=1
         TYPE=INTEGER VALUE=236
         name=10
         TYPE=INTEGER VALUE=133
         name=11
         TYPE=INTEGER VALUE=224
         name=12
         TYPE=INTEGER VALUE=22
         name=13
         TYPE=INTEGER VALUE=96
         name=14
         TYPE=INTEGER VALUE=213
         name=15
         TYPE=INTEGER VALUE=67
         name=2
         TYPE=INTEGER VALUE=67
         name=3
         TYPE=INTEGER VALUE=241
         name=4
         TYPE=INTEGER VALUE=34
         name=5
         TYPE=INTEGER VALUE=104
         name=6
         TYPE=INTEGER VALUE=69
         name=7
         TYPE=INTEGER VALUE=151
         name=8
         TYPE=INTEGER VALUE=255
         name=9
         TYPE=INTEGER VALUE=186
      ]
      name=syncTimestamp
      TYPE=DOUBLE VALUE=1.52934e+12
   ]
]
*/
/*
06-20 14:28:54.548: E/cr_SYNC(20391): DeviceResolver saving into DEVICES_NAMES=devicesNames
06-20 14:28:54.548: E/cr_SYNC(20391): DeviceResolver saving into result.toString()=
{"devices":[{"name":"Akexey Cube tab","objectId":"241, 236, 67, 241, 34, 104, 69, 151, 255, 186, 133, 224, 22, 96, 213, 67","deviceId":"0"},
            {"name":"mi4c2","objectId":"131, 167, 121, 234, 248, 73, 7, 51, 141, 194, 173, 96, 73, 73, 70, 67","deviceId":"2"}]}
*/

    //base::JSONReader
    //json object

SyncDevices sync_devices;

    //then merge to existing

    std::unique_ptr<base::Value> root = std::make_unique<base::Value>(base::Value::Type::DICTIONARY);
    std::unique_ptr<base::Value> arr_devices = std::make_unique<base::Value>(base::Value::Type::LIST);

    for (const auto &val : records_v->GetList() ) {
      // for (const auto iter : dict.DictItems()) {
      // }
      const base::Value *p_name = val.FindPath({"device","name"});
      LOG(ERROR) << "TAGAB OnResolvedPreferences p_name="<<p_name;
      LOG(ERROR) << "TAGAB OnResolvedPreferences p_name->GetString()="<<p_name->GetString();

      const base::Value *p_device_id = val.FindPath({"deviceId","0"});
      LOG(ERROR) << "TAGAB OnResolvedPreferences p_device_id="<<p_device_id;
      LOG(ERROR) << "TAGAB OnResolvedPreferences p_device_id->GetInt()="<<p_device_id->GetInt();

      const base::Value *p_object_data = val.FindKey("objectData");
      LOG(ERROR) << "TAGAB OnResolvedPreferences p_object_data="<<p_object_data;
      LOG(ERROR) << "TAGAB OnResolvedPreferences p_object_data->GetString()="<<p_object_data->GetString();

      //objectId
      const base::Value *p_object_id = val.FindKey("objectId");
      LOG(ERROR) << "TAGAB OnResolvedPreferences p_object_data="<<p_object_id;
      //iterate
      std::string object_id;
      for (int i = 0; i < 16; ++i) {
          //
          const base::Value *p_byte = p_object_id->FindKey({base::IntToString(i)});

          object_id += base::IntToString(p_byte->GetInt());
          if (i != 15) {object_id += ", ";}
      }

      const base::Value *p_sync_timestampt = val.FindKey("syncTimestamp");
      LOG(ERROR) << "TAGAB OnResolvedPreferences p_sync_timestampt="<<p_sync_timestampt;
      LOG(ERROR) << "TAGAB OnResolvedPreferences p_sync_timestampt->GetDouble()="<<p_sync_timestampt->GetDouble();
      double d_sync_timestampt = p_sync_timestampt->GetDouble();
      uint64_t i64_sync_timestampt = base::checked_cast<uint64_t>(d_sync_timestampt);
      LOG(ERROR) << "TAGAB OnResolvedPreferences i64_sync_timestampt="<<i64_sync_timestampt;

      base::Time time = base::Time::FromDoubleT(d_sync_timestampt);
      base::Time timeJs = base::Time::FromJsTime(d_sync_timestampt);
      //BASE_EXPORT std::ostream& operator<<(std::ostream& os, Time time);

      LOG(ERROR) << "TAGAB OnResolvedPreferences time="<<time;
      LOG(ERROR) << "TAGAB OnResolvedPreferences timeJs="<<timeJs;
/*
[65273:65273:0626/232423.730312:ERROR:brave_sync_controller.cc(772)] TAGAB OnResolvedPreferences p_sync_timestampt=0x39f3c4451da0
[65273:65273:0626/232423.730330:ERROR:brave_sync_controller.cc(773)] TAGAB OnResolvedPreferences p_sync_timestampt->GetDouble()=1.53004e+12
[65273:65273:0626/232423.730349:ERROR:brave_sync_controller.cc(776)] TAGAB OnResolvedPreferences i64_sync_timestampt=1530044662354
[65273:65273:0626/232423.730369:ERROR:brave_sync_controller.cc(782)] TAGAB OnResolvedPreferences time=50455-03-09 06:12:33.999 UTC
[65273:65273:0626/232423.730390:ERROR:brave_sync_controller.cc(783)] TAGAB OnResolvedPreferences timeJs=2018-06-26 20:24:22.354 UTC
*/

      //base::Time::To;

      LOG(ERROR) << "TAGAB OnResolvedPreferences object_id="<<object_id;
      sync_devices.devices_.push_back(SyncDevice(p_name->GetString(),
          object_id, base::IntToString(p_device_id->GetInt()), i64_sync_timestampt));
    }

    std::string sync_devices_json = sync_devices.ToJson();
LOG(ERROR) << "TAGAB OnResolvedPreferences sync_devices_json="<<sync_devices_json;

LOG(ERROR) << "TAGAB OnResolvedPreferences before SaveObjectId";
    sync_obj_map_->SaveObjectId(DEVICES_NAMES, sync_devices_json, "");
LOG(ERROR) << "TAGAB OnResolvedPreferences SaveObjectId done";

    // Inform devices list chain has been changed
LOG(ERROR) << "TAGAB OnResolvedPreferences OnSyncStateChanged()";
    if (sync_ui_) {
      sync_ui_->OnSyncStateChanged();
    } else {
LOG(ERROR) << "TAGAB OnResolvedPreferences sync_ui_ is empty, don't call OnSyncStateChanged()";
    }
LOG(ERROR) << "TAGAB OnResolvedPreferences OnSyncStateChanged() done";
}

void BraveSyncController::OnResolvedBookmarks(const std::string &category_name,
  std::unique_ptr<base::Value> records_v) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnResolvedBookmarks:";
}

void BraveSyncController::OnResolvedHistorySites(const std::string &category_name,
  std::unique_ptr<base::Value> records_v) {
  LOG(ERROR) << "TAGAB BraveSyncController::OnResolvedHistorySites:";
}

//////////////////////////



/////

std::unique_ptr<base::Value> BraveSyncController::PrepareResolvedResponse(const std::unique_ptr<base::Value> &records) {

  std::unique_ptr<base::Value> resolvedResponse(new base::Value(base::Value::Type::LIST));
  // result should be list<list<server_record, local_record>>
//using ListStorage = std::vector<Value>;

  for (const base::Value &val : records->GetList() ) {
    LOG(ERROR) << "TAGAB val.type()="<< base::Value::GetTypeName(val.type());
    DCHECK(val.is_dict());

    //std::unique_ptr<base::Value> server_record(std::move(new base::Value(val)));
    //base::Value server_record(val);
    base::Value server_record(val.Clone());

    //std::unique_ptr<base::Value> local_record(std::move(new base::Value(base::Value::Type::EMPTY)));
    base::Value local_record(base::Value::Type::NONE);
    // local_record should be get by server_record->objectId => <local object id> => <local object>

    std::unique_ptr<base::Value> resolvedResponseRow(new base::Value(base::Value::Type::LIST));
    //resolvedResponseRow->GetList().push_back(std::move(*server_record));
    resolvedResponseRow->GetList().push_back(std::move(server_record));
    //resolvedResponseRow->GetList().push_back(std::move(*local_record));
    resolvedResponseRow->GetList().push_back(std::move(local_record));


    resolvedResponse->GetList().push_back(std::move(*resolvedResponseRow));
  }

LOG(ERROR) << "TAGAB BraveSyncController::PrepareResolvedResponse" << std::endl << brave::debug::ToPrintableString(*resolvedResponse);
  return resolvedResponse;
}


void BraveSyncController::SendResolveSyncRecords(
  const std::string &category_name,
  const base::Value* response) {
    CHECK(response);
    base::Value command("resolve-sync-records");
    CallJsLibBV(command, base::Value(), base::Value(category_name), *response, base::Value());
}


//fetch-sync-records
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
        // callbackList['fetch-sync-records'](
        //     null,
        //     ['BOOKMARKS', 'PREFERENCES'],
        //     0,//mLatestRecordTimeStampt,
        //     300);

void BraveSyncController::FetchSyncRecords(const bool &bookmarks,
  const bool &history, const bool &preferences, int64_t start_at, int max_records) {
    LOG(ERROR) << "TAGAB BraveSyncController::FetchSyncRecords:";
    DCHECK(bookmarks || history || preferences);
    if (!(bookmarks || history || preferences)) {
      return;
    }

    //base::Value command("fetch-sync-records");
    //base::Value categoryNames(base::Value::Type::LIST);
    std::string categoryNames = "[";
    using namespace bravesync::jslib_const;
    if (history) {
      categoryNames += "\"";
      categoryNames += kHistorySites;//"HISTORY_SITES";
      categoryNames += "\",";
    }
    if (bookmarks) {
      categoryNames += "\"";
      categoryNames += kBookmarks;//"BOOKMARKS";
      categoryNames += "\",";
    }
    if (preferences) {
      categoryNames += "\"";
      categoryNames += kPreferences;//"PREFERENCES";
      categoryNames += "\",";
    }
    if (categoryNames.length() > 1) {
      categoryNames.resize(categoryNames.length() - 1);
    }
    categoryNames += "]";

    //CallJsLibBV(command, base::Value(), categoryNames, base::Value(start_at), base::Value(max_records));
    // I cannot use int64 -> Base::Value, I should use strings

    std::stringstream ss_start_at;
    ss_start_at << start_at;
    std::stringstream ss_max_records;
    ss_max_records << max_records;
    CallJsLibStr("fetch-sync-records", "", categoryNames, ss_start_at.str() , ss_max_records.str());
}

void BraveSyncController::SendCreateDevice() {
  LOG(ERROR) << "TAGAB BraveSyncController::SendCreateDevice";
  //SetUpdateDeleteDeviceName(CREATE_RECORD, mDeviceName, mDeviceId, "");

  std::string deviceName = sync_prefs_->GetThisDeviceName();
  std::string objectId = bravesync::tools::GenerateObjectId();
  LOG(ERROR) << "TAGAB BraveSyncController::SendCreateDevice deviceName=" << deviceName;
  LOG(ERROR) << "TAGAB BraveSyncController::SendCreateDevice objectId="<<objectId;
  std::string deviceId = sync_prefs_->GetThisDeviceId();
  CHECK(!deviceId.empty());

  std::stringstream request;
  request << "[";
  //CreateDeviceCreationRecord(deviceName, objectId, action, deviceId);
  std::string action = CREATE_RECORD;
  std::string stmp = CreateDeviceCreationRecord(deviceName, objectId, action, deviceId);
  LOG(ERROR) << "TAGAB BraveSyncController::SendCreateDevice objectId="<<stmp;
  request << stmp;
  request << "]";

  std::vector<std::string> ids;
  //SendSyncRecords(SyncRecordType.PREFERENCES, request, action, ids);
  SendSyncRecords(SyncRecordType_PREFERENCES, request.str(), action, ids);
}

void BraveSyncController::SendSyncRecords(const std::string &recordType,
  const std::string &recordsJSON,
  const std::string &action,
  const std::vector<std::string> &ids
) {

  LOG(ERROR) << "TAGAB BraveSyncController::SendSyncRecords";
  LOG(ERROR) << "TAGAB recordType=" << recordType;
  LOG(ERROR) << "TAGAB recordsJSON=" << recordsJSON;
  LOG(ERROR) << "TAGAB action=" << action;

/*
if (!mSyncIsReady.IsReady()) {
    return;
}
SaveGetDeleteNotSyncedRecords(recordType, action, ids, NotSyncedRecordsOperation.AddItems);
StringBuilder script = new StringBuilder("javascript:callbackList['send-sync-records'](null, '");
script.append(recordType).append("'");
script.append(", ").append(recordsJSON).append(")");
Log.i(TAG, "SendSyncRecords: " + recordsJSON);
CallScript(script);
*/
  if (!sync_initialized_) {
    LOG(ERROR) << "TAGAB BraveSyncController::SendSyncRecords sync is not initialized";
    DCHECK(false);
    return;
  }

  //what is this?
  SaveGetDeleteNotSyncedRecords(recordType, action, ids, NotSyncedRecordsOperation::AddItems);
  //^ for now this is a

/*
void CallJsLibStr(const std::string &command,
  const std::string &arg1, const std::string &arg2, const std::string &arg3,
  const std::string &arg4);
*/



  CallJsLibStr("send-sync-records", std::string(), recordType, recordsJSON, std::string());
}

std::vector<std::string> BraveSyncController::SaveGetDeleteNotSyncedRecords(
  const std::string &recordType, const std::string &action,
  const std::vector<std::string> &ids,
  NotSyncedRecordsOperation operation) {
  LOG(ERROR) << "TAGAB BraveSyncController::SaveGetDeleteNotSyncedRecords, early quit";
return std::vector<std::string>();

  LOG(ERROR) << "TAGAB recordType=" << recordType;
  LOG(ERROR) << "TAGAB action=" << action;
  LOG(ERROR) << "TAGAB operation=" << operation;

  if (operation != NotSyncedRecordsOperation::GetItems && 0 == ids.size()) {
    return std::vector<std::string>();
  }

  std::string recordId = recordType + action;
  LOG(ERROR) << "TAGAB recordId=" << recordId;

  //Complex logic for sync

  return std::vector<std::string>();
/*
if (NotSyncedRecordsOperation.GetItems != operation && 0 == ids.size()) {
    return null;
}
String recordId = recordType + action;
ArrayList<String> existingList = GetNotSyncedRecords(recordId);
if (NotSyncedRecordsOperation.GetItems == operation) {
    return existingList;
} else if (NotSyncedRecordsOperation.AddItems == operation) {
    for (String id: ids) {
        if (!existingList.contains(id)) {
            existingList.add(id);
        }
    }
} else if (NotSyncedRecordsOperation.DeleteItems == operation) {
    boolean listChanged = false;
    boolean clearLocalDb = action.equals(DELETE_RECORD);
    for (String id: ids) {
        boolean itemRemoved = existingList.remove(id);
        if (!listChanged) {
            listChanged = itemRemoved;
        }
        // Delete corresponding objectIds
        if (clearLocalDb) {
            nativeDeleteByLocalId(id);
        }
    }
    if (!listChanged) {
        return null;
    }
}

SaveNotSyncedRecords(recordId, existingList);

return null;

*/

}

void BraveSyncController::SendAllLocalBookmarks() {
  LOG(ERROR) << "TAGAB BraveSyncController::SendAllLocalBookmarks";
}

void BraveSyncController::SendAllLocalHistorySites() {
  LOG(ERROR) << "TAGAB BraveSyncController::SendAllLocalHistorySites";
}

std::string BraveSyncController::CreateDeviceCreationRecord(
  const std::string &deviceName,
  const std::string &objectId,
  const std::string &action,
  const std::string &deviceId) {
  LOG(ERROR) << "TAGAB BraveSyncController::CreateDeviceCreationRecord";
/*
assert !deviceName.isEmpty();
if (deviceName.isEmpty()) {
    return new StringBuilder(deviceName);
}
StringBuilder record = new StringBuilder("{ action: ").append(action).append(", ");
record.append("deviceId: [").append(deviceId).append("], ");
record.append("objectId: [").append(objectId).append("], ");
record.append(SyncObjectData.DEVICE).append(": { name: \"").append(replaceUnsupportedCharacters(deviceName)).append("\"}}");

//Log.i(TAG, "!!!device record == " + record);
return record;
*/
  DCHECK(!deviceName.empty());
  if (deviceName.empty()) {
    return std::string();
  }

  std::stringstream ss;
  ss << "{ \"action\": " << action << ", ";
  ss << "\"deviceId\": [" << deviceId << "], ";
  ss << "\"objectId\": [" << objectId << "], ";
  ss << "\"" << SyncObjectData_DEVICE << "\"" << ": { \"name\": \"" << bravesync::tools::replaceUnsupportedCharacters(deviceName) << "\"}}";

  LOG(ERROR) << "TAGAB BraveSyncController::CreateDeviceCreationRecord ss.str()=" << ss.str();

  return ss.str();
}

void BraveSyncController::SetUpdateDeleteDeviceName(
  const std::string &action,
  const std::string &deviceName,
  const std::string &deviceId,
  const std::string &objectId) {
  LOG(ERROR) << "TAGAB BraveSyncController::SetUpdateDeleteDeviceName";
  LOG(ERROR) << "TAGAB action=" << action;
  LOG(ERROR) << "TAGAB deviceName=" << deviceName;
  LOG(ERROR) << "TAGAB deviceId=" << deviceId;
  LOG(ERROR) << "TAGAB objectId=" << objectId;

//  #error 1;
  /*
  if (action.equals(CREATE_RECORD)) {
      objectId = GenerateObjectId("deviceName");
  }
  assert !objectId.isEmpty();
  if (objectId.isEmpty()) {
      return;
  }
  StringBuilder request = new StringBuilder("[");
  request.append(CreateDeviceCreationRecord(deviceName, objectId, action, deviceId)).append("]");
  ArrayList<String> ids = new ArrayList<String>();
  //ids.add(id);
  //Log.i(TAG, "!!!device operation request: " + request.toString());
  SendSyncRecords(SyncRecordType.PREFERENCES, request, action, ids);
  */

  std::string objectIdCopy;
  if (action == CREATE_RECORD) {
    objectIdCopy = this->GenerateObjectId2("deviceName");
  } else {
    objectIdCopy = objectId;
  }

  std::stringstream request;
  request << "[";
  DCHECK(!objectIdCopy.empty());
  //std::string action = CREATE_RECORD;
  std::string stmp = CreateDeviceCreationRecord(deviceName, objectIdCopy, action, deviceId);
  LOG(ERROR) << "TAGAB BraveSyncController::SetUpdateDeleteDeviceName stmp="<<stmp;
  request << stmp;
  request << "]";

  std::vector<std::string> ids;
  SendSyncRecords(SyncRecordType_PREFERENCES, request.str(), action, ids);
  ;
}

std::string BraveSyncController::GenerateObjectId2(const std::string &local_id) {
// String res = GetObjectId(localId);
// if (0 != res.length()) {
//     return res;
// }

  std::string res = sync_obj_map_->GetObjectIdByLocalId(local_id);
  if (!res.empty()) {
    return res;
  }

  return bravesync::tools::GenerateObjectId();
}
