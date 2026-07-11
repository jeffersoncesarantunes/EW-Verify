# Security Policy

## Supported Versions

| Version | Supported |
|---------|-----------|
| latest  | ✅       |

## Reporting a Vulnerability

This is a Purple Team adversarial validation tool for Electronic Warfare scenarios. If you discover a security vulnerability, please do NOT open a public issue.

Contact the maintainer directly at jefferson.antunes@gmail.com with details about the issue.

We commit to acknowledging receipt within 48 hours and providing a fix timeline within 7 days.

## Known Limitations

- **Wireless interface requirements**: Deauthentication and jamming scenarios require a wireless adapter that supports monitor mode. Without it, those scenarios will report SKIP.
- **GPS spoofing detection**: GPS spoofing detection depends on the presence of a physical GPS device or an NMEA stream. Software-based GPS (e.g., over network) may not be detected.
- **TOCTOU in /proc analysis**: The tool reads /proc/net/wireless to assess spectrum state. Between reading and analysis, the state could change. This is single-threaded and the window is negligible, but it is a documented inherent limitation.
- **Legal compliance**: This tool is designed for authorized lab environments only. Running wireless attacks against networks you do not own is illegal in most jurisdictions.
