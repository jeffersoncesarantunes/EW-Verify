#include <stdio.h>
#include <unistd.h>
#include "ewverify.h"

int run_gps_spoof(report_t *r, int idx, int total, int live)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    scenario_print_header(idx, total, "GPS_SPOOF");

    int has_gps = 0;

    if (access("/dev/ttyGPS0", F_OK) == 0 ||
        access("/dev/ttyACM0", F_OK) == 0 ||
        access("/dev/ttyUSB0", F_OK) == 0) {
        has_gps = 1;
    }

    if (!has_gps && tool_exists("gpsd")) {
        has_gps = 1;
    }

    if (live && has_gps) {
        char lbuf[256];
        if (run_cmd("timeout 3 gpspipe -w -n 3 2>/dev/null", lbuf, sizeof(lbuf)) == 0 && lbuf[0]) {
            detected_ew = 1;
        }
    }

    if (has_gps) {
        result = SCENARIO_WARN;
        printf("WARN  [EW:%s  KS:%s]  T1557\n",
               detected_ew ? "✔" : "✘",
               detected_kernel ? "✔" : "✘");
    } else {
        printf("SKIP  (no GPS device)\n");
    }

    report_add_scenario(r, "GPS_SPOOF", "T1557",
                        "GPS spoofing via forged NMEA data injection",
                        result, detected_ew, detected_kernel);
    return 0;
}
