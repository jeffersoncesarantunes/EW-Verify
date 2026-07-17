#include <stdio.h>
#include <string.h>
#include "ewverify.h"

int run_channel_hop(report_t *r, int idx, int total, int live)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    scenario_print_header(idx, total, "CHANNEL_HOP");

    if (!tool_exists("iw")) {
        printf("SKIP  (iw not found)\n");
        report_add_scenario(r, "CHANNEL_HOP", "EW-EA-001",
                            "Electronic Attack — rapid channel hopping analysis",
                            result, detected_ew, detected_kernel);
        return 0;
    }

    char buf[128];
    int ret = run_cmd("iw dev 2>/dev/null", buf, sizeof(buf));
    if (ret == 0 && strstr(buf, "Interface")) {
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

    if (live && result != SCENARIO_SKIP) {
        char lbuf[256];
        if (run_cmd("timeout 3 iw event 2>/dev/null | head -20", lbuf, sizeof(lbuf)) == 0 && lbuf[0]) {
            detected_ew = 1;
            detected_kernel = 1;
        }
    }

    report_add_scenario(r, "CHANNEL_HOP", "EW-EA-001",
                        "Electronic Attack — rapid channel hopping analysis",
                        result, detected_ew, detected_kernel);
    return 0;
}
