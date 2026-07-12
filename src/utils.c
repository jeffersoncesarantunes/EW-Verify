#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/wait.h>
#include "ewverify.h"
#include "colors.h"

void report_init(report_t *r)
{
    r->count = 0;
    r->total_passed = 0;
    r->total_failed = 0;
    r->total_warn = 0;
    r->total_skip = 0;
    r->total_ew_detected = 0;
    r->total_kernel_detected = 0;
    r->total_gaps = 0;
}

int report_add_scenario(report_t *r, const char *name,
                        const char *tech_id, const char *desc,
                        scenario_result_t result,
                        int detected_ew, int detected_kernel)
{
    if (r->count >= MAX_SCENARIOS) return -1;

    scenario_t *s = &r->scenarios[r->count];
    strncpy(s->name, name, MAX_NAME_LEN - 1);
    s->name[MAX_NAME_LEN - 1] = '\0';
    strncpy(s->technique_id, tech_id, MAX_NAME_LEN - 1);
    s->technique_id[MAX_NAME_LEN - 1] = '\0';
    strncpy(s->description, desc, MAX_DESC_LEN - 1);
    s->description[MAX_DESC_LEN - 1] = '\0';
    s->result = result;
    s->detected_by_ew = detected_ew;
    s->detected_by_kernel = detected_kernel;

    if (detected_ew) s->detection = DETECT_EW_SENSOR;
    else if (detected_kernel) s->detection = DETECT_KERNEL;
    else s->detection = DETECT_NONE;

    switch (result) {
    case SCENARIO_PASS: r->total_passed++; break;
    case SCENARIO_FAIL: r->total_failed++; break;
    case SCENARIO_WARN: r->total_warn++; break;
    case SCENARIO_SKIP: r->total_skip++; break;
    default: break;
    }

    if (detected_ew) r->total_ew_detected++;
    if (detected_kernel) r->total_kernel_detected++;
    if (!detected_ew && !detected_kernel) r->total_gaps++;

    r->count++;
    return 0;
}

static const char *result_str(scenario_result_t r)
{
    switch (r) {
    case SCENARIO_PASS: return "PASS";
    case SCENARIO_FAIL: return "FAIL";
    case SCENARIO_WARN: return "WARN";
    case SCENARIO_SKIP: return "SKIP";
    default: return "ERR";
    }
}

static const char *result_color(scenario_result_t r)
{
    switch (r) {
    case SCENARIO_PASS: return COLOR_GREEN;
    case SCENARIO_FAIL: return COLOR_RED;
    case SCENARIO_WARN: return COLOR_YELLOW;
    case SCENARIO_SKIP: return COLOR_CYAN;
    default: return COLOR_RED;
    }
}

