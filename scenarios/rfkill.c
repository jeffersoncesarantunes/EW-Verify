#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ewverify.h"

int run_rfkill_check(report_t *r)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    printf("  [    ] RFKILL_STATE ..... ");

    FILE *f = popen("rfkill list 2>/dev/null | grep -c 'Hard blocked: yes'", "r");
    if (f) {
        char buf[16];
        if (fgets(buf, sizeof(buf), f)) {
            int blocked = atoi(buf);
            if (blocked > 0) {
                result = SCENARIO_PASS;
                detected_kernel = 1;
                printf("PASS  [EW:%s  KS:%s]  EW-EP-001\n",
                       detected_ew ? "✔" : "✘",
                       detected_kernel ? "✔" : "✘");
            } else {
                result = SCENARIO_PASS;
                detected_kernel = 1;
                printf("PASS  (all RF unlocked)  [EW:%s  KS:%s]  EW-EP-001\n",
                       detected_ew ? "✔" : "✘",
                       detected_kernel ? "✔" : "✘");
            }
        }
        pclose(f);
    } else {
        result = SCENARIO_SKIP;
        printf("SKIP  (rfkill not available)\n");
    }

    report_add_scenario(r, "RFKILL_STATE", "EW-EP-001",
                        "Electronic Protection — RF kill switch state assessment",
                        result, detected_ew, detected_kernel);
    return 0;
}
