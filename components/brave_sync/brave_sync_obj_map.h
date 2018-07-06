/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_OBJ_MAP_H_
#define BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_OBJ_MAP_H_

#include <string>

// #include <jni.h>
// #include "../../../../base/android/jni_weak_ref.h"

namespace brave_sync_storage {

// class BraveSyncWorker {
// public:
//     BraveSyncWorker(JNIEnv* env, jobject obj);
//     ~BraveSyncWorker();
//
//     // Register the BraveSyncStorage's native methods through JNI.
//     // static bool RegisterBraveSyncStorage(JNIEnv* env);
//
// private:
//     //JavaObjectWeakGlobalRef weak_java_shields_config_;
// };

class BraveSyncObjMap {
public:
  BraveSyncObjMap();
  ~BraveSyncObjMap();

  std::string GetLocalIdByObjectId(const std::string &object_id);

  std::string GetObjectIdByLocalId(
        //const std::string &obj,
        const std::string &localId);

  void SaveObjectId(
        const std::string &localId,
        const std::string &objectIdJSON,
        const std::string &objectId);
  void DeleteByLocalId(const std::string &localId);
  void Clear();
  void ResetSync(const std::string& key);
};

}  // namespace brave_sync_storage

#endif //BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_OBJ_MAP_H_
