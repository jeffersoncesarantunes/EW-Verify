#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ewverify.h"

int run_rfkill_check(report_t *r, int idx, int total, int live)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    scenario_print_header(idx, total, "RFKILL_STATE");

    char buf[64];
    int ret = run_cmd("rfkill list 2>/dev/null | grep -c 'Hard blocked: yes'",
                      buf, sizeof(buf));

    if (ret == 0 && buf[0] != '\0') {
        char *end = NULL;
        errno = 0;
        long val = strtol(buf, &end, 10);
        if (errno == 0 && end != buf && val >= 0) {
            result = SCENARIO_PASS;
            detected_kernel = 1;
            if (val > 0) {
                printf("PASS  [EW:%s  KS:%s]  EW-EP-001\n",
                       detected_ew ? "✔" : "✘",
                       detected_kernel ? "✔" : "✘");
            } else {
                printf("PASS  (all RF unlocked)  [EW:%s  KS:%s]  EW-EP-001\n",
                       detected_ew ? "✔" : "✘",
                       detected_kernel ? "✔" : "✘");
            }
        } else {
            printf("WARN  (invalid rfkill output)\n");
        }
    } else {
        printf("SKIP  (rfkill not available)\n");
    }

    if (live && result != SCENARIO_SKIP) {
        detected_kernel = 1;
    }

    report_add_scenario(r, "RFKILL_STATE", "EW-EP-001",
                        "Electronic Protection — RF kill switch state assessment",
                        result, detected_ew, detected_kernel);
    return 0;
}
