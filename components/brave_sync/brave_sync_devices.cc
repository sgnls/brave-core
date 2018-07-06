/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_sync/brave_sync_devices.h"
#include "base/values.h"
#include "base/json/json_writer.h"
#include "base/json/json_reader.h"

#include "brave/components/brave_sync/value_debug.h"

#include "base/time/time.h"

SyncDevice::SyncDevice() :
  last_active_ts_(0) {
}

SyncDevice::SyncDevice(const SyncDevice& other) = default;

SyncDevice::SyncDevice(const std::string &name,
  const std::string &object_id,
  const std::string &device_id,
  const double &last_active_ts) :
  name_(name),
  object_id_(object_id),
  device_id_(device_id),
  last_active_ts_(last_active_ts) {
}

SyncDevice::~SyncDevice() = default;

std::unique_ptr<base::Value> SyncDevice::ToValue() const {
  auto val_sync_device = std::make_unique<base::Value>(base::Value::Type::DICTIONARY);

  val_sync_device->SetKey("name", base::Value(name_));
  val_sync_device->SetKey("object_id", base::Value(object_id_));
  val_sync_device->SetKey("device_id", base::Value(device_id_));
  val_sync_device->SetKey("last_active", base::Value(last_active_ts_));

  return val_sync_device;
}

SyncDevices::SyncDevices() = default;
SyncDevices::~SyncDevices() = default;

// {"devices":[{"name":"Akexey Cube tab","objectId":"241, 236, 67, 241, 34, 104, 69, 151, 255, 186, 133, 224, 22, 96, 213, 67","deviceId":"0"},
//             {"name":"mi4c2","objectId":"131, 167, 121, 234, 248, 73, 7, 51, 141, 194, 173, 96, 73, 73, 70, 67","deviceId":"2"}]}
std::string SyncDevices::ToJson() const {
  // devices_ => base::Value => json

  std::string json;
  bool result = base::JSONWriter::WriteWithOptions(
    *this->ToValue(),
    0,//base::JSONWriter::Options::,
    &json);

  DCHECK(result);

  return json;
}

std::unique_ptr<base::Value> SyncDevices::ToValue() const {
  auto val_sync_device = std::make_unique<base::Value>(base::Value::Type::DICTIONARY);

  auto arr_devices = std::make_unique<base::Value>(base::Value::Type::LIST);
  for (const SyncDevice &device : devices_) {
    arr_devices->GetList().push_back(std::move(*device.ToValue()));
  }

  val_sync_device->SetKey("devices", std::move(*arr_devices));

  return val_sync_device;
}

std::unique_ptr<base::Value> SyncDevices::ToValueArrOnly() const {
  auto arr_devices = std::make_unique<base::Value>(base::Value::Type::LIST);
  for (const SyncDevice &device : devices_) {
    arr_devices->GetList().push_back(std::move(*device.ToValue()));
  }

  return arr_devices;
}

void SyncDevices::FromJson(const std::string &str_json) {
  LOG(ERROR) << "TAGAB SyncDevices::FromJson: str_json="<<str_json;

  //JSON ==> Value
  int error_code_out = 0;
  std::string error_msg_out;
  int error_line_out = 0;
  int error_column_out = 0;
  std::unique_ptr<base::Value> records_v = base::JSONReader::ReadAndReturnError(
    str_json,
    base::JSONParserOptions::JSON_PARSE_RFC,
    &error_code_out,
    &error_msg_out,
    &error_line_out,
    &error_column_out);

  LOG(ERROR) << "TAGAB SyncDevices::FromJson: records_v.get()="<<records_v.get();
  LOG(ERROR) << "TAGAB SyncDevices::FromJson: error_msg_out="<<error_msg_out;

  DCHECK(records_v);
  if (!records_v) {
    return;
  }
  LOG(ERROR) << "SyncDevices::FromJson bv_devices: " << brave::debug::ToPrintableString(*records_v);
/*
[53558:53558:0621/170026.637688:ERROR:brave_sync_devices.cc(103)] SyncDevices::FromJson bv_devices: TYPE=DICTIONARY
[
   name=devices
   TYPE=LIST
   [
      TYPE=DICTIONARY
      [
         name=device_id
         TYPE=STRING VALUE=<0>
         name=last_active
         TYPE=STRING VALUE=<>
         name=name
         TYPE=STRING VALUE=<Akexey Cube tab>
         name=object_id
         TYPE=STRING VALUE=<241, 236, 67, 241, 34, 104, 69, 151, 255, 186, 133, 224, 22, 96, 213, 67>
      ]

      TYPE=DICTIONARY
      [
         name=device_id
         TYPE=STRING VALUE=<2>
         name=last_active
         TYPE=STRING VALUE=<>
         name=name
         TYPE=STRING VALUE=<mi4c2>
         name=object_id
         TYPE=STRING VALUE=<131, 167, 121, 234, 248, 73, 7, 51, 141, 194, 173, 96, 73, 73, 70, 67>
      ]

   ]
]
*/
  devices_.clear();
  const base::Value* pv_arr = records_v->FindKey("devices");
  CHECK(pv_arr->is_list());
  for (const base::Value &val : pv_arr->GetList() ) {
// DEPRECATED, use Value::FindPath(path) and Value::GetString() instead.
    std::string name = val.FindKey("name")->GetString();
    std::string object_id = val.FindKey("object_id")->GetString();
    std::string device_id = val.FindKey("device_id")->GetString();
    double last_active = 0;
    const base::Value *v_last_active = val.FindKey("last_active");
    if (v_last_active->is_double()) {
      last_active = v_last_active->GetDouble();
    } else {
      LOG(ERROR) << "TAGAB SyncDevices::FromJson: last_active is not double";
    }

    LOG(ERROR) << "TAGAB SyncDevices::FromJson: name="<<name;
    LOG(ERROR) << "TAGAB SyncDevices::FromJson: object_id="<<object_id;
    LOG(ERROR) << "TAGAB SyncDevices::FromJson: device_id="<<device_id;
    LOG(ERROR) << "TAGAB SyncDevices::FromJson: last_active="<<last_active;
    LOG(ERROR) << "TAGAB SyncDevices::FromJson: base::Time::FromJsTime="<<base::Time::FromJsTime(last_active);

    devices_.push_back(SyncDevice(
      name,
      object_id,
      device_id,
      last_active
    ));
  }

}

void SyncDevices::Merge(const SyncDevice &device, int action) {
  ;
}

const SyncDevice* SyncDevices::GetByDeviceId(const std::string &device_id) {
  LOG(ERROR) << "SyncDevices::GetByDeviceId device_id="<<device_id;

  for (const auto & device: devices_) {
LOG(ERROR) << "SyncDevices::GetByDeviceId device.device_id_="<<device.device_id_;
    if (device.device_id_ == device_id) {
      return &device;
    }
  }

  DCHECK(false) << "Not expected to find no device";
  return nullptr;
}
