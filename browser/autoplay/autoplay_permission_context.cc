/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/autoplay/autoplay_permission_context.h"

#include "chrome/browser/content_settings/tab_specific_content_settings.h"
#include "chrome/browser/permissions/permission_request_id.h"
#include "chrome/common/chrome_features.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "third_party/blink/public/mojom/feature_policy/feature_policy.mojom.h"

AutoplayPermissionContext::AutoplayPermissionContext(Profile* profile)
    : PermissionContextBase(
          profile,
          CONTENT_SETTINGS_TYPE_AUTOPLAY,
          blink::mojom::FeaturePolicyFeature::kAutoplay) {}

AutoplayPermissionContext::~AutoplayPermissionContext() = default;

void AutoplayPermissionContext::UpdateTabContext(
    const PermissionRequestID& id,
    const GURL& requesting_frame,
    bool allowed) {
  TabSpecificContentSettings* content_settings =
      TabSpecificContentSettings::GetForFrame(id.render_process_id(),
                                              id.render_frame_id());
  if (!content_settings)
    return;

  if (!allowed) {
    content_settings->OnContentBlocked(CONTENT_SETTINGS_TYPE_AUTOPLAY);
  }
}

void AutoplayPermissionContext::NotifyPermissionSet(
    const PermissionRequestID& id,
    const GURL& requesting_origin,
    const GURL& embedding_origin,
    const BrowserPermissionCallback& callback,
    bool persist,
    ContentSetting content_setting) {
  PermissionContextBase::NotifyPermissionSet(id, requesting_origin,
                                             embedding_origin, callback,
                                             persist, content_setting);
  // Ask -> Allow
  if (persist && content_setting == CONTENT_SETTING_ALLOW) {
    content::WebContents* web_contents =
      content::WebContents::FromRenderFrameHost(
        content::RenderFrameHost::FromID(id.render_process_id(),
                                         id.render_frame_id()));
     web_contents->GetController().Reload(content::ReloadType::NORMAL, false);
  }
}

bool AutoplayPermissionContext::IsRestrictedToSecureOrigins() const {
  return false;
}
