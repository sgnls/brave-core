/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_SYNC_DRIVER_BRAVE_SYNC_SERVICE_BASE_H_
#define BRAVE_COMPONENTS_SYNC_DRIVER_BRAVE_SYNC_SERVICE_BASE_H_

#include "components/sync/driver/sync_service_base.h"

class BraveSyncController;

namespace syncer {

class BraveSyncServiceBase : public SyncServiceBase {
 public:
  BraveSyncServiceBase(std::unique_ptr<SyncClient> sync_client,
                std::unique_ptr<SigninManagerWrapper> signin,
                const version_info::Channel& channel,
                const base::FilePath& base_directory,
                const std::string& debug_identifier);
 ~BraveSyncServiceBase() override;
 private:
   BraveSyncController* brave_sync_controller_;
   //std::unique_ptr<BraveSyncController> brave_sync_controller_;
};

}  // namespace syncer

#endif // BRAVE_COMPONENTS_SYNC_DRIVER_BRAVE_SYNC_SERVICE_BASE_H_
