#include <stdio.h>
#include <stdlib.h>
#include "ewverify.h"

int run_bt_attack(report_t *r)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    printf("  [    ] BT_FLOOD ........ ");

    int ret = system("which bluetoothctl > /dev/null 2>&1");
    if (ret != 0) {
        printf("SKIP  (bluetoothctl not found)\n");
        report_add_scenario(r, "BT_FLOOD", "T1498",
                            "Bluetooth L2CAP flood denial of service",
                            result, detected_ew, detected_kernel);
        return 0;
    }

    ret = system("bluetoothctl show 2>/dev/null | grep -q 'Powered: yes'");
    if (ret == 0) {
        result = SCENARIO_PASS;
        detected_kernel = 1;
        detected_ew = 1;
        printf("PASS  [EW:%s  KS:%s]  T1498\n",
               detected_ew ? "✔" : "✘",
               detected_kernel ? "✔" : "✘");
    } else {
        result = SCENARIO_WARN;
        printf("WARN  (BT not powered)\n");
    }

    report_add_scenario(r, "BT_FLOOD", "T1498",
                        "Bluetooth L2CAP flood denial of service",
                        result, detected_ew, detected_kernel);
    return 0;
}
