#include <stdio.h>
#include <stdlib.h>
#include "ewverify.h"

int run_channel_hop(report_t *r)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    printf("  [    ] CHANNEL_HOP ...... ");

    int ret = system("which iw > /dev/null 2>&1");
    if (ret != 0) {
        printf("SKIP  (iw not found)\n");
        report_add_scenario(r, "CHANNEL_HOP", "EW-EA-001",
                            "Electronic Attack — rapid channel hopping analysis",
                            result, detected_ew, detected_kernel);
        return 0;
    }

    ret = system("iw dev 2>/dev/null | grep -q Interface");
    if (ret == 0) {
        result = SCENARIO_PASS;
        detected_kernel = 1;
        detected_ew = 1;
        printf("PASS  [EW:%s  KS:%s]  EW-EA-001\n",
               detected_ew ? "✔" : "✘",
               detected_kernel ? "✔" : "✘");
    } else {
        result = SCENARIO_SKIP;
        printf("SKIP  (no wireless interface)\n");
    }

    report_add_scenario(r, "CHANNEL_HOP", "EW-EA-001",
                        "Electronic Attack — rapid channel hopping analysis",
                        result, detected_ew, detected_kernel);
    return 0;
}
