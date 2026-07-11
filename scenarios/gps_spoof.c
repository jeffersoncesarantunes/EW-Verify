#include <stdio.h>
#include <stdlib.h>
#include "ewverify.h"

int run_gps_spoof(report_t *r)
{
    scenario_result_t result = SCENARIO_SKIP;
    int detected_ew = 0, detected_kernel = 0;

    printf("  [    ] GPS_SPOOF ....... ");

    int has_gps = 0;
    int ret = system("ls /dev/ttyGPS* /dev/ttyACM* /dev/ttyUSB* 2>/dev/null | grep -q .");
    if (ret == 0) has_gps = 1;

    if (!has_gps) {
        ret = system("which gpsd > /dev/null 2>&1");
        if (ret == 0) has_gps = 1;
    }

    if (has_gps) {
        result = SCENARIO_WARN;
        detected_ew = 0;
        detected_kernel = 0;
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
