#ifndef BRAVE_BROWSER_UI_WEBUI_SYNC_SYNC_UI_H_
#define BRAVE_BROWSER_UI_WEBUI_SYNC_SYNC_UI_H_


#include "base/macros.h"
//#include "content/public/browser/web_ui_controller.h"
#include "brave/browser/ui/webui/basic_ui.h"
#include "brave/browser/ui/webui/sync/i_sync_ui.h"

namespace base {
  class ListValue;
}  // namespace base

class IBraveSyncController;

// The WebUI for chrome://bravesync
class SyncUI :  public BasicUI, //AB: should bot be inherited from BasicUI,
                public ISyncUI
{
  // because when OnPreferenceChanged is invoked, then in complains
  // brave_new_tab is not defined. It displays shields statistics and
  // is not required for sync
 public:
  explicit SyncUI(content::WebUI* web_ui, const std::string& host,
      const std::string& js_file, int js_resource_id, int html_resource_id);
  ~SyncUI() override;

  // should be invoked by controller when we know sync status has been changed
  //OnSyncStateChanged()
  // ISyncUI
  void OnSyncStateChanged() override;

  void OnHaveSyncWords(const std::string &sync_words) override;

 private:
  // Add two numbers together using integer arithmetic.
  void AddNumbers(const base::ListValue* args); //TODO, AB: remove AddNumbers, this is from Chromium example
  void SetupSyncHaveCode(const base::ListValue* args);
  void SetupSyncNewToSync(const base::ListValue* args);

  void PageLoaded(const base::ListValue* args);

  void NeedSyncWords(const base::ListValue* args);
  void NeedSyncQRcode(const base::ListValue* args);

  void DeleteDevice(const base::ListValue* args);
  void ResetSync(const base::ListValue* args);


  // this should grab actual data from controller and update the page
  void LoadSyncSettingsView();

  IBraveSyncController *sync_controller_;

  DISALLOW_COPY_AND_ASSIGN(SyncUI);
};

#endif  // BRAVE_BROWSER_UI_WEBUI_SYNC_SYNC_UI_H_
