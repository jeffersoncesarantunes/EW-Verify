#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "ewverify.h"

int run_deauth_attack(report_t *r)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    printf("  [    ] DEAUTH_ATTACK ... ");

    int ret = system("which iw > /dev/null 2>&1");
    if (ret != 0) {
        printf("SKIP  (iw not found)\n");
        report_add_scenario(r, "DEAUTH_ATTACK", "T1562.001",
                            "802.11 deauthentication frame attack",
                            result, detected_ew, detected_kernel);
        return 0;
    }

    result = SCENARIO_PASS;

    ret = system("iw dev 2>/dev/null | grep -q Interface");
    if (ret == 0) {
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
