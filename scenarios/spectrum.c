#include <stdio.h>
#include <string.h>
#include "ewverify.h"

int run_spectrum_scan(report_t *r, int idx, int total, int live)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    scenario_print_header(idx, total, "SPECTRUM_SCAN");

    FILE *f = fopen("/proc/net/wireless", "r");
    if (f) {
        char line[256];
        int has_wireless = 0;
        while (fgets(line, sizeof(line), f)) {
            if (strstr(line, "wlan") || strstr(line, "wlp")) {
                has_wireless = 1;
                break;
            }
        }
        fclose(f);

        if (has_wireless) {
            result = SCENARIO_PASS;
            detected_kernel = 1;
            printf("PASS  [EW:%s  KS:%s]  EW-ES-001\n",
                   detected_ew ? "✔" : "✘",
                   detected_kernel ? "✔" : "✘");
        } else {
            result = SCENARIO_WARN;
            printf("WARN  (no wireless interfaces)\n");
        }
    } else {
        printf("SKIP  (/proc/net/wireless not available)\n");
    }

    if (live && result != SCENARIO_SKIP) {
        char lbuf[256];
        if (run_cmd("timeout 3 iw dev wlan0 survey dump 2>/dev/null | head -20", lbuf, sizeof(lbuf)) == 0 && lbuf[0]) {
            detected_ew = 1;
            detected_kernel = 1;
        }
    }

    report_add_scenario(r, "SPECTRUM_SCAN", "EW-ES-001",
                        "Electronic Support — spectrum congestion monitoring",
                        result, detected_ew, detected_kernel);
    return 0;
}
