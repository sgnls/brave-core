#ifndef BRAVE_BROWSER_UI_WEBUI_SYNC_I_SYNC_JS_LAYER_H_
#define BRAVE_BROWSER_UI_WEBUI_SYNC_I_SYNC_JS_LAYER_H_

namespace base {
  class ListValue;
  class Value;
}  // namespace base

class ISyncJsLayer {
public:
  virtual void InitJsLib() = 0;
  // virtual void RunCommand1(const std::string &command, const std::string &arg1,
  //   const std::string &arg2, const std::string &arg3, const std::string &arg4) = 0;

  virtual void RunCommandBV(const std::vector<const base::Value*> &args) = 0;

  virtual void RunCommandStr(const std::string &command,
    const std::string &arg1, const std::string &arg2, const std::string &arg3,
    const std::string &arg4) = 0;
};

class ISyncJsLayerResponseReceiver {
public:
  virtual void OnJsLibMessage(const std::string &message, const base::ListValue* args) = 0;
};

#endif //BRAVE_BROWSER_UI_WEBUI_SYNC_I_SYNC_JS_LAYER_H_
