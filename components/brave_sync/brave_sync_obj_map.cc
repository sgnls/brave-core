/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 //#include "brave_sync_worker.h"
 #include "brave/components/brave_sync/brave_sync_obj_map.h"
 // #include "base/android/jni_android.h"
 // #include "base/android/jni_string.h"
 // #include "jni/BraveSyncWorker_jni.h"
 #include "base/files/file_util.h"
 #include "base/path_service.h"
 #include "third_party/leveldatabase/src/include/leveldb/db.h"

 //#include "components/filesystem/file_system_app.h"
 #include "base/base_paths.h"
 #include "chrome/common/chrome_paths.h"

#include <string>

#include "base/threading/thread.h"
#include "base/threading/thread_restrictions.h" // TODO, AB: remove



namespace brave_sync_storage {

#define DB_FILE_NAME      "brave_sync_db"

leveldb::DB* g_level_db;
static std::mutex* g_pLevel_db_init_mutex = new std::mutex();


// BraveSyncWorker::BraveSyncWorker(JNIEnv* env, jobject obj):
//   weak_java_shields_config_(env, obj) {
// }
//
// BraveSyncWorker::~BraveSyncWorker() {
// }

BraveSyncObjMap::BraveSyncObjMap() {
  ;
}

BraveSyncObjMap::~BraveSyncObjMap() {
  ;
}

void CreateOpenDatabase() {
    if (!g_pLevel_db_init_mutex) {
        return;
    }
    std::lock_guard<std::mutex> guard(*g_pLevel_db_init_mutex);

    if (nullptr == g_level_db) {
        base::FilePath app_data_path;
        //PathService::Get(base::DIR_ANDROID_APP_DATA, &app_data_path);
        //app_data_path = filesystem::FileSystemApp::GetUserDataDir();
//        bool success = PathService::Get(base::DIR_APP_DATA, &app_data_path);
        bool success = PathService::Get(chrome::DIR_USER_DATA, &app_data_path);
        DCHECK(success);
LOG(ERROR) << "TAGAB CreateOpenDatabase success=" << success;
LOG(ERROR) << "TAGAB CreateOpenDatabase app_data_path=" << app_data_path;


//TODO, AB: in profile
        base::FilePath dbFilePath = app_data_path.Append(DB_FILE_NAME);
LOG(ERROR) << "TAGAB CreateOpenDatabase dbFilePath=" << dbFilePath;
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::Status status = leveldb::DB::Open(options, dbFilePath.value().c_str(), &g_level_db);
LOG(ERROR) << "TAGAB CreateOpenDatabase status=" << status.ToString();
        if (!status.ok() || !g_level_db) {
            if (g_level_db) {
                delete g_level_db;
                g_level_db = nullptr;
            }

            LOG(ERROR) << "sync level db open error " << DB_FILE_NAME;
            return;
        }
LOG(ERROR) << "TAGAB DB opened";
    }
}

// base::android::ScopedJavaLocalRef<jstring> JNI_BraveSyncWorker_GetLocalIdByObjectId(JNIEnv*
//       env, const base::android::JavaParamRef<jobject>& jcaller,
//       const base::android::JavaParamRef<jstring>& objectId) {
//     CreateOpenDatabase();
//     if (nullptr == g_level_db) {
//         return base::android::ConvertUTF8ToJavaString(env, "");
//     }
//
//     std::string value;
//     g_level_db->Get(leveldb::ReadOptions(), base::android::ConvertJavaStringToUTF8(objectId), &value);
//
//     return base::android::ConvertUTF8ToJavaString(env, value);
// }

std::string BraveSyncObjMap::GetLocalIdByObjectId(const std::string &objectId) {
base::ScopedAllowBlockingForTesting sab;// TODO, AB: remove
//base::ScopedAllowBlocking sab;// TODO, AB: remove
    CreateOpenDatabase();
    if (nullptr == g_level_db) {
        return "";
    }

    // May return some other Status on an error.
    // virtual Status Get(const ReadOptions& options,
    //                    const Slice& key, std::string* value) = 0;


    std::string value;
    leveldb::Status db_status = g_level_db->Get(leveldb::ReadOptions(), objectId, &value);
    if (!db_status.ok()) {
      LOG(ERROR) << "sync level db get error " << db_status.ToString();
    }

    return value;
}

// base::android::ScopedJavaLocalRef<jstring> JNI_BraveSyncWorker_GetObjectIdByLocalId(JNIEnv* env,
//       const base::android::JavaParamRef<jobject>& obj,
//       const base::android::JavaParamRef<jstring>& localId) {
//     CreateOpenDatabase();
//     if (nullptr == g_level_db) {
//         return base::android::ConvertUTF8ToJavaString(env, "");
//     }
//
//     std::string value;
//     g_level_db->Get(leveldb::ReadOptions(), base::android::ConvertJavaStringToUTF8(localId), &value);
//
//     return base::android::ConvertUTF8ToJavaString(env, value);
// }

std::string BraveSyncObjMap::GetObjectIdByLocalId(
      //const std::string &obj,
      const std::string &localId) {
base::ScopedAllowBlockingForTesting sab;// TODO, AB: remove
    CreateOpenDatabase();
    if (nullptr == g_level_db) {
        return "";
    }

    std::string value;
    leveldb::Status db_status = g_level_db->Get(leveldb::ReadOptions(), localId, &value);

    if (!db_status.ok()) {
      LOG(ERROR) << "sync level db get error " << db_status.ToString();
    }

    return value;
}

// void JNI_BraveSyncWorker_SaveObjectId(JNIEnv* env, const
//       base::android::JavaParamRef<jobject>& jcaller,
//       const base::android::JavaParamRef<jstring>& localId,
//       const base::android::JavaParamRef<jstring>& objectIdJSON,
//       const base::android::JavaParamRef<jstring>& objectId) {
//     CreateOpenDatabase();
//     if (nullptr == g_level_db) {
//         return;
//     }
//     std::string strLocalId = base::android::ConvertJavaStringToUTF8(localId);
//
//     g_level_db->Put(leveldb::WriteOptions(), strLocalId,
//         base::android::ConvertJavaStringToUTF8(objectIdJSON));
//     std::string strObjectId = base::android::ConvertJavaStringToUTF8(objectId);
//     if (0 != strObjectId.size()) {
//         g_level_db->Put(leveldb::WriteOptions(), strObjectId, strLocalId);
//     }
// }

void BraveSyncObjMap::SaveObjectId(
      const std::string &localId,
      const std::string &objectIdJSON,
      const std::string &objectId) {
LOG(ERROR) << "TAGAB BraveSyncObjMap::SaveObjectId - enter";
base::ScopedAllowBlockingForTesting sab;// TODO, AB: remove
    CreateOpenDatabase();
    if (nullptr == g_level_db) {
LOG(ERROR) << "TAGAB BraveSyncObjMap::SaveObjectId nullptr == g_level_db ???";
        return;
    }
//TODO, AB: levelDB transactions, virtual Status Write(const WriteOptions& options, WriteBatch* updates) = 0;
    leveldb::Status db_status = g_level_db->Put(leveldb::WriteOptions(), localId, objectIdJSON);
    if (!db_status.ok()) {
      LOG(ERROR) << "sync level db put error " << db_status.ToString();
    }

    if (0 != objectId.size()) {
        db_status = g_level_db->Put(leveldb::WriteOptions(), objectId, localId);
        if (!db_status.ok()) {
          LOG(ERROR) << "sync level db put error " << db_status.ToString();
        }
    }
LOG(ERROR) << "TAGAB BraveSyncObjMap::SaveObjectId - DONE";
}

// void JNI_BraveSyncWorker_DeleteByLocalId(JNIEnv* env,
//       const base::android::JavaParamRef<jobject>& obj,
//       const base::android::JavaParamRef<jstring>& localId) {
//     CreateOpenDatabase();
//     if (nullptr == g_level_db) {
//         return;
//     }
//
//     std::string strLocalId = base::android::ConvertJavaStringToUTF8(localId);
//     std::string value;
//     g_level_db->Get(leveldb::ReadOptions(), strLocalId, &value);
//
//     g_level_db->Delete(leveldb::WriteOptions(), strLocalId);
//     g_level_db->Delete(leveldb::WriteOptions(), value);
// }
void BraveSyncObjMap::DeleteByLocalId(const std::string &localId) {
base::ScopedAllowBlockingForTesting sab;// TODO, AB: remove
    CreateOpenDatabase();
    if (nullptr == g_level_db) {
        return;
    }

    //std::string strLocalId = base::android::ConvertJavaStringToUTF8(localId);
    std::string value;
    leveldb::Status db_status = g_level_db->Get(leveldb::ReadOptions(), localId, &value);
    if (!db_status.ok()) {
      LOG(ERROR) << "sync level db get error " << db_status.ToString();
    }

    db_status = g_level_db->Delete(leveldb::WriteOptions(), localId);
    if (!db_status.ok()) {
      LOG(ERROR) << "sync level db delete error " << db_status.ToString();
    }
    db_status = g_level_db->Delete(leveldb::WriteOptions(), value);
    if (!db_status.ok()) {
      LOG(ERROR) << "sync level db delete error " << db_status.ToString();
    }
}

// static void JNI_BraveSyncWorker_Clear(JNIEnv* env, const base::android::JavaParamRef<jobject>& obj) {
//     if (g_level_db) {
//         delete g_level_db;
//         g_level_db = nullptr;
//     }
//     if (g_pLevel_db_init_mutex) {
//         delete g_pLevel_db_init_mutex;
//         g_pLevel_db_init_mutex = nullptr;
//     }
// }

void BraveSyncObjMap::Clear() {
    if (g_level_db) {
        delete g_level_db;
        g_level_db = nullptr;
    }
    if (g_pLevel_db_init_mutex) {
        delete g_pLevel_db_init_mutex;
        g_pLevel_db_init_mutex = nullptr;
    }
}

// static void JNI_BraveSyncWorker_ResetSync(JNIEnv* env,
//       const base::android::JavaParamRef<jobject>& obj,
//       const base::android::JavaParamRef<jstring>& key) {
//     CreateOpenDatabase();
//     if (nullptr == g_level_db) {
//         return;
//     }
//     g_level_db->Delete(leveldb::WriteOptions(), base::android::ConvertJavaStringToUTF8(key));
// }

void BraveSyncObjMap::ResetSync(const std::string& key) {
base::ScopedAllowBlockingForTesting sab;// TODO, AB: remove
    CreateOpenDatabase();
    if (nullptr == g_level_db) {
        return;
    }
    g_level_db->Delete(leveldb::WriteOptions(), key);
}

// static
/*bool BraveSyncWorker::RegisterBraveSyncStorage(JNIEnv* env) {
    g_level_db = nullptr;
    return RegisterNativesImpl(env);
}*/

}   // namespace brave_sync_storage