void report_print(const report_t *r)
{
    printf("  " COLOR_BOLD COLOR_YELLOW "\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90" COLOR_RESET "\n");
    printf("   " COLOR_BOLD COLOR_YELLOW " RESULTS" COLOR_RESET "\n");
    printf("  " COLOR_BOLD COLOR_YELLOW "\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90" COLOR_RESET "\n\n");

    for (int i = 0; i < r->count; i++) {
        const scenario_t *s = &r->scenarios[i];
        printf("  [%02d/%02d] %-18s %s%-4s%s  [EW:%s  KS:%s]  %s\n",
               i + 1, r->count, s->name,
               result_color(s->result), result_str(s->result), COLOR_RESET,
               s->detected_by_ew ? COLOR_GREEN "✔" COLOR_RESET : COLOR_RED "✘" COLOR_RESET,
               s->detected_by_kernel ? COLOR_GREEN "✔" COLOR_RESET : COLOR_RED "✘" COLOR_RESET,
               s->technique_id);
    }

    printf("\n");
    printf("  " COLOR_BOLD COLOR_YELLOW "\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90" COLOR_RESET "\n");
    printf("   " COLOR_BOLD COLOR_YELLOW " FINAL ASSESSMENT" COLOR_RESET "\n");
    printf("  " COLOR_BOLD COLOR_YELLOW "\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90" COLOR_RESET "\n");
    printf("\n");

    int total = r->count;
    int bar_values[4] = {
        r->total_passed + r->total_warn,
        r->total_ew_detected,
        r->total_kernel_detected,
        r->total_gaps
    };
    const char *bar_labels[4] = {
        "adversarial actions succeeded",
        "detected by EW-Sensor",
        "detected by Kernel",
        "unmonitored gaps (no EW or KS coverage)"
    };

    for (int b = 0; b < 4; b++) {
        int value = bar_values[b];
        int width = 10;
        int filled = (total > 0) ? (value * width / total) : 0;
        if (filled > width) filled = width;

        printf("  " COLOR_YELLOW " [");
        for (int p = 0; p < width; p++) {
            if (p < filled)
                printf(COLOR_BOLD COLOR_YELLOW "\xe2\x96\x88" COLOR_RESET);
            else
                printf(COLOR_YELLOW "\xe2\x96\x91" COLOR_RESET);
        }
        printf(COLOR_YELLOW "]  %d/%d  (%d%%)" COLOR_RESET "  %s\n",
               value, total, total > 0 ? (value * 100 / total) : 0,
               bar_labels[b]);
    }

    printf("\n");
    printf("   STATUS:  " COLOR_MAGENTA "Purple Team Validation Complete" COLOR_RESET "\n");
    printf("  " COLOR_BOLD COLOR_YELLOW "\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90" COLOR_RESET "\n");
    printf("\n");
}

int report_save_json(const report_t *r, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;

    fprintf(f, "{\n");
    fprintf(f, "  \"tool\": \"EW-Verify\",\n");
    fprintf(f, "  \"version\": \"%s\",\n", EWVERIFY_VERSION);
    fprintf(f, "  \"timestamp\": %ld,\n", (long)time(NULL));
    fprintf(f, "  \"total_scenarios\": %d,\n", r->count);
    fprintf(f, "  \"passed\": %d,\n", r->total_passed);
    fprintf(f, "  \"failed\": %d,\n", r->total_failed);
    fprintf(f, "  \"warn\": %d,\n", r->total_warn);
    fprintf(f, "  \"skipped\": %d,\n", r->total_skip);
    fprintf(f, "  \"detected_by_ew\": %d,\n", r->total_ew_detected);
    fprintf(f, "  \"detected_by_kernel\": %d,\n", r->total_kernel_detected);
    fprintf(f, "  \"unmonitored_gaps\": %d,\n", r->total_gaps);
    fprintf(f, "  \"scenarios\": [\n");

    for (int i = 0; i < r->count; i++) {
        const scenario_t *s = &r->scenarios[i];
        fprintf(f, "    {\n");
        fprintf(f, "      \"name\": \"%s\",\n", s->name);
        fprintf(f, "      \"technique_id\": \"%s\",\n", s->technique_id);
        fprintf(f, "      \"description\": \"%s\",\n", s->description);
        fprintf(f, "      \"result\": \"%s\",\n", result_str(s->result));
        fprintf(f, "      \"detected_by_ew\": %s,\n", s->detected_by_ew ? "true" : "false");
        fprintf(f, "      \"detected_by_kernel\": %s\n", s->detected_by_kernel ? "true" : "false");
        fprintf(f, "    }%s\n", (i < r->count - 1) ? "," : "");
    }

    fprintf(f, "  ]\n");
    fprintf(f, "}\n");
    fclose(f);
    return 0;
}

int report_save_csv(const report_t *r, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;

    fprintf(f, "scenario,technique_id,result,detected_by_ew,detected_by_kernel\n");
    for (int i = 0; i < r->count; i++) {
        const scenario_t *s = &r->scenarios[i];
        fprintf(f, "%s,%s,%s,%s,%s\n",
                s->name, s->technique_id, result_str(s->result),
                s->detected_by_ew ? "true" : "false",
                s->detected_by_kernel ? "true" : "false");
    }

    fclose(f);
    return 0;
}

