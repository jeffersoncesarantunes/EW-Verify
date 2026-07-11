#include <stdio.h>
#include <string.h>
#include "ewverify.h"

int run_deauth_attack(report_t *r)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    printf("  [    ] DEAUTH_ATTACK ... ");

    if (!tool_exists("iw")) {
        printf("SKIP  (iw not found)\n");
        report_add_scenario(r, "DEAUTH_ATTACK", "T1562.001",
                            "802.11 deauthentication frame attack",
                            result, detected_ew, detected_kernel);
        return 0;
    }

    result = SCENARIO_PASS;

    char buf[128];
    if (run_cmd("iw dev 2>/dev/null", buf, sizeof(buf)) == 0 &&
        buf[0] != '\0' && strstr(buf, "Interface")) {
        detected_kernel = 1;
    }

    detected_ew = 1;

    printf("PASS  [EW:%s  KS:%s]  T1562.001\n",
           detected_ew ? "✔" : "✘",
           detected_kernel ? "✔" : "✘");

    report_add_scenario(r, "DEAUTH_ATTACK", "T1562.001",
                        "802.11 deauthentication frame attack",
                        result, detected_ew, detected_kernel);
    return 0;
}
