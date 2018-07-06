#ifndef BRAVE_BROWSER_UI_WEBUI_SYNC_SYNC_JS_LAYER_H_
#define BRAVE_BROWSER_UI_WEBUI_SYNC_SYNC_JS_LAYER_H_
#pragma once

#include "base/macros.h"
//#include "content/public/browser/web_ui_controller.h"
#include "brave/browser/ui/webui/basic_ui.h"

#include "brave/browser/ui/webui/sync/isync_js_layer.h"

namespace base {
  class ListValue;
  class Value;
}  // namespace base


class ISyncJsLayerResponseReceiver;

// The WebUI for chrome://bravesynclib
class SyncJsLayer : public BasicUI,
                    public ISyncJsLayer { //AB: should bot be inherited from BasicUI,
  // because when OnPreferenceChanged is invoked, then in complains
  // brave_new_tab is not defined. It displays shields statistics and not required for sync
public:
  explicit SyncJsLayer(content::WebUI* web_ui, const std::string& host,
      const std::string& js_file, int js_resource_id, int html_resource_id);
  ~SyncJsLayer() override;

private:
  // Add two numbers together using integer arithmetic.
  void AddNumbers(const base::ListValue* args);
  void HandleMessage(const base::ListValue* args
    //const base::Value* message//, const base::Value* arg1,
//    const base::Value* arg2, const base::Value* arg3, const base::Value* arg4
  );

  // ISyncJsLayer virtual functions
  void InitJsLib() override;
  // void RunCommand1(const std::string &command, const std::string &arg1,
  //   const std::string &arg2, const std::string &arg3, const std::string &arg4) override;
  void RunCommandBV(const std::vector<const base::Value*> &args) override;
  void RunCommandStr(const std::string &command,
    const std::string &arg1, const std::string &arg2, const std::string &arg3,
    const std::string &arg4) override;

  DISALLOW_COPY_AND_ASSIGN(SyncJsLayer);

  ISyncJsLayerResponseReceiver *response_receiver_; //TODO AB: the list of delegates?

  bool init_js_lib_invoked_;
  bool page_loaded_;
  bool need_init_js_lib_;
};

#endif  // BRAVE_BROWSER_UI_WEBUI_SYNC_SYNC_JS_LAYER_H_
