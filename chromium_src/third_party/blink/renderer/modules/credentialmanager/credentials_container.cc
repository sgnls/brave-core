#include "third_party/blink/renderer/modules/credentialmanager/credentials_container.h"

#include "gin/converter.h"
#include "third_party/blink/renderer/bindings/core/v8/script_promise.h"
#include "third_party/blink/renderer/bindings/core/v8/script_promise_resolver.h"
#include "third_party/blink/renderer/platform/bindings/v8_per_isolate_data.h"
#include "v8/include/v8.h"

namespace blink {

  CredentialsContainer::CredentialsContainer() = default;

  ScriptPromise CredentialsContainer::get(
    ScriptState* script_state,
    const CredentialRequestOptions& options) {
      ScriptPromise::InternalResolver resolver(script_state);
      ScriptPromise promise = resolver.Promise();
      v8::Isolate* isolate = V8PerIsolateData::MainThreadIsolate();
      resolver.Resolve(v8::Null(isolate));
      return promise;
  }

  ScriptPromise CredentialsContainer::store(ScriptState* script_state,
                                          Credential* credential) {
    ScriptPromise::InternalResolver resolver(script_state);
    ScriptPromise promise = resolver.Promise();
    v8::Isolate* isolate = V8PerIsolateData::MainThreadIsolate();
    resolver.Resolve(v8::Null(isolate));
    return promise;
  }

  ScriptPromise CredentialsContainer::preventSilentAccess(
    ScriptState* script_state) {
    ScriptPromise::InternalResolver resolver(script_state);
    ScriptPromise promise = resolver.Promise();
    v8::Isolate* isolate = V8PerIsolateData::MainThreadIsolate();
    resolver.Resolve(v8::Null(isolate));
    return promise;
  }

  CredentialsContainer* CredentialsContainer::Create() {
    return new CredentialsContainer();
  }

  ScriptPromise CredentialsContainer::create(
    ScriptState* script_state,
    const CredentialCreationOptions& options,
    ExceptionState& exception_state) {
      ScriptPromise::InternalResolver resolver(script_state);
      ScriptPromise promise = resolver.Promise();
      v8::Isolate* isolate = V8PerIsolateData::MainThreadIsolate();
      resolver.Resolve(v8::Null(isolate));
      return promise;
  }
} // namespace blink
