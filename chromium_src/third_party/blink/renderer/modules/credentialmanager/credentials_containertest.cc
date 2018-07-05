/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/path_service.h"
#include "brave/browser/brave_content_browser_client.h"
#include "brave/common/brave_paths.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/common/chrome_content_client.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/test/browser_test_utils.h"

const char kCredentialsGetTest[] = "/credentialsget.html";
const char kCredentialsStoreTest[] = "/credentialsstore.html";
const char kCredentialsPreventSilentAccessTest[] = "/credentialspreventsilentaccess.html";

class CredentialsContainersTest : public InProcessBrowserTest {
  public:
    void SetUpOnMainThread() override {
      InProcessBrowserTest::SetUpOnMainThread();

      content_client_.reset(new ChromeContentClient);
      content::SetContentClient(content_client_.get());
      browser_content_client_.reset(new BraveContentBrowserClient());
      content::SetBrowserClientForTesting(browser_content_client_.get());
      content::SetupCrossSiteRedirector(embedded_test_server());

      brave::RegisterPathProvider();
      base::FilePath test_data_dir;
      PathService::Get(brave::DIR_TEST_DATA, &test_data_dir);
      embedded_test_server()->ServeFilesFromDirectory(test_data_dir);

      ASSERT_TRUE(embedded_test_server()->Start());
    }

    void TearDown() override {
      browser_content_client_.reset();
      content_client_.reset();
    }

  private:
    std::unique_ptr<ChromeContentClient> content_client_;
    std::unique_ptr<BraveContentBrowserClient> browser_content_client_;
    base::ScopedTempDir temp_user_data_dir_;
};

IN_PROC_BROWSER_TEST_F(CredentialsContainersTest, GetDisabled) {
  GURL url = embedded_test_server()->GetURL(kCredentialsGetTest);
  ui_test_utils::NavigateToURL(browser(), url);
  content::WebContents* contents = browser()->tab_strip_model()->GetActiveWebContents();
  ASSERT_TRUE(content::WaitForLoadStop(contents));
  EXPECT_EQ(url, contents->GetURL());

  bool credentialsGetBlocked;
  ASSERT_TRUE(ExecuteScriptAndExtractBool(
      contents,
      "window.domAutomationController.send(credentialsGetBlocked())",
      &credentialsGetBlocked));
  EXPECT_TRUE(credentialsGetBlocked);
}

IN_PROC_BROWSER_TEST_F(CredentialsContainersTest, StoreDisabled) {
  GURL url = embedded_test_server()->GetURL(kCredentialsStoreTest);
  ui_test_utils::NavigateToURL(browser(), url);
  content::WebContents* contents = browser()->tab_strip_model()->GetActiveWebContents();
  ASSERT_TRUE(content::WaitForLoadStop(contents));
  EXPECT_EQ(url, contents->GetURL());

  bool credentialsStoreBlocked;
  ASSERT_TRUE(ExecuteScriptAndExtractBool(
      contents,
      "window.domAutomationController.send(credentialsStoreBlocked())",
      &credentialsStoreBlocked));
  EXPECT_TRUE(credentialsStoreBlocked);
}

IN_PROC_BROWSER_TEST_F(CredentialsContainersTest, PreventSilentAccessDisabled) {
  GURL url = embedded_test_server()->GetURL(kCredentialsPreventSilentAccessTest);
  ui_test_utils::NavigateToURL(browser(), url);
  content::WebContents* contents = browser()->tab_strip_model()->GetActiveWebContents();
  ASSERT_TRUE(content::WaitForLoadStop(contents));
  EXPECT_EQ(url, contents->GetURL());

  bool credentialsPreventSilentAccess;
  ASSERT_TRUE(ExecuteScriptAndExtractBool(
      contents,
      "window.domAutomationController.send(credentialspreventSilentAccessBlocked())",
      &credentialsPreventSilentAccess));
  EXPECT_TRUE(credentialsPreventSilentAccess);
}
