#include "brave/browser/ui/webui/sync/sync_ui.h"
#include "base/values.h"
//#include "chrome/browser/ui/webui/chrome_web_ui.h"-no such files
#include "content/public/browser/web_ui_message_handler.h"
#include "base/bind.h"
#include "brave/components/brave_sync/brave_sync_controller.h"
#include "brave/components/brave_sync/brave_sync_settings.h"
//^ TODO, AB: alphabetical order for includes

#include "brave/components/brave_sync/brave_sync_devices.h"
#include "brave/components/brave_sync/values_conv.h"

#include "brave/components/brave_sync/value_debug.h"

SyncUI::SyncUI(content::WebUI* web_ui, const std::string& host,
    const std::string& js_file, int js_resource_id, int html_resource_id)
    : BasicUI(web_ui, host, js_file, js_resource_id, html_resource_id) {
LOG(ERROR) << "TAGAB SyncUI::SyncUI CTOR";

  // Register callback handler.
  //TODO, AB: remove addNumbers, it was from the Chromium example of WebUI
  this->web_ui()->RegisterMessageCallback("addNumbers",
     base::Bind(&SyncUI::AddNumbers,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("setupSyncHaveCode",
     base::Bind(&SyncUI::SetupSyncHaveCode,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("setupSyncNewToSync",
     base::Bind(&SyncUI::SetupSyncNewToSync,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("pageLoaded",
     base::Bind(&SyncUI::PageLoaded,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("needSyncWords",
     base::Bind(&SyncUI::NeedSyncWords,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("needSyncQRcode",
     base::Bind(&SyncUI::NeedSyncQRcode,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("deleteDevice",
     base::Bind(&SyncUI::DeleteDevice,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("resetSync",
     base::Bind(&SyncUI::ResetSync,
                base::Unretained(this)));

  sync_controller_ = static_cast<IBraveSyncController*>(BraveSyncController::GetInstance());
  sync_controller_->SetupUi(this);
}

SyncUI::~SyncUI() {
LOG(ERROR) << "TAGAB SyncUI::~SyncUI DTOR";
}


//TODO, AB: remove addNumbers, it was from the Chromium example of WebUI
void SyncUI::AddNumbers(const base::ListValue* args) {
LOG(ERROR) << "SyncUI::AddNumbers";
  int term1, term2;
  if (!args->GetInteger(0, &term1) || !args->GetInteger(1, &term2))
   return;
  //base::FundamentalValue result(term1 + term2);
  base::Value result(term1 + term2);
  web_ui()->CallJavascriptFunctionUnsafe("hello_world.addResult", result);

  // Calling these functions directly is discouraged. It's generally preferred
  // to call WebUIMessageHandler::CallJavascriptFunction, as that has
  // lifecycle controls to prevent calling JavaScript before the page is ready.
  //??web_ui()->CallJavascriptFunction("hello_world.addResult", result);
}

;;;;

void SyncUI::SetupSyncHaveCode(const base::ListValue* args) {
  LOG(ERROR) << "SyncUI::SetupSyncHaveCode";
  std::string sync_words, device_name;
  if (!args->GetString(0, &sync_words) || !args->GetString(1, &device_name))
   return;

  LOG(ERROR) << "SyncUI::SetupSyncHaveCode sync_words=" << sync_words;
  LOG(ERROR) << "SyncUI::SetupSyncHaveCode device_name=" << device_name;

  sync_controller_->OnSetupSyncHaveCode(sync_words, device_name);
}

void SyncUI::SetupSyncNewToSync(const base::ListValue* args) {
  LOG(ERROR) << "SyncUI::SetupSyncNewToSync";
  std::string sync_words, device_name;
  if (!args->GetString(0, &device_name)) {
   return;
  }

  LOG(ERROR) << "SyncUI::SetupSyncHaveCode device_name=" << device_name;

  sync_controller_->OnSetupSyncNewToSync(device_name);
}

void SyncUI::OnSyncStateChanged() {
LOG(ERROR) << "SyncUI::OnSyncStateChanged";
  LoadSyncSettingsView();
}

void SyncUI::OnHaveSyncWords(const std::string &sync_words) {
LOG(ERROR) << "SyncUI::OnHaveSyncWords sync_words="<<sync_words;
  web_ui()->CallJavascriptFunctionUnsafe("hello_world.haveSyncWords", base::Value(sync_words));
}

void SyncUI::LoadSyncSettingsView() {
LOG(ERROR) << "SyncUI::LoadSyncSettingsView";
  //std::vector<DeviceInChain> devices_in_chain;

  BraveSyncSettings settings;
  sync_controller_->GetSettings(settings);

  // std::vector<DeviceInChain> devices;
  // sync_controller_->GetDevices(devices);
  // std::unique_ptr<base::Value> bv_devices = bravesync::BraveSyncDevicesToValue(devices);

  SyncDevices devices;
  sync_controller_->GetDevices(devices);
  std::unique_ptr<base::Value> bv_devices = devices.ToValueArrOnly();

LOG(ERROR) << "SyncUI::LoadSyncSettingsView bv_devices: " << brave::debug::ToPrintableString(*bv_devices);


  // pass  to web UI
  // TODO, AB: rename hello_world.xxxx JS object
  //base::Value result("ou yeah");
  //web_ui()->CallJavascriptFunctionUnsafe("hello_world.showSettings", result);

  std::unique_ptr<base::Value> bv_settings = bravesync::BraveSyncSettingsToValue(&settings);
  web_ui()->CallJavascriptFunctionUnsafe("hello_world.showSettings", *bv_settings, *bv_devices);
}


void SyncUI::PageLoaded(const base::ListValue* args) {
LOG(ERROR) << "SyncUI::PageLoaded";
  LoadSyncSettingsView();
}

void SyncUI::NeedSyncWords(const base::ListValue* args) {
  LOG(ERROR) << "SyncUI::NeedSyncWords";
  /*std::string sync_words = */sync_controller_->GetSyncWords();

  //web_ui()->CallJavascriptFunctionUnsafe("hello_world.haveSyncWords", base::Value(sync_words));
}

void SyncUI::NeedSyncQRcode(const base::ListValue* args) {
  LOG(ERROR) << "SyncUI::NeedSyncQRcode";
  // std::string sync_qr_base64 = sync_controller_->GetSyncQrCodeBase64();
  // web_ui()->CallJavascriptFunctionUnsafe("hello_world.haveSyncQrCode", base::Value(sync_qr_base64));

  std::string seed = sync_controller_->GetSeed();
  web_ui()->CallJavascriptFunctionUnsafe("hello_world.haveSeedForQrCode", base::Value(seed));
}

void SyncUI::DeleteDevice(const base::ListValue* args) {
  LOG(ERROR) << "SyncUI::DeleteDevice args=" << brave::debug::ToPrintableString(*args);

  std::string device_id;
  if (!args->GetString(0, &device_id)) {
    return;
  }
  LOG(ERROR) << "SyncUI::DeleteDevice device_id=" << device_id;
  sync_controller_->OnDeleteDevice(device_id);
}

void SyncUI::ResetSync(const base::ListValue* args) {
  LOG(ERROR) << "SyncUI::ResetSync args=" << brave::debug::ToPrintableString(*args);

  sync_controller_->OnResetSync();
}
