#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ewverify.h"

int run_regulatory_check(report_t *r)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    printf("  [    ] REGULATORY_CHECK . ");

    FILE *f = popen("iw reg get 2>/dev/null", "r");
    if (f) {
        char line[256];
        int found = 0;
        while (fgets(line, sizeof(line), f)) {
            if (strstr(line, "country")) {
                found = 1;
                break;
            }
        }
        pclose(f);

        if (found) {
            result = SCENARIO_PASS;
            detected_kernel = 1;
            printf("PASS  [EW:%s  KS:%s]  EW-EP-002\n",
                   detected_ew ? "✔" : "✘",
                   detected_kernel ? "✔" : "✘");
        } else {
            result = SCENARIO_WARN;
            printf("WARN  (no regulatory info)\n");
        }
    } else {
        printf("SKIP  (iw not available)\n");
    }

    report_add_scenario(r, "REGULATORY_CHECK", "EW-EP-002",
                        "Electronic Protection — wireless regulatory domain compliance",
                        result, detected_ew, detected_kernel);
    return 0;
}
