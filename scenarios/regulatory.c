#include <stdio.h>
#include <string.h>
#include "ewverify.h"

int run_regulatory_check(report_t *r, int idx, int total, int live)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    scenario_print_header(idx, total, "REGULATORY_CHECK");

    char buf[256];
    int ret = run_cmd("iw reg get 2>/dev/null", buf, sizeof(buf));

    if (ret == 0 && buf[0] != '\0') {
        if (strstr(buf, "country")) {
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

    if (live && result != SCENARIO_SKIP) {
        detected_kernel = 1;
    }

    report_add_scenario(r, "REGULATORY_CHECK", "EW-EP-002",
                        "Electronic Protection — wireless regulatory domain compliance",
                        result, detected_ew, detected_kernel);
    return 0;
}
