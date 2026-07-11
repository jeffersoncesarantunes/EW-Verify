#include <stdio.h>
#include <stdlib.h>
#include "ewverify.h"

int run_aireplay_attack(report_t *r)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    printf("  [    ] AIREPLAY_ATTACK .. ");

    int ret = system("which aireplay-ng > /dev/null 2>&1");
    if (ret != 0) {
        printf("SKIP  (aireplay-ng not found)\n");
        report_add_scenario(r, "AIREPLAY_ATTACK", "T1562.001",
                            "Aireplay-ng deauthentication and packet injection",
                            result, detected_ew, detected_kernel);
        return 0;
    }

    result = SCENARIO_WARN;
    printf("WARN  [EW:%s  KS:%s]  T1562.001\n",
           detected_ew ? "✔" : "✘",
           detected_kernel ? "✔" : "✘");

    report_add_scenario(r, "AIREPLAY_ATTACK", "T1562.001",
                        "Aireplay-ng deauthentication and packet injection",
                        result, detected_ew, detected_kernel);
    return 0;
}
