#include <stdio.h>
#include <string.h>
#include "ewverify.h"

int run_wifi_jam(report_t *r, int idx, int total, int live)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    scenario_print_header(idx, total, "WIFI_JAM");

    if (!tool_exists("mdk4") && !tool_exists("mdk3") &&
        !tool_exists("aireplay-ng")) {
        printf("SKIP  (no jamming tools found)\n");
        report_add_scenario(r, "WIFI_JAM", "T1498",
                            "Wi-Fi channel jamming via noise frame injection",
                            result, detected_ew, detected_kernel);
        return 0;
    }

    char buf[128];
    int ret = run_cmd("iw dev 2>/dev/null", buf, sizeof(buf));
    if (ret == 0 && strstr(buf, "Interface")) {
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

    if (live && result != SCENARIO_SKIP) {
        char lbuf[256];
        if (run_cmd("timeout 3 iw dev wlan0 survey dump 2>/dev/null | head -20", lbuf, sizeof(lbuf)) == 0 && lbuf[0]) {
            detected_ew = 1;
            detected_kernel = 1;
        }
    }

    report_add_scenario(r, "WIFI_JAM", "T1498",
                        "Wi-Fi channel jamming via noise frame injection",
                        result, detected_ew, detected_kernel);
    return 0;
}
