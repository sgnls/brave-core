/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_SYNC_CONTROLLER_H_
#define BRAVE_COMPONENTS_SYNC_CONTROLLER_H_

#include "base/memory/singleton.h"
#include "base/macros.h"
#include "brave/browser/ui/webui/sync/isync_js_layer.h"
#include "chrome/browser/ui/browser_list_observer.h"

class Browser;

class BraveSyncController;
class ISyncJsLayer;
class ISyncUI;

class SyncDevices;

namespace bravesync {
namespace prefs {
  class BraveSyncPrefs;
}
}

namespace brave_sync_storage {
  class BraveSyncObjMap;
}

class BraveSyncControllerFactory {
public:
  static BraveSyncControllerFactory* GetInstance();
  static BraveSyncController *CreateController();
private:
  friend struct base::DefaultSingletonTraits<BraveSyncControllerFactory>;
  //friend struct base::DefaultSingletonTraits<BraveSyncController>;
  /*Not good AB*///friend class base::Singleton<BraveSyncController, base::DefaultSingletonTraits<BraveSyncController>, BraveSyncController>;

};

struct BraveSyncSettings;
//struct DeviceInChain;



//TODO, AB: ? rename  IBraveSyncController => BraveSyncController
//                    BraveSyncController => BraveSyncControllerImpl ?
class IBraveSyncController {
public:
  virtual void OnSetupSyncHaveCode(const std::string &sync_words,
    const std::string &device_name) = 0;

  virtual void OnSetupSyncNewToSync(const std::string &device_name) = 0;

  virtual void OnDeleteDevice(const std::string &device_id) = 0;
  virtual void OnResetSync() = 0;

  virtual void GetSettings(BraveSyncSettings &settings) = 0;
  //virtual void GetDevices(std::vector<DeviceInChain> &devices) = 0;
  //virtual void GetDevices(std::vector<SyncDevice> &devices) = 0;
  virtual void GetDevices(SyncDevices &devices) = 0;

  virtual void GetSyncWords() = 0;
  //virtual std::string GetSyncQrCodeBase64() = 0;
  virtual std::string GetSeed() = 0;

  virtual void SetupUi(ISyncUI *sync_ui) = 0;


};

