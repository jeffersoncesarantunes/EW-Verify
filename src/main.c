#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "ewverify.h"
#include "colors.h"
#include "modules.h"

static void print_banner(void)
{
    const char *title = "EW-Verify | Purple Team - Electronic Warfare";
    int width = 55;
    int pad = width - (int)strlen(title);
    int left = pad / 2;
    int right = pad - left;

    printf("\n");
    printf("        " COLOR_YELLOW "\xe2\x95\x94");
    for (int i = 0; i < width; i++) printf("\xe2\x95\x90");
    printf("\xe2\x95\x97" COLOR_RESET "\n");
    printf("        " COLOR_YELLOW "\xe2\x95\x91");
    printf("%*s%s%*s", left, "", title, right, "");
    printf("\xe2\x95\x91" COLOR_RESET "\n");
    printf("        " COLOR_YELLOW "\xe2\x95\x9a");
    for (int i = 0; i < width; i++) printf("\xe2\x95\x90");
    printf("\xe2\x95\x9d" COLOR_RESET "\n");
    printf("\n");
}

static void print_usage(void)
{
    printf("EW-Verify v%s — Electronic Warfare Purple Team Validation\n\n", EWVERIFY_VERSION);
    printf("Usage: ewverify [options]\n\n");
    printf("Options:\n");
    printf("  --json              Export JSON report (silent mode, no banner)\n");
    printf("  --csv               Export CSV report\n");
    printf("  --deauth            Run deauthentication attack scenario\n");
    printf("  --gps-spoof         Run GPS spoofing scenario\n");
    printf("  --bt-attack         Run Bluetooth attack scenario\n");
    printf("  --spectrum          Run spectrum congestion analysis\n");
    printf("  --rfkill            Check RF kill switch state\n");
    printf("  --regulatory        Check wireless regulatory domain\n");
    printf("  --aireplay          Run aireplay-ng attack scenario\n");
    printf("  --sdr               Check SDR device readiness\n");
    printf("  --wifi-jam          Run Wi-Fi jamming scenario\n");
    printf("  --channel-hop       Run channel hopping analysis\n");
    printf("  --live              Invoke live detection tools for validation\n");
    printf("  --verify-only       Read-only assessment (no attacks)\n");
    printf("  --cleanup           Clean up any remaining child processes\n");
    printf("  --check-req         Check available hardware and tools\n");
    printf("  -h, --help          Show this help\n");
    printf("  -V, --version       Show version\n");
}

int main(int argc, char *argv[])
{
    int opt_json = 0, opt_csv = 0, opt_live = 0;
    int opt_verify_only = 0, opt_cleanup = 0, opt_check_req = 0;
    int run_all = 1;
    int opt_deauth = 0, opt_gps = 0, opt_bt = 0;
    int opt_spectrum = 0, opt_rfkill = 0, opt_reg = 0;
    int opt_aireplay = 0, opt_sdr = 0, opt_jam = 0, opt_chop = 0;

    static struct option long_opts[] = {
        {"json",        no_argument, 0, 'j'},
        {"csv",         no_argument, 0, 'c'},
        {"live",        no_argument, 0, 'l'},
        {"verify-only", no_argument, 0, 'v'},
        {"cleanup",     no_argument, 0, 'C'},
        {"check-req",   no_argument, 0, 256},
        {"deauth",      no_argument, 0, 'd'},
        {"gps-spoof",   no_argument, 0, 'g'},
        {"bt-attack",   no_argument, 0, 'b'},
        {"spectrum",    no_argument, 0, 's'},
        {"rfkill",      no_argument, 0, 'r'},
        {"regulatory",  no_argument, 0, 'R'},
        {"aireplay",    no_argument, 0, 'a'},
        {"sdr",         no_argument, 0, 'S'},
        {"wifi-jam",    no_argument, 0, 'w'},
        {"channel-hop", no_argument, 0, 'H'},
        {"help",        no_argument, 0, 'h'},
        {"version",     no_argument, 0, 'V'},
        {0, 0, 0, 0}
    };

    int c;
    while ((c = getopt_long(argc, argv, "jclvCdgbsrRaSwHhV", long_opts, NULL)) != -1) {
        switch (c) {
        case 'j': opt_json = 1; break;
        case 'c': opt_csv = 1; break;
        case 'l': opt_live = 1; break;
        case 'v': opt_verify_only = 1; break;
        case 'C': opt_cleanup = 1; break;
        case 256: opt_check_req = 1; break;
        case 'd': opt_deauth = 1; run_all = 0; break;
        case 'g': opt_gps = 1; run_all = 0; break;
        case 'b': opt_bt = 1; run_all = 0; break;
        case 's': opt_spectrum = 1; run_all = 0; break;
        case 'r': opt_rfkill = 1; run_all = 0; break;
        case 'R': opt_reg = 1; run_all = 0; break;
        case 'a': opt_aireplay = 1; run_all = 0; break;
        case 'S': opt_sdr = 1; run_all = 0; break;
        case 'w': opt_jam = 1; run_all = 0; break;
        case 'H': opt_chop = 1; run_all = 0; break;
        case 'h': print_usage(); return 0;
        case 'V': printf("EW-Verify v%s\n", EWVERIFY_VERSION); return 0;
        default: print_usage(); return 1;
        }
    }

    if (geteuid() != 0) {
        fprintf(stderr, "EW-Verify requires root privileges.\n");
        return 1;
    }

    if (opt_check_req) {
        check_requirements();
        return 0;
    }

    report_t report;
    report_init(&report);

    if (!opt_json) {
        print_banner();
        print_separator();
        printf("   " COLOR_BOLD COLOR_YELLOW " RUN SEQUENCE INITIATED" COLOR_RESET "\n");
        print_separator();
        printf("\n");
    }

    if (opt_verify_only) {
        return run_verify_only(&report);
    }

    if (opt_cleanup) {
        printf("  Cleaning up child processes...\n");
        return 0;
    }

    int seq = 0, total_scenarios = 10;
    if (run_all || opt_deauth)    run_deauth_attack(&report, ++seq, total_scenarios, opt_live);
    if (run_all || opt_gps)       run_gps_spoof(&report, ++seq, total_scenarios, opt_live);
    if (run_all || opt_bt)        run_bt_attack(&report, ++seq, total_scenarios, opt_live);
    if (run_all || opt_spectrum)  run_spectrum_scan(&report, ++seq, total_scenarios, opt_live);
    if (run_all || opt_rfkill)    run_rfkill_check(&report, ++seq, total_scenarios, opt_live);
    if (run_all || opt_reg)       run_regulatory_check(&report, ++seq, total_scenarios, opt_live);
    if (run_all || opt_aireplay)  run_aireplay_attack(&report, ++seq, total_scenarios, opt_live);
    if (run_all || opt_sdr)       run_sdr_readiness(&report, ++seq, total_scenarios, opt_live);
    if (run_all || opt_jam)       run_wifi_jam(&report, ++seq, total_scenarios, opt_live);
    if (run_all || opt_chop)      run_channel_hop(&report, ++seq, total_scenarios, opt_live);

    if (!opt_json) {
        report_print(&report);
    }

    if (opt_json) {
        report_save_json(&report, "ewverify-report.json");
    }

    if (opt_csv) {
        report_save_csv(&report, "ewverify-report.csv");
    }

    return 0;
}
