#include <dev/cpuid.h>

// Including GCC's provided cpuid.h
#include <cpuid.h>

enum
{
    CPUID_FEAT_EDX_APIC = 1 << 9,
};

int cpuid_check_apic(void)
{
    int unused, edx;
    __cpuid(1, unused, unused, unused, edx);
    return edx & CPUID_FEAT_EDX_APIC;
}

void cpuid_model(char vendor_id[13])
{
    int unused, ebx, ecx, edx;
    __cpuid(0, unused, ebx, ecx, edx);

    vendor_id[0] = ebx >> 0 & 0xFF;
    vendor_id[1] = ebx >> 8 & 0xFF;
    vendor_id[2] = ebx >> 16 & 0xFF;
    vendor_id[3] = ebx >> 24 & 0xFF;

    vendor_id[4] = edx >> 0 & 0xFF;
    vendor_id[5] = edx >> 8 & 0xFF;
    vendor_id[6] = edx >> 16 & 0xFF;
    vendor_id[7] = edx >> 24 & 0xFF;

    vendor_id[8] = ecx >> 0 & 0xFF;
    vendor_id[9] = ecx >> 8 & 0xFF;
    vendor_id[10] = ecx >> 16 & 0xFF;
    vendor_id[11] = ecx >> 24 & 0xFF;

    vendor_id[12] = 0;
}
