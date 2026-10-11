#pragma once

#include <orion/InitializationContext.hpp>

namespace orion {

class Loader {
public:
    static void initialize(const orion::InitializationContext& ctx);
};

} // namespace orion
