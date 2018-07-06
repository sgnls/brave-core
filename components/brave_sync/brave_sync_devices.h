/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_DEVICES_H_
#define BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_DEVICES_H_

#include <string>
#include <vector>
#include <memory>

namespace base {
  class Value;
} // namespace base

//TODO, AB: maybe struct and file should have the same name

// struct DeviceInChain {
//   std::string id_;
//   std::string name_;
//   std::string last_active_;
// };

class SyncDevice {
public:
  SyncDevice();
  SyncDevice(const SyncDevice& other);
  SyncDevice(const std::string &name,
    const std::string &object_id,
    const std::string &device_id,
    const double &last_active_ts);
  ~SyncDevice();

  std::unique_ptr<base::Value> ToValue() const;

  std::string name_;
  std::string object_id_;
  std::string device_id_;
  //std::string last_active_;
  double last_active_ts_;
};

class SyncDevices {
public:
   SyncDevices();
   ~SyncDevices();
   std::vector<SyncDevice> devices_;
   std::unique_ptr<base::Value> ToValue() const;
   std::unique_ptr<base::Value> ToValueArrOnly() const;
   std::string ToJson() const;
   void FromJson(const std::string &str_json);
   void Merge(const SyncDevice &device, int action);

   const SyncDevice *GetByDeviceId(const std::string &device_id);
};


#endif //BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_DEVICES_H_
