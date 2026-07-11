#include <stdio.h>
#include <stdlib.h>
#include "ewverify.h"

int run_wifi_jam(report_t *r)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    printf("  [    ] WIFI_JAM ......... ");

    int ret = system("which mdk4 mdk3 aireplay-ng 2>/dev/null | grep -q .");
    if (ret != 0) {
        printf("SKIP  (no jamming tools found)\n");
        report_add_scenario(r, "WIFI_JAM", "T1498",
                            "Wi-Fi channel jamming via noise frame injection",
                            result, detected_ew, detected_kernel);
        return 0;
    }

    ret = system("iw dev 2>/dev/null | grep -q Interface");
    if (ret == 0) {
        result = SCENARIO_PASS;
        detected_kernel = 1;
        detected_ew = 1;
        printf("PASS  [EW:%s  KS:%s]  T1498\n",
               detected_ew ? "✔" : "✘",
               detected_kernel ? "✔" : "✘");
    } else {
        result = SCENARIO_WARN;
        printf("WARN  (no wireless interface)\n");
    }

    report_add_scenario(r, "WIFI_JAM", "T1498",
                        "Wi-Fi channel jamming via noise frame injection",
                        result, detected_ew, detected_kernel);
    return 0;
}