class BraveSyncController : public IBraveSyncController,
                            public ISyncJsLayerResponseReceiver,
                            public BrowserListObserver {
public:
  BraveSyncController();
  ~BraveSyncController() override;

  static BraveSyncController* GetInstance();

  // BrowserListObserver overrides:
  void OnBrowserAdded(Browser* browser) override;
  void OnBrowserSetLastActive(Browser* browser) override;


  // IBraveSyncController messages from UI
  void OnSetupSyncHaveCode(const std::string &sync_words,
    const std::string &device_name) override;
  void OnSetupSyncNewToSync(const std::string &device_name) override;
  void OnDeleteDevice(const std::string &device_id) override;
  void OnResetSync() override;
  void GetSettings(BraveSyncSettings &settings) override;
  //void GetDevices(std::vector<DeviceInChain> &devices) override;
  void GetDevices(SyncDevices &devices) override;
  void GetSyncWords() override;
  //std::string GetSyncQrCodeBase64() override;
  std::string GetSeed() override;
  ////////////////////////////////////////////////////////////////

  void SetupJsLayer(ISyncJsLayer *sync_js_layer);
  void SetupUi(ISyncUI *sync_ui) override; //TODO, AB: why SetupJsLayer is not overridden?

private:
  DISALLOW_COPY_AND_ASSIGN(BraveSyncController);
  //friend struct base::DefaultSingletonTraits<BraveSyncControllerFactory>;
  friend struct base::DefaultSingletonTraits<BraveSyncController>;

  void LoadJsLibPseudoTab();

  void InitJsLib(const bool &setup_new_sync); // setup_new_sync: either "new to sync" or "have sync code"
  //void CallJsLib(const std::string &command);
  void CallJsLibBV(const base::Value &command,
    const base::Value &arg1, const base::Value &arg2, const base::Value &arg3,
    const base::Value &arg4);
  void CallJsLibStr(const std::string &command,
    const std::string &arg1, const std::string &arg2, const std::string &arg3,
    const std::string &arg4);

  // ISyncJsLayerResponseReceiver methods
  void OnJsLibMessage(const std::string &message, const base::ListValue* args) override;
  void OnGotInitData(const base::ListValue* args);
  void OnWordsToBytesDone(const base::ListValue* args);
  void OnBytesToWordsDone(const base::ListValue* args);
  void OnSyncReady(const base::ListValue* args);
  void OnSaveInitData(const base::ListValue* args);
  void OnGetExistingObjects(const base::ListValue* args);
  void OnResolvedSyncRecords(const base::ListValue* args);

  void OnResolvedPreferences(const std::string &category_name,
    std::unique_ptr<base::Value> records_v);
  void OnResolvedBookmarks(const std::string &category_name,
    std::unique_ptr<base::Value> records_v);
  void OnResolvedHistorySites(const std::string &category_name,
    std::unique_ptr<base::Value> records_v);

  void RequestSyncData();
  void FetchSyncRecords(const bool &bookmarks,
    const bool &history, const bool &preferences, int64_t start_at, int max_records);

  std::unique_ptr<base::Value> PrepareResolvedResponse(const std::unique_ptr<base::Value> &records);
  void SendResolveSyncRecords(const std::string &category_name, const base::Value* response);

  void SendCreateDevice();
  void SendAllLocalBookmarks();
  void SendAllLocalHistorySites();

  std::string CreateDeviceCreationRecord(
    const std::string &deviceName,
    const std::string &objectId,
    const std::string &action,
    const std::string &deviceId);

  //void SendSyncRecords( SyncRecordType_PREFERENCES, request, action, ids);
  //public void SendSyncRecords(String recordType, StringBuilder recordsJSON, String action, ArrayList<String> ids)
  void SendSyncRecords(const std::string &recordType,
    const std::string &recordsJSON,
    const std::string &action,
    const std::vector<std::string> &ids  );

  //private synchronized ArrayList<String> SaveGetDeleteNotSyncedRecords(String recordType, String action, ArrayList<String> ids, NotSyncedRecordsOperation operation)
  enum NotSyncedRecordsOperation {
    GetItems,
    AddItems,
    DeleteItems
  };

  //public void SetUpdateDeleteDeviceName(String action, String deviceName, String deviceId, String objectId) {
  void SetUpdateDeleteDeviceName(
    const std::string &action,
    const std::string &deviceName,
    const std::string &deviceId,
    const std::string &objectId);

  std::vector<std::string> SaveGetDeleteNotSyncedRecords(
    const std::string &recordType, const std::string &action,
    const std::vector<std::string> &ids,
    NotSyncedRecordsOperation operation);

  std::string GenerateObjectId2(const std::string &local_id);

  ISyncUI *sync_ui_;  // need this to send messages to SyncWebUi

  ISyncJsLayer *sync_js_layer_;
  std::vector<char> seed_;  // between OnWordsToBytesDone => InitJsLib

  std::string seed_str_; //Should be saved between <> to OnSaveInitData
  std::string device_id_str_;

  bool sync_initialized_;
  int32_t device_id_;

  // This should be used only for passing
  // between OnSetupSyncHaveCode or OnSetupSyncNewToSync to OnSaveInitData
  std::string device_name_;

  // Not sure, should we save this or we can query it each time? Android?
  //load each time
  //std::vector<DeviceInChain> devices_;

  std::auto_ptr<bravesync::prefs::BraveSyncPrefs> sync_prefs_;

  std::unique_ptr<BraveSyncSettings> settings_;

  std::unique_ptr<brave_sync_storage::BraveSyncObjMap> sync_obj_map_;
};



#endif //BRAVE_COMPONENTS_SYNC_CONTROLLER_H_
