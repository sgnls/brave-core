/* This Source Code Form is subject to the terms of the Mozilla Public
 * + * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * + * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/brave_views_delegate.h"

#if defined(OS_LINUX)

#include "chrome/common/channel_info.h"
#include "chrome/grit/chrome_unscaled_resources.h"
#include "components/version_info/channel.h"
#include "ui/base/resource/resource_bundle.h"

namespace {

int GetWindowIconResourceId() {
#if defined(OFFICIAL_BUILD)
  switch (chrome::GetChannel()) {
    case version_info::Channel::DEV:
      return IDR_PRODUCT_LOGO_128_DEV;
    case version_info::Channel::BETA:
      return IDR_PRODUCT_LOGO_128_BETA;
    case version_info::Channel::CANARY:
      return IDR_PRODUCT_LOGO_128_NIGHTLY;
    default:
      return IDR_PRODUCT_LOGO_128;
  }
#else
  return IDR_PRODUCT_LOGO_128_DEVELOPMENT;
#endif
}

}  // namespace

gfx::ImageSkia* BraveViewsDelegate::GetDefaultWindowIcon() const {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  return rb.GetImageSkiaNamed(GetWindowIconResourceId());
}

#endif  // OS_LINUX

