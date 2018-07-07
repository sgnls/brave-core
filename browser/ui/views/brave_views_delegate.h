/* This Source Code Form is subject to the terms of the Mozilla Public
+ * License, v. 2.0. If a copy of the MPL was not distributed with this file,
+ * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_VIEWS_BRAVE_VIEWS_DELEGATE_H_
#define BRAVE_BROWSER_UI_VIEWS_BRAVE_VIEWS_DELEGATE_H_

#include "chrome/browser/ui/views/chrome_views_delegate.h"

class BraveViewsDelegate : public ChromeViewsDelegate {
 public:
  BraveViewsDelegate() = default;
  ~BraveViewsDelegate() override = default;
 private:
#if defined(OS_LINUX)
  // ChromeViewsDelegate overrides:
  gfx::ImageSkia* GetDefaultWindowIcon() const override;
#endif

  DISALLOW_COPY_AND_ASSIGN(BraveViewsDelegate);
};

#endif  // BRAVE_BROWSER_UI_VIEWS_BRAVE_VIEWS_DELEGATE_H_
