#include <stdio.h>
#include <string.h>
#include "ewverify.h"
#include "colors.h"

int run_verify_only(report_t *r)
{
    printf("\n");
    print_separator();
    printf("   " COLOR_BOLD COLOR_YELLOW " READ-ONLY ASSESSMENT" COLOR_RESET "\n");
    print_separator();
    printf("\n");

    struct { const char *name; const char *tech; const char *desc; int ok; } checks[] = {
        {"wireless_tool", "T0855", "RF sensors and wireless tooling available", 0},
        {"bluetooth",     "T0855", "Bluetooth coverage surface present", 0},
        {"gps",           "T1491", "GPS/spoofing surface present", 0},
        {"sdr",           "T0855", "SDR (RTL-SDR/HackRF) surface present", 0},
        {"monitor_mode",  "T1543", "Monitor-mode capable wireless interface", 0},
    };

    int results[] = {
        has_wireless_tool(),
        has_bluetooth(),
        has_gps(),
        has_sdr(),
        has_monitor_mode(),
    };

    size_t total_checks = sizeof(checks) / sizeof(checks[0]);

    for (size_t i = 0; i < total_checks; i++) {
        scenario_result_t result = results[i] ? SCENARIO_PASS : SCENARIO_WARN;
        report_add_scenario(r, checks[i].name, checks[i].tech,
                            checks[i].desc, result,
                            results[i], 0);
        printf("   %s  %-16s  %s\n",
               results[i] ? COLOR_GREEN "✔" COLOR_RESET : COLOR_YELLOW "⚠" COLOR_RESET,
               checks[i].name, checks[i].desc);
    }

    printf("\n   " COLOR_BOLD "Verified %zu surfaces. No attack or modification performed." COLOR_RESET "\n\n", total_checks);
    report_print(r);

    return 0;
}
