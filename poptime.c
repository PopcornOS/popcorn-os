#include <efi.h>
#include <efilib.h>
#include "popcorn.h"
unsigned __int64 __rdtsc(void);

static double tscs_per_ns = 0.0;

void popd_calibrate() {
    EFI_TIME start, end;
    unsigned long long tsc_start, tsc_end;

    // Read EFI time at start
    gRT->GetTime(&start, NULL);
    tsc_start = __rdtsc();

    // Busy-wait until EFI time advances by at least 5 seconds
    do {
        gRT->GetTime(&end, NULL);
    } while ((end.Second - start.Second) < 4);

    tsc_end = __rdtsc();

    // Compute ticks per second (average over 2 seconds)
    unsigned long long delta_ticks = tsc_end - tsc_start;
    double ticks_per_sec = (double)delta_ticks / 4.0;

    // Convert to ticks per nanosecond
    tscs_per_ns = ticks_per_sec / 1e9;
}

static unsigned long long tsc2ns(unsigned long long tsc) {
    return (unsigned long long)(tsc / tscs_per_ns);
}

popd_Datetime pop_API popdk_datetime(pop_Services* svc) {
    EFI_TIME efitime;
    gRT->GetTime(&efitime, NULL);
    unsigned long long ns = tsc2ns(__rdtsc()) % 1000000000ULL;
    return (popd_Datetime){
        .year = efitime.Year, 
        .month = efitime.Month, 
        .day = efitime.Day, 
        .hours = efitime.Hour, 
        .minutes = efitime.Minute, 
        .seconds = efitime.Second, 
        .nanoseconds = efitime.Nanosecond > ns ? efitime.Nanosecond : ns
    };
}

unsigned long long pop_API popdk_uptimens(pop_Services* svc) {
    return tsc2ns(__rdtsc());
}