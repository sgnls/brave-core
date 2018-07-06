/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_sync/values_conv.h"
#include "base/values.h"
#include "brave/components/brave_sync/brave_sync_settings.h"
#include "brave/components/brave_sync/brave_sync_devices.h"

namespace bravesync {

using base::Value;

std::unique_ptr<Value> BraveSyncSettingsToValue(BraveSyncSettings *brave_sync_settings) {
  CHECK(brave_sync_settings);
  auto result = std::make_unique<Value>(Value::Type::DICTIONARY);

  result->SetKey("this_device_name", Value(brave_sync_settings->this_device_name_));
  result->SetKey("sync_this_device", Value(brave_sync_settings->sync_this_device_));
  result->SetKey("sync_bookmarks", Value(brave_sync_settings->sync_bookmarks_));
  result->SetKey("sync_settings", Value(brave_sync_settings->sync_settings_));
  result->SetKey("sync_history", Value(brave_sync_settings->sync_history_));

  result->SetKey("sync_configured", Value(brave_sync_settings->sync_configured_));

  return result;
}

// std::unique_ptr<Value> BraveSyncDevicesToValue(const std::vector<DeviceInChain> &devices) {
//   auto result = std::make_unique<Value>(Value::Type::LIST);
//   result->GetList().reserve(devices.size());
//
//   for(const auto &device : devices) {
//     auto bv_device = std::make_unique<Value>(Value::Type::DICTIONARY);
//     bv_device->SetPath({"id"}, Value(device.id_));
//     bv_device->SetPath({"name"}, Value(device.name_));
//     bv_device->SetPath({"last_active"}, Value(device.last_active_));
//
//     result->GetList().push_back(std::move(*bv_device)); //good
//   }
//
//   return result;
// }

} // namespace bravesync
