#include "brave/browser/ui/webui/sync/sync_js_layer.h"
#include "base/values.h"
//#include "chrome/browser/ui/webui/chrome_web_ui.h"-no such files
#include "content/public/browser/web_ui_message_handler.h"
#include "base/bind.h"

#include "brave/components/brave_sync/brave_sync_controller.h"

SyncJsLayer::SyncJsLayer(content::WebUI* web_ui, const std::string& host,
    const std::string& js_file, int js_resource_id, int html_resource_id)
    : BasicUI(web_ui, host, js_file, js_resource_id, html_resource_id),
    init_js_lib_invoked_(false),
    page_loaded_(false),
    need_init_js_lib_(false) {

  // Register callback handler.
  this->web_ui()->RegisterMessageCallback("addNumbers",
     base::Bind(&SyncJsLayer::AddNumbers,
                base::Unretained(this)));

/*
typedef base::Callback<void(const base::ListValue*)> MessageCallback;
void WebUIImpl::RegisterMessageCallback(const std::string &message,
                                        const MessageCallback& callback) {


*/

  this->web_ui()->RegisterMessageCallback("HandleMessage",
     base::Bind(&SyncJsLayer::HandleMessage,
                base::Unretained(this)));



   response_receiver_ = BraveSyncController::GetInstance();
   BraveSyncController::GetInstance()->SetupJsLayer(this);
}

SyncJsLayer::~SyncJsLayer() {
}

void SyncJsLayer::AddNumbers(const base::ListValue* args) {
LOG(ERROR) << "TAGAB SyncJsLayer::AddNumbers";
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

  LOG(ERROR) << "TAGAB SyncJsLayer::AddNumbers page_loaded_=" << page_loaded_;
  LOG(ERROR) << "TAGAB SyncJsLayer::AddNumbers need_init_js_lib_=" << need_init_js_lib_;
  LOG(ERROR) << "TAGAB SyncJsLayer::AddNumbers init_js_lib_invoked_=" << init_js_lib_invoked_;

  page_loaded_ = true;
  if (need_init_js_lib_) {
    web_ui()->CallJavascriptFunctionUnsafe("hello_world.initJsLib");
    init_js_lib_invoked_ = true;
  }
}

void SyncJsLayer::InitJsLib() {
LOG(ERROR) << "TAGAB SyncJsLayer::InitJsLib";
LOG(ERROR) << "TAGAB SyncJsLayer::InitJsLib page_loaded_=" << page_loaded_;
LOG(ERROR) << "TAGAB SyncJsLayer::InitJsLib need_init_js_lib_=" << need_init_js_lib_;
LOG(ERROR) << "TAGAB SyncJsLayer::InitJsLib init_js_lib_invoked_=" << init_js_lib_invoked_;


  DCHECK(init_js_lib_invoked_ == false);
  if (page_loaded_) {
    web_ui()->CallJavascriptFunctionUnsafe("hello_world.initJsLib");
    init_js_lib_invoked_ = true;
      // ^- when is called right after browser start
      // [41447:41447:0703/141816.983776:ERROR:CONSOLE(1)] "Uncaught ReferenceError: hello_world is not defined", source:  (1)
  } else {
    DCHECK(need_init_js_lib_ == false);
    need_init_js_lib_ = true;
  }
}

void SyncJsLayer::HandleMessage(const base::ListValue* args
  //const base::Value* message//,
  //  const base::Value* arg1,
  // const base::Value* arg2, const base::Value* arg3, const base::Value* arg4
)
{
LOG(ERROR) << "TAGAB SyncJsLayer::HandleMessage";
LOG(ERROR) << "TAGAB args->GetSize()="<<args->GetSize();
LOG(ERROR) << "TAGAB args->GetList()[0].GetString()="<<args->GetList()[0].GetString();

for (const auto &val : args->GetList() ) {
  LOG(ERROR) << "TAGAB val.type()="<< base::Value::GetTypeName(val.type());
}


const std::string message = args->GetList()[0].GetString();
if (message == "words_to_bytes_done" || message == "bytes_to_words_done") {
  LOG(ERROR) << "TAGAB args[1].GetString()="<<args->GetList()[1];
} else {
  LOG(ERROR) << "TAGAB args[1].GetString()="<<args->GetList()[1].GetString();
  LOG(ERROR) << "TAGAB args[2].GetString()="<<args->GetList()[2];//.GetString();
  LOG(ERROR) << "TAGAB args[3].GetString()="<<args->GetList()[3].GetString();
  LOG(ERROR) << "TAGAB args[4].GetBool()="<<args->GetList()[4].GetBool();
}


//LOG(ERROR) << "TAGAB message=" << *message;
//LOG(ERROR) << "TAGAB message2";
// LOG(ERROR) << "TAGAB arg1=" << *arg1;
// LOG(ERROR) << "TAGAB arg2=" << *arg2;
// LOG(ERROR) << "TAGAB arg3=" << *arg3;
// LOG(ERROR) << "TAGAB arg4=" << *arg4;

  if (response_receiver_) {
    response_receiver_->OnJsLibMessage(message, args);
  }

}

// void SyncJsLayer::RunCommand1(const std::string &command, const std::string &arg1,
//   const std::string &arg2, const std::string &arg3, const std::string &arg4) {
//   LOG(ERROR) << "TAGAB SyncJsLayer::RunCommand1 ";
//
//   DCHECK(command == "got-init-data" || false);
//
//   base::Value bv_command(command);
//   base::Value bv_arg1(arg1);
//   base::Value bv_arg2(arg2);
//   base::Value bv_arg3(arg3);
//   base::Value bv_arg4(arg4);
//
//   std::vector<const base::Value*> args = { &bv_command, &bv_arg1, &bv_arg2, &bv_arg3, &bv_arg4};
//   //web_ui()->CallJavascriptFunctionUnsafe("hello_world.callJsLib", command, arg1, arg2, arg3, arg4);
//   RunCommand2(args);
// }

void SyncJsLayer::RunCommandBV(const std::vector<const base::Value*> &args) {
  LOG(ERROR) << "TAGAB SyncJsLayer::RunCommand2 ";

  web_ui()->CallJavascriptFunctionUnsafe("hello_world.callJsLib", args);
}

void SyncJsLayer::RunCommandStr(const std::string &command,
  const std::string &arg1, const std::string &arg2, const std::string &arg3,
  const std::string &arg4) {
  LOG(ERROR) << "TAGAB SyncJsLayer::RunCommandStr command=<"<<command<<">";

  // Messages marked as browser -> webview from braveSync/client/constants/messages.js
  DCHECK(command == "got-init-data" ||
    command == "words_to_bytes" ||
    command == "bytes_to_words" ||
    command == "send-sync-records"  ||
    command == "fetch-sync-records"  ||
    command == "fetch-sync-devices"  ||
    command == "resolve-sync-records"  ||
    command == "send-sync-records"  ||
    command == "delete-sync-user"  ||
    command == "delete-sync-category"  ||
    command == "get-bookmarks-base-order"  ||
    command == "get-bookmark-order" );

  base::Value bv_command(command);
  base::Value bv_arg1(arg1);
  base::Value bv_arg2(arg2);
  base::Value bv_arg3(arg3);
  base::Value bv_arg4(arg4);

  std::vector<const base::Value*> args = { &bv_command, &bv_arg1, &bv_arg2, &bv_arg3, &bv_arg4};
  //web_ui()->CallJavascriptFunctionUnsafe("hello_world.callJsLib", command, arg1, arg2, arg3, arg4);
  web_ui()->CallJavascriptFunctionUnsafe("hello_world.callJsLibStr", args);
}
