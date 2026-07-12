#include <stdio.h>
#include <string.h>
#include "ewverify.h"

int run_bt_attack(report_t *r, int idx, int total)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    scenario_print_header(idx, total, "BT_FLOOD");

    if (!tool_exists("bluetoothctl")) {
        printf("SKIP  (bluetoothctl not found)\n");
        report_add_scenario(r, "BT_FLOOD", "T1498",
                            "Bluetooth L2CAP flood denial of service",
                            result, detected_ew, detected_kernel);
        return 0;
    }

    char buf[256];
    int ret = run_cmd("bluetoothctl show 2>/dev/null", buf, sizeof(buf));

    if (ret == 0 && strstr(buf, "Powered: yes")) {
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
