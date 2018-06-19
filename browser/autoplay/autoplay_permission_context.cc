/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/autoplay/autoplay_permission_context.h"

AutoplayPermissionContext::AutoplayPermissionContext(Profile* profile)
    : PermissionContextBase(
          profile,
          CONTENT_SETTINGS_TYPE_AUTOPLAY,
          blink::mojom::FeaturePolicyFeature::kAutoplay) {}

AutoplayPermissionContext::~AutoplayPermissionContext() = default;

bool AutoplayPermissionContext::IsRestrictedToSecureOrigins() const {
  return false;
}
