#include <fcntl.h>
#include <new>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

namespace android {
    class MemoryDealer {
    public:
        // Der echte Android 13 Konstruktor erwartet size_t (64-Bit auf arm64)
        MemoryDealer(size_t size, const char* name, uint32_t flags);
    };
}

extern "C" {
    // Dieser Shim fängt das alte 32-Bit-Symbol (j = uint32_t) ab, das Samsungs RIL sucht,
    // und leitet es sicher an den modernen 64-Bit-Konstruktor weiter.
    void* _ZN7android10MemoryDealerC1EjPKcj(void* obj, uint32_t size, const char* name, uint32_t flags) {
        // Explizites Umwandeln auf die vom Android 13 Framework erwartete 64-Bit-Größe
        size_t size64 = static_cast<size_t>(size);

        // Verwende den Standard-Konstruktor-Aufruf statt riskanter manueller Placement-News
        android::MemoryDealer* dealer = new (obj) android::MemoryDealer(size64, name, flags);

        // Zwingend erforderlich: Rückgabe des Zeigers an den RIL-Dienst
        return dealer;
    }

    // Fängt acquire_wake_lock ab und schreibt direkt in den 3.18-Kernel-Sysfs-Knoten,
    // um das Endlos-Warten auf android.system.suspend.ISystemSuspend/default zu verhindern.
    int acquire_wake_lock(int /*lock*/, const char* id) {
        if (!id) return -1;
        int fd = open("/sys/power/wake_lock", O_WRONLY | O_CLOEXEC);
        if (fd >= 0) {
            write(fd, id, strlen(id));
            close(fd);
        }
        return 0;
    }

    // Fängt release_wake_lock ab und gibt das Wakelock über /sys/power/wake_unlock wieder frei.
    int release_wake_lock(const char* id) {
        if (!id) return -1;
        int fd = open("/sys/power/wake_unlock", O_WRONLY | O_CLOEXEC);
        if (fd >= 0) {
            write(fd, id, strlen(id));
            close(fd);
        }
        return 0;
    }
}
