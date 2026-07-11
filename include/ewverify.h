#ifndef EWVERIFY_H
#define EWVERIFY_H

#include <stdint.h>
#include <stddef.h>

#define EWVERIFY_VERSION "0.1.0"
#define MAX_SCENARIOS 16
#define MAX_NAME_LEN 64
#define MAX_DESC_LEN 256

typedef enum {
    SCENARIO_PASS,
    SCENARIO_FAIL,
    SCENARIO_WARN,
    SCENARIO_SKIP,
    SCENARIO_ERROR
} scenario_result_t;

typedef enum {
    DETECT_EW_SENSOR,
    DETECT_KERNEL,
    DETECT_NONE
} detection_source_t;

typedef struct {
    char name[MAX_NAME_LEN];
    char technique_id[MAX_NAME_LEN];
    char description[MAX_DESC_LEN];
    scenario_result_t result;
    int detected_by_ew;
    int detected_by_kernel;
    detection_source_t detection;
} scenario_t;

typedef struct {
    scenario_t scenarios[MAX_SCENARIOS];
    int count;
    int total_passed;
    int total_failed;
    int total_warn;
    int total_skip;
    int total_ew_detected;
    int total_kernel_detected;
    int total_gaps;
} report_t;

void report_init(report_t *r);
int report_add_scenario(report_t *r, const char *name,
                        const char *tech_id, const char *desc,
                        scenario_result_t result,
                        int detected_ew, int detected_kernel);
void report_print(const report_t *r);
int report_save_json(const report_t *r, const char *path);
int report_save_csv(const report_t *r, const char *path);

int tool_exists(const char *name);
int run_cmd(const char *cmd, char *out, size_t outsz);

#endif
