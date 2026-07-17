#include <stdio.h>
#include "ewverify.h"

int run_aireplay_attack(report_t *r, int idx, int total, int live)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    scenario_print_header(idx, total, "AIREPLAY_ATTACK");

    if (!tool_exists("aireplay-ng")) {
        printf("SKIP  (aireplay-ng not found)\n");
        report_add_scenario(r, "AIREPLAY_ATTACK", "T1562.001",
                            "Aireplay-ng deauthentication and packet injection",
                            result, detected_ew, detected_kernel);
        return 0;
    }

    result = SCENARIO_WARN;

    if (live) {
        char lbuf[256];
        if (run_cmd("timeout 3 aireplay-ng -9 wlan0mon 2>/dev/null", lbuf, sizeof(lbuf)) == 0) {
            detected_ew = 1;
            result = SCENARIO_PASS;
        }
    }

    printf("WARN  [EW:%s  KS:%s]  T1562.001\n",
           detected_ew ? "✔" : "✘",
           detected_kernel ? "✔" : "✘");

    report_add_scenario(r, "AIREPLAY_ATTACK", "T1562.001",
                        "Aireplay-ng deauthentication and packet injection",
                        result, detected_ew, detected_kernel);
    return 0;
}
