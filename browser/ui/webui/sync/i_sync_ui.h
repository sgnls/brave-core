#ifndef BRAVE_BROWSER_UI_WEBUI_SYNC_I_SYNC_UI_H_
#define BRAVE_BROWSER_UI_WEBUI_SYNC_I_SYNC_UI_H_

// TODO, AB: "Ixxxx" is not a Chromium style
// maybe use SyncUI and SyncUIimpl names?
// or delegates even better?

// This is need to send messages to SyncUI from SyncController
// (call methods of SyncUI from SyncController)
class ISyncUI {
public:
  // UI should pull all data and modify the page
  //virtual void ReloadSettings() = 0;
  virtual void OnSyncStateChanged() = 0;

  virtual void OnHaveSyncWords(const std::string &sync_words) = 0;
};

#endif //BRAVE_BROWSER_UI_WEBUI_SYNC_I_SYNC_UI_H_
