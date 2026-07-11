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
    printf("  ──────────────────────────────────────────────────────────────\n");
    printf("    RESULTS\n");
    printf("  ──────────────────────────────────────────────────────────────\n\n");

    for (int i = 0; i < r->count; i++) {
        const scenario_t *s = &r->scenarios[i];
        printf("  [%02d/%02d] %-18s %s%-4s%s  [EW:%s  KS:%s]  %s\n",
               i + 1, r->count, s->name,
               result_color(s->result), result_str(s->result), COLOR_RESET,
               s->detected_by_ew ? COLOR_GREEN "✔" COLOR_RESET : COLOR_RED "✘" COLOR_RESET,
               s->detected_by_kernel ? COLOR_GREEN "✔" COLOR_RESET : COLOR_RED "✘" COLOR_RESET,
               s->technique_id);
    }

    printf("\n  ──────────────────────────────────────────────────────────────\n");
    printf("    FINAL ASSESSMENT\n");
    printf("  ──────────────────────────────────────────────────────────────\n\n");

    int total = r->count;
    int ew_bars = total > 0 ? (r->total_ew_detected * 10) / total : 0;
    int ks_bars = total > 0 ? (r->total_kernel_detected * 10) / total : 0;
    int gap_bars = total > 0 ? (r->total_gaps * 10) / total : 0;
    int adv_bars = total > 0 ? ((r->total_passed + r->total_warn) * 10) / total : 0;

    printf("  [%.*s%.*s]  %d/%d  (%d%%)  adversarial actions succeeded\n",
           adv_bars, "██████████", 10 - adv_bars, "░░░░░░░░░░",
           r->total_passed + r->total_warn, total,
           total > 0 ? ((r->total_passed + r->total_warn) * 100) / total : 0);
    printf("  [%.*s%.*s]  %d/%d  (%d%%)  detected by EW sensor\n",
           ew_bars, "██████████", 10 - ew_bars, "░░░░░░░░░░",
           r->total_ew_detected, total,
           total > 0 ? (r->total_ew_detected * 100) / total : 0);
    printf("  [%.*s%.*s]  %d/%d  (%d%%)  detected by kernel\n",
           ks_bars, "██████████", 10 - ks_bars, "░░░░░░░░░░",
           r->total_kernel_detected, total,
           total > 0 ? (r->total_kernel_detected * 100) / total : 0);
    printf("  [%.*s%.*s]  %d/%d  (%d%%)  unmonitored gaps (no coverage)\n",
           gap_bars, "██████████", 10 - gap_bars, "░░░░░░░░░░",
           r->total_gaps, total,
           total > 0 ? (r->total_gaps * 100) / total : 0);

    printf("\n   STATUS:  Purple Team Validation Complete\n");
    printf("  ──────────────────────────────────────────────────────────────\n\n");
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

    return pclose(f);
}
