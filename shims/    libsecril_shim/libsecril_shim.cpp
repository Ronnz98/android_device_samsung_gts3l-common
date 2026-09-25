#include <stdint.h>
#include <stddef.h> // Für size_t
#include <new>

namespace android {
    class MemoryDealer {
    public:
        MemoryDealer(size_t size, const char* name, uint32_t flags);
    };
}

extern "C" {

    void _ZN7android10MemoryDealerC1EjPKcj(void* obj, uint32_t size, const char* name, uint32_t flags) {

        ::new (obj) android::MemoryDealer(static_cast<size_t>(size), name, flags);
    }
}
