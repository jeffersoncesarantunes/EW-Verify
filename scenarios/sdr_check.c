#include <stdio.h>
#include <stdlib.h>
#include "ewverify.h"

int run_sdr_readiness(report_t *r)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    printf("  [    ] SDR_READINESS .... ");

    int ret = system("lsusb 2>/dev/null | grep -qiE 'rtl2832|hackrf|bladerf|plutosdr|airspy'");
    if (ret == 0) {
        result = SCENARIO_PASS;
        detected_ew = 1;
        detected_kernel = 1;
        printf("PASS  [EW:%s  KS:%s]  EW-ES-002\n",
               detected_ew ? "✔" : "✘",
               detected_kernel ? "✔" : "✘");
    } else {
        ret = system("which rtl_test hackrf_info 2>/dev/null | grep -q .");
        if (ret == 0) {
            result = SCENARIO_PASS;
            detected_ew = 1;
            printf("PASS  (SDR tools found, no device)  [EW:%s  KS:%s]  EW-ES-002\n",
                   detected_ew ? "✔" : "✘",
                   detected_kernel ? "✔" : "✘");
        } else {
            printf("SKIP  (no SDR device or tools found)\n");
        }
    }

    report_add_scenario(r, "SDR_READINESS", "EW-ES-002",
                        "Electronic Support — SDR device capability assessment",
                        result, detected_ew, detected_kernel);
    return 0;
}
