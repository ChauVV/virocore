//
//  LazyMaterial_JNI.cpp
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//


#ifndef ANDROID_LAZYMATERIAL_JNI_H
#define ANDROID_LAZYMATERIAL_JNI_H

#include <jni.h>
#include <memory>
#include "VROLazyMaterial.h"
#include "PersistentRef.h"
#include "VROPlatformUtil.h"
#include "Material_JNI.h"

class VROLazyMaterialJNI : public VROLazyMaterial {

public:
    VROLazyMaterialJNI(jobject obj) {
        JNIEnv *env = VROPlatformGetJNIEnv();
        _jobj = env->NewWeakGlobalRef(obj);
    }

    virtual ~VROLazyMaterialJNI() {
        JNIEnv *env = VROPlatformGetJNIEnv();
        env->DeleteWeakGlobalRef(_jobj);
    }

    std::shared_ptr<VROMaterial> get() {
        jobject localObj = VROPlatformGetJNIEnv()->NewLocalRef(_jobj);
        if (localObj == NULL) {
            return nullptr;
        }

        jlong jptr = VROPlatformCallJavaLongFunction(localObj, "get", "()J");
        VROPlatformGetJNIEnv()->DeleteLocalRef(localObj);
        return Material::native(jptr);
    }

private:
    // The corresponding LazyMaterialJni
    jobject _jobj;
};

namespace LazyMaterial {
    inline jlong jptr(std::shared_ptr<VROLazyMaterialJNI> ptr) {
        PersistentRef<VROLazyMaterialJNI> *persistentRef = new PersistentRef<VROLazyMaterialJNI>(ptr);
        return reinterpret_cast<intptr_t>(persistentRef);
    }

    inline std::shared_ptr<VROLazyMaterialJNI> native(jlong ptr) {
        PersistentRef<VROLazyMaterialJNI> *persistentRef = reinterpret_cast<PersistentRef<VROLazyMaterialJNI> *>(ptr);
        return persistentRef->get();
    }
}

#endif //ANDROID_LAZYMATERIAL_JNI_H