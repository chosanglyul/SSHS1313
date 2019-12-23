#include <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
}

// SSID to connect to
char ssid[] = "SSHS-WiFi";
char wlpasswd[] = "";
char username[] = "Com&C";
char identity[] = "Com&C";
char password[] = "1234abcd";

u8 ca_cert[] = "-----BEGIN CERTIFICATE-----\n"
               "MIIFETCCA/mgAwIBAgIJAO0ntD34r/+QMA0GCSqGSIb3DQEBCwUAMIGiMQswCQYD\n"
               "VQQGEwJVUzEXMBUGA1UECAwOTm9ydGggQ2Fyb2xpbmExDzANBgNVBAcMBkR1cmhh\n"
               "bTEeMBwGA1UECgwVTWl0Y2hlbGwvVHJhbXBlciBJbmMuMR8wHQYJKoZIhvcNAQkB\n"
               "FhBidXJ0bXNAZ21haWwuY29tMSgwJgYDVQQDDB9CdXJ0IFNlbGYgQ2VydGlmaWNh\n"
               "dGUgQXV0aG9yaXR5MB4XDTE4MDEyMzE1NTgxOFoXDTE4MDMyNDE1NTgxOFowgaIx\n"
               "CzAJBgNVBAYTAlVTMRcwFQYDVQQIDA5Ob3J0aCBDYXJvbGluYTEPMA0GA1UEBwwG\n"
               "RHVyaGFtMR4wHAYDVQQKDBVNaXRjaGVsbC9UcmFtcGVyIEluYy4xHzAdBgkqhkiG\n"
               "9w0BCQEWEGJ1cnRtc0BnbWFpbC5jb20xKDAmBgNVBAMMH0J1cnQgU2VsZiBDZXJ0\n"
               "aWZpY2F0ZSBBdXRob3JpdHkwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIB\n"
               "AQC2O2m4JCmqimeztaZ8JasIuZqksrxvCjyWpkykoGzoR6v1/kDSuLgdp7EmtLKN\n"
               "vOJPQ2ea0yVvkFf0dQYcgDhNdAsXcqskMOprS+jefzlLQUbU1jTk+AOSiOcmxWT1\n"
               "H8uYhSp/1TkkWDM8dVO9RRIQyigVinsJNhdPyUVsTaQh3s29AL8CboixUNZWngOo\n"
               "d1+66lwy4N+3hXACJWlurgvc0q7RfqSqfJY/+lugfID0ANpGQqqu1Dq2IXZbLyx9\n"
               "roOPpC+fqX2wn5g9+RuA18OY/GPtkHc6clAoiJDGXsTRpffgsI5spdJ26WknbhO/\n"
               "3Q2vQJ2Q5m/QvSp55qOM5nevAgMBAAGjggFGMIIBQjAdBgNVHQ4EFgQUT3hChU84\n"
               "EEXHesDdKOtPqfhrg68wgdcGA1UdIwSBzzCBzIAUT3hChU84EEXHesDdKOtPqfhr\n"
               "g6+hgaikgaUwgaIxCzAJBgNVBAYTAlVTMRcwFQYDVQQIDA5Ob3J0aCBDYXJvbGlu\n"
               "YTEPMA0GA1UEBwwGRHVyaGFtMR4wHAYDVQQKDBVNaXRjaGVsbC9UcmFtcGVyIElu\n"
               "Yy4xHzAdBgkqhkiG9w0BCQEWEGJ1cnRtc0BnbWFpbC5jb20xKDAmBgNVBAMMH0J1\n"
               "cnQgU2VsZiBDZXJ0aWZpY2F0ZSBBdXRob3JpdHmCCQDtJ7Q9+K//kDAPBgNVHRMB\n"
               "Af8EBTADAQH/MDYGA1UdHwQvMC0wK6ApoCeGJWh0dHA6Ly93d3cuZXhhbXBsZS5v\n"
               "cmcvZXhhbXBsZV9jYS5jcmwwDQYJKoZIhvcNAQELBQADggEBADNmDxddEh1XSo8J\n"
               "fanCRuYqngpVFtKCzRNCtQ7aOAPrj6gRMtMAqQhlYmcbxjaVOCboAvJ8evY8x5We\n"
               "YU6eIAmg95PlDws8EvcVQhO2ZYcuOHTvdu5Q4Ior0oXaCfqdS35YcPW7pRGprY/x\n"
               "w7kbG84FFatpXPuDGQB65haqQ2KXPrILiE/jAKpTu9EBLEEN4uIAuxbB6unARovX\n"
               "YoXIQHFc32bZAjihquvs4J7FNkcSr3lJpuv03dryqMB+Qn3G8yM7PFLNL2QyS0Zu\n"
               "UbS5kDO7UqbTjtrUDolp5sUNvyrkX5ILYfZeEeaHqAlA20SFqNG7BcTY222NsCft\n"
               "yNXqX1c=\n"
               "-----END CERTIFICATE-----";

void setup() {
  Serial.begin(115200);
  // Setting ESP into STATION mode only (no AP mode or dual mode)
  wifi_set_opmode(0x01);
  
  struct station_config wifi_config;
  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);
  strcpy((char*)wifi_config.password, wlpasswd);
  wifi_station_set_config(&wifi_config);
  wifi_station_set_wpa2_enterprise_auth(1);
  // Clean up to be sure no old data is still inside
  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();
  wifi_station_clear_enterprise_identity();
  wifi_station_clear_enterprise_username();
  wifi_station_clear_enterprise_password();
  wifi_station_clear_enterprise_new_password();
  wifi_station_set_enterprise_ca_cert(ca_cert, sizeof(ca_cert));
  wifi_station_set_enterprise_identity((uint8*)identity, strlen(identity));
  wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  wifi_station_set_enterprise_password((uint8*)password, strlen(password));
  wifi_station_connect();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
}
