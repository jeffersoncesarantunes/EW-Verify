#include <stdio.h>
#include <string.h>
#include "ewverify.h"

int run_sdr_readiness(report_t *r)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    printf("  [    ] SDR_READINESS .... ");

    char buf[256];
    int ret = run_cmd("lsusb 2>/dev/null", buf, sizeof(buf));

    if (ret == 0 &&
        (strstr(buf, "rtl2832") || strstr(buf, "hackrf") ||
         strstr(buf, "bladerf") || strstr(buf, "plutosdr") ||
         strstr(buf, "airspy"))) {
        result = SCENARIO_PASS;
        detected_ew = 1;
        detected_kernel = 1;
        printf("PASS  [EW:%s  KS:%s]  EW-ES-002\n",
               detected_ew ? "✔" : "✘",
               detected_kernel ? "✔" : "✘");
    } else {
        int tools = 0;
        if (tool_exists("rtl_test")) tools = 1;
        if (tool_exists("hackrf_info")) tools = 1;

        if (tools) {
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
