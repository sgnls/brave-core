/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/brave_pages.h"

#include "brave/common/webui_url_constants.h"
#include "chrome/browser/ui/singleton_tabs.h"
#include "url/gurl.h"

#include "base/debug/stack_trace.h"

//////////////////
//TODO, AB: move to separate file
#include "content/public/browser/browser_thread.h"
void DisplayThreadInfo();
void DisplayThreadInfo() {
  using content::BrowserThread;
  LOG(ERROR) << "TAGAB IsThreadInitialized(UI)=" << BrowserThread::IsThreadInitialized(BrowserThread::UI);
  LOG(ERROR) << "TAGAB IsThreadInitialized(IO)=" << BrowserThread::IsThreadInitialized(BrowserThread::IO);
  BrowserThread::ID id = BrowserThread::ID_COUNT;
  bool bKnownThread =  BrowserThread::GetCurrentThreadIdentifier(&id);
  if (bKnownThread) {
    if (id == BrowserThread::UI) {
      LOG(ERROR) << "TAGAB in UI THREAD";
    } else if (id == BrowserThread::IO){
      LOG(ERROR) << "TAGAB in IO THREAD";
    } else {
      LOG(ERROR) << "TAGAB in ??? THREAD";
    }
  } else {
    LOG(ERROR) << "TAGAB UNKNOWN THREAD";
  }
}
//////////////////


namespace brave {

void ShowBravePayments(Browser* browser) {
  ShowSingletonTabOverwritingNTP(
      browser,
      GetSingletonTabNavigateParams(browser, GURL(kBraveUIPaymentsURL)));
}

void ShowBraveSync(Browser* browser) {
LOG(ERROR) << "TAGAB ShowBraveSync";
DisplayThreadInfo();
   ShowSingletonTabOverwritingNTP(
       browser,
      GetSingletonTabNavigateParams(browser, GURL(kBraveUISyncURL)));
      //GetSingletonTabNavigateParams(browser, GURL(kBraveSyncLibURL)));
}

void LoadBraveSyncJsLib(Browser* browser) {
LOG(ERROR) << "TAGAB LoadBraveSyncJsLib";
DisplayThreadInfo();
   ShowSingletonTabOverwritingNTP(
       browser,
      GetSingletonTabNavigateParams(browser, GURL(kBraveSyncLibURL)));
}


}  // namespace brave
