#ifndef MODULES_H
#define MODULES_H

#include "ewverify.h"

int run_deauth_attack(report_t *r);
int run_gps_spoof(report_t *r);
int run_bt_attack(report_t *r);
int run_spectrum_scan(report_t *r);
int run_rfkill_check(report_t *r);
int run_regulatory_check(report_t *r);
int run_aireplay_attack(report_t *r);
int run_sdr_readiness(report_t *r);
int run_wifi_jam(report_t *r);
int run_channel_hop(report_t *r);
int run_verify_only(report_t *r);

#endif
