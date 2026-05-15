#include "popcorn.h"
int pop_API pop_main(pop_Services* svc, int argc, CHAR16** argv) {
    while (TRUE) {
        popd_Datetime time = svc->datetime(svc);

        svc->print(svc, L"\r");
        svc->printint(svc, time.year);
        svc->print(svc, L"-");
        svc->printint(svc, time.month);
        svc->print(svc, L"-");
        svc->printint(svc, time.day);
        svc->print(svc, L" ");
        svc->printint(svc, time.hours);
        svc->print(svc, L":");
        svc->printint(svc, time.minutes);
        svc->print(svc, L":");
        svc->printint(svc, time.seconds);
        svc->print(svc, L" ");
        svc->printint(svc, time.nanoseconds);
        svc->print(svc, L"ns ");
    }
}