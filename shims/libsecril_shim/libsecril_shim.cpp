#include <stdint.h>
#include <stddef.h> // Für size_t
#include <new>

namespace android {
    class MemoryDealer {
    public:
        // Entspricht der exakten Signatur in der libbinder.so von Android 13
        MemoryDealer(size_t size, const char* name, uint32_t flags);
    };
}

extern "C" {
    // Das alte Symbol, das der Samsung-RIL-Blob aufruft
    void _ZN7android10MemoryDealerC1EjPKcj(void* obj, uint32_t size, const char* name, uint32_t flags) {
        // Leitet die Parameter (inklusive flags) direkt an den echten Konstruktor weiter
        ::new (obj) android::MemoryDealer(static_cast<size_t>(size), name, flags);
    }
}