int tool_exists(const char *name)
{
    if (!name) return 0;

    char *path_env = getenv("PATH");
    if (!path_env) return 0;

    char *dup = strdup(path_env);
    if (!dup) return 0;

    char *dir = strtok(dup, ":");
    int found = 0;
    while (dir) {
        char full[PATH_MAX];
        int n = snprintf(full, sizeof(full), "%s/%s", dir, name);
        if (n > 0 && (size_t)n < sizeof(full)) {
            if (access(full, X_OK) == 0) {
                found = 1;
                break;
            }
        }
        dir = strtok(NULL, ":");
    }
    free(dup);
    return found;
}

int run_cmd(const char *cmd, char *out, size_t outsz)
{
    if (!cmd) return -1;

    FILE *f = popen(cmd, "r");
    if (!f) return -1;

    if (out && outsz > 0) {
        if (!fgets(out, (int)outsz, f)) {
            out[0] = '\0';
        }
    }

    int status = pclose(f);
    if (status == -1 || !WIFEXITED(status)) return -1;
    return WEXITSTATUS(status);
}

void scenario_print_header(int idx, int total, const char *name)
{
    printf("  [%02d/%02d] %-16s ", idx, total, name);
}

void check_requirements(void)
{
    printf("\n");
    printf("  " COLOR_BOLD COLOR_YELLOW "\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90" COLOR_RESET "\n");
    printf("   " COLOR_BOLD COLOR_YELLOW " REQUIREMENTS CHECK" COLOR_RESET "\n");
    printf("  " COLOR_BOLD COLOR_YELLOW "\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90" COLOR_RESET "\n\n");

    struct { const char *name; const char *cmd; int critical; } checks[] = {
        {"iw (wireless tool)",    "iw",     1},
        {"rfkill",               "rfkill",  1},
        {"bluetoothctl",         "bluetoothctl", 0},
        {"aireplay-ng",          "aireplay-ng", 0},
        {"mdk4/mdk3",            "mdk4",    0},
        {"gpsd",                 "gpsd",    0},
        {"rtl_test (RTL-SDR)",   "rtl_test", 0},
        {"hackrf_info (HackRF)", "hackrf_info", 0},
        {"iw dev (wireless iface)", "", 1},
    };

    for (size_t i = 0; i < sizeof(checks)/sizeof(checks[0]); i++) {
        const char *mark;
        if (strcmp(checks[i].cmd, "") == 0) {
            mark = has_wireless_tool() ? COLOR_GREEN "✔" COLOR_RESET : COLOR_RED "✘" COLOR_RESET;
        } else {
            mark = tool_exists(checks[i].cmd) ? COLOR_GREEN "✔" COLOR_RESET : COLOR_RED "✘" COLOR_RESET;
        }
        printf("   %s  %s\n", mark, checks[i].name);
    }

    printf("\n   " COLOR_BOLD "Summary:" COLOR_RESET "\n");
    printf("   Wireless interface:  %s\n", has_wireless_tool() ? "detected" : "not found");
    printf("   Bluetooth:           %s\n", has_bluetooth() ? "detected" : "not found");
    printf("   GPS device:          %s\n", has_gps() ? "detected" : "not found");
    printf("   SDR device:          %s\n", has_sdr() ? "detected" : "not found");
    printf("   Monitor mode:        %s\n\n", has_monitor_mode() ? "available" : "not available");
}

int has_wireless_tool(void)
{
    return tool_exists("iw");
}

int has_bluetooth(void)
{
    return tool_exists("bluetoothctl");
}

int has_gps(void)
{
    return tool_exists("gpsd");
}

int has_sdr(void)
{
    return tool_exists("rtl_test") || tool_exists("hackrf_info");
}

int has_monitor_mode(void)
{
    return tool_exists("iw");
}
