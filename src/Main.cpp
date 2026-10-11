#include <Macros.hpp>
#include <orion/Loader.hpp>

void orion::Loader::initialize(const orion::InitializationContext& ctx) {
    (void)ctx;
}

extern "C" EXPORT void initialize(const orion::InitializationContext& ctx) {
    orion::Loader::initialize(ctx);
}
