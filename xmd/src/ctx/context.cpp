#include "ctx/context.h"

namespace xmd {
    context& context::per_thread() {
        std::lock_guard guard(access_mut);

        auto id = std::this_thread::get_id();
        if (auto iter = thread_contexts.find(id); iter != thread_contexts.end()) {
            auto& thread_ctx = *iter->second;
            return thread_ctx;
        }
        else {
            auto& thread_ctx_ptr = thread_contexts[id];
            thread_ctx_ptr = std::make_unique<context>();
            return *thread_ctx_ptr;
        }
    }
}