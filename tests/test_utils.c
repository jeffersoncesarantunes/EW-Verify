#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ewverify.h"

static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        printf("  FAIL: %s\n", msg); \
        tests_failed++; \
    } else { \
        printf("  PASS: %s\n", msg); \
        tests_passed++; \
    } \
} while (0)

static void test_report_init(void)
{
    report_t r;
    report_init(&r);

    ASSERT(r.count == 0, "report count starts at 0");
    ASSERT(r.total_passed == 0, "total_passed starts at 0");
    ASSERT(r.total_failed == 0, "total_failed starts at 0");
    ASSERT(r.total_gaps == 0, "total_gaps starts at 0");
}

static void test_report_add_scenario(void)
{
    report_t r;
    report_init(&r);

    int ret = report_add_scenario(&r, "TEST_SCENARIO", "T9999",
                                  "Test scenario", SCENARIO_PASS, 1, 0);
    ASSERT(ret == 0, "add scenario succeeds");
    ASSERT(r.count == 1, "count increased");
    ASSERT(r.total_passed == 1, "passed count increased");
    ASSERT(r.total_ew_detected == 1, "EW detection counted");
    ASSERT(r.total_gaps == 0, "no gap when detected by EW");
}

static void test_report_add_max(void)
{
    report_t r;
    report_init(&r);

    for (int i = 0; i < MAX_SCENARIOS; i++) {
        report_add_scenario(&r, "S", "T0", "", SCENARIO_PASS, 0, 0);
    }

    int ret = report_add_scenario(&r, "OVERFLOW", "T0", "",
                                  SCENARIO_PASS, 0, 0);
    ASSERT(ret == -1, "overflow returns -1");
    ASSERT(r.count == MAX_SCENARIOS, "count capped at MAX_SCENARIOS");
}

int main(void)
{
    printf("EW-Verify Test Suite\n");
    printf("────────────────────\n\n");

    test_report_init();
    test_report_add_scenario();
    test_report_add_max();

    printf("\n────────────────────\n");
    printf("  %d passed, %d failed\n\n", tests_passed, tests_failed);

    return tests_failed > 0 ? 1 : 0;
}
