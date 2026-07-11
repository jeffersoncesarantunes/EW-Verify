# EW Validation Protocol

## Overview

EW-Verify uses a three-phase validation protocol to assess Electronic Warfare detection capabilities on Linux systems.

## Phase 1: Pre-Check

Before any adversarial action, the tool verifies:

1. Root privileges
2. Required kernel interfaces (/proc, /sys)
3. Available wireless tools (iw, rfkill)
4. SDR device presence (USB enumeration)

## Phase 2: Adversarial Execution

Each scenario executes a controlled EW action:

- **Read-only scenarios**: Check RF state, regulatory domain, SDR presence
- **Active scenarios**: Deauth, flood, jam attacks (require monitor mode)

## Phase 3: Detection Correlation

Every action is cross-referenced against:

1. **Kernel interfaces**: /proc/net/wireless, /sys/class/rfkill, dmesg
2. **EW sensor tools**: iw event, Wireshark, aircrack-ng
3. **Detection gap analysis**: Blind spots with no coverage

## Safety Guidelines

- All attacks target test interfaces only
- Use in Faraday cages or isolated RF labs
- Follow local spectrum regulations
- Obtain written authorization before testing
