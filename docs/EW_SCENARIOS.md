# Electronic Warfare Scenarios

## Scenario Reference

### 1. DEAUTH_ATTACK (T1562.001)
Sends 802.11 deauthentication frames to disconnect clients from an access point. Validates whether the kernel's wireless stack (nl80211/cfg80211) or a detection tool detects the attack.

### 2. GPS_SPOOF (T1557)
Injects forged NMEA sentences into a GPS device stream to manipulate location data. Tests whether GPS-aware applications or kernel-level monitoring detect the spoofing.

### 3. BT_FLOOD (T1498)
Floods a Bluetooth target with L2CAP packets to cause denial of service. Validates kernel Bluetooth stack detection.

### 4. SPECTRUM_SCAN (EW-ES-001)
Reads signal level indicators from /proc/net/wireless and iw dev to assess spectrum congestion.

### 5. RFKILL_STATE (EW-EP-001)
Queries /sys/class/rfkill to determine whether RF switches are hardware or software blocked.

### 6. REGULATORY_CHECK (EW-EP-002)
Checks the wireless regulatory domain via iw reg get to validate country code and channel restrictions.

### 7. AIREPLAY_ATTACK (T1562.001)
Runs aireplay-ng in test mode to validate whether deauthentication and packet injection are detected.

### 8. SDR_READINESS (EW-ES-002)
Probes USB devices for RTL-SDR, HackRF, or BladeRF and checks for installed SDR tools.

### 9. WIFI_JAM (T1498)
Floods a target Wi-Fi channel with noise frames using available tools.

### 10. CHANNEL_HOP (EW-EA-001)
Rapidly cycles through Wi-Fi channels while measuring signal changes to simulate frequency-hopping EW attacks.
