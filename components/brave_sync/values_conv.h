/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_VALUES_CONV_H_
#define BRAVE_COMPONENTS_BRAVE_SYNC_VALUES_CONV_H_

#include <memory>
#include <vector>

namespace base {
  class Value;
}

struct BraveSyncSettings;
//struct DeviceInChain;

namespace bravesync {

std::unique_ptr<base::Value> BraveSyncSettingsToValue(BraveSyncSettings *brave_sync_settings);
//std::unique_ptr<base::Value> BraveSyncDevicesToValue(const std::vector<DeviceInChain> &devices);

} // namespace bravesync

#endif //BRAVE_COMPONENTS_BRAVE_SYNC_VALUES_CONV_H_
