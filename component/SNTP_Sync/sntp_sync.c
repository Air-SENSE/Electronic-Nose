#include "sntp_sync.h"


#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_CUSTOM
void sntp_sync_time(struct timeval *tv)
{
    // Set the system time to the specified time
    settimeofday(tv, NULL);

    // Log a message indicating that the time is synchronized from custom code
    ESP_LOGI(__func__, "Time is synchronized from custom code");

    // Set the SNTP sync status to completed
    sntp_set_sync_status(SNTP_SYNC_STATUS_COMPLETED);
}
#endif

void sntp_setTimmeZoneToVN()
{
    // Buffer to store the formatted time string
    char strftime_buf[64] = { 0 };

    time_t now = 0;
    struct tm timeInfo = { 0 };

    // Set timezone to VietNam Standard Time
    setenv("TZ", "GMT-07", 1);
    tzset();

    // Get the current time in the specified timezone
    localtime_r(&now, &timeInfo);

    // Format the time into a string and store it in strftime_buf
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeInfo);

    // Log the current date/time in Viet Nam
    ESP_LOGI(__func__, "The current date/time in Viet Nam is: %s ", strftime_buf);

    // End of the function
    return;
}

void sntp_printServerInformation(void)
{
    ESP_LOGI(__func__, "List of configured NTP servers:");

    // Loop through the SNTP servers
    for (uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i){
        // Check if the server name is available
        if (esp_sntp_getservername(i)){
            // Log the server name
            ESP_LOGI(__func__, "server %d: %s", i, esp_sntp_getservername(i));
        } else {
            // We have either IPv4 or IPv6 address, let's print it
            char buff[INET6_ADDRSTRLEN];
            ip_addr_t const *ip = esp_sntp_getserver(i);
            // Convert the IP address to a string and log it
            if (ipaddr_ntoa_r(ip, buff, INET6_ADDRSTRLEN) != NULL)
                ESP_LOGI(__func__, "server %d: %s", i, buff);
        }
    }
}

esp_err_t sntp_initialize(void *sntp_callback)
{

#if LWIP_DHCP_GET_NTP_SRV
    /**
     * NTP server address could be acquired via DHCP,
     * see following menuconfig options:
     * 'LWIP_DHCP_GET_NTP_SRV' - enable STNP over DHCP
     * 'LWIP_SNTP_DEBUG' - enable debugging messages
     *
     * NOTE: This call should be made BEFORE esp acquires IP address from DHCP,
     * otherwise NTP option would be rejected by default.
     */
    ESP_LOGI(__func__, "Initializing SNTP");
    esp_sntp_config_t sntp_config = ESP_NETIF_SNTP_DEFAULT_CONFIG(CONFIG_SNTP_TIME_SERVER);
    sntp_config.start = false;                       // start SNTP service explicitly (after connecting)
    sntp_config.server_from_dhcp = true;             // accept NTP offers from DHCP server, if any (need to enable *before* connecting)
    sntp_config.renew_servers_after_new_IP = true;   // let esp-netif update configured SNTP server(s) after receiving DHCP lease
    sntp_config.index_of_first_server = 1;           // updates from server num 1, leaving server 0 (from DHCP) intact
    // configure the event on which we renew servers
#ifdef CONFIG_EXAMPLE_CONNECT_WIFI
    sntp_config.ip_event_to_renew = IP_EVENT_STA_GOT_IP;
#endif
    sntp_config.sync_cb = (esp_sntp_time_cb_t)sntp_callback; // only if we need the notification function
    return esp_netif_sntp_init(&sntp_config);

#endif /* LWIP_DHCP_GET_NTP_SRV */

#if LWIP_DHCP_GET_NTP_SRV
    ESP_LOGI(__func__, "Starting SNTP");
    esp_netif_sntp_start();
#if LWIP_IPV6 && SNTP_MAX_SERVERS > 2
    /* This demonstrates using IPv6 address as an additional SNTP server
     * (statically assigned IPv6 address is also possible)
     */
    ip_addr_t ip6;
    if (ipaddr_aton("2a01:3f7::1", &ip6)) {    // ipv6 ntp source "ntp.netnod.se"
        esp_sntp_setserver(2, &ip6);
    }
#endif  /* LWIP_IPV6 */

#else
    ESP_LOGI(__func__, "Initializing and starting SNTP");
#if CONFIG_LWIP_SNTP_MAX_SERVERS > 1
    /* This demonstrates configuring more than one server
     */
    esp_sntp_config_t sntp_config = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(2,
                               ESP_SNTP_SERVER_LIST(CONFIG_SNTP_TIME_SERVER, "pool.ntp.org", "time.google.com", "time.cloudflare.com" ) );
#else
    /*
     * This is the basic default config with one server and starting the service
     */
    esp_sntp_config_t sntp_config = ESP_NETIF_SNTP_DEFAULT_CONFIG(CONFIG_SNTP_TIME_SERVER);
#endif
    sntp_config.sync_cb = (esp_sntp_time_cb_t)sntp_callback;     // Note: This is only needed if we want
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    sntp_config.smooth_sync = true;
#endif

    return esp_netif_sntp_init(&sntp_config);
#endif
}

esp_err_t sntp_syncTime(void)
{
    // wait for time to be set
    // Initialize variables for time and retry count
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry_count = 0;

    // Synchronize with SNTP server and retry if necessary
    while (esp_netif_sntp_sync_wait(2000 / portTICK_PERIOD_MS) == ESP_ERR_TIMEOUT && ++retry_count < CONFIG_SNTP_TIME_MAX_RETRY)
    {
        ESP_LOGI(__func__, "Waiting for system time to be set... (%d/%d)", retry_count, CONFIG_SNTP_TIME_MAX_RETRY);
        retry_count++;
        if (retry_count == CONFIG_SNTP_TIME_MAX_RETRY)
        {
            return ESP_ERR_TIMEOUT;
        }
    }

    // Get current time and convert to local time
    time(&now);
    localtime_r(&now, &timeinfo);
    return ESP_OK;  // Return success
}

// esp_err_t fetch_and_store_time_in_nvs(void *args)
// {
//     initialize_sntp();
//     if (obtain_time() != ESP_OK) {
//         return ESP_FAIL;
//     }

//     nvs_handle_t my_handle;
//     esp_err_t err;

//     time_t now;
//     time(&now);

//     //Open
//     err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
//     if (err != ESP_OK) {
//         goto exit;
//     }

//     //Write
//     err = nvs_set_i64(my_handle, "timestamp", now);
//     if (err != ESP_OK) {
//         goto exit;
//     }

//     err = nvs_commit(my_handle);
//     if (err != ESP_OK) {
//         goto exit;
//     }

//     nvs_close(my_handle);
//     esp_netif_deinit();

// exit:
//     if (err != ESP_OK) {
//         ESP_LOGE(TAG, "Error updating time in nvs");
//     } else {
//         ESP_LOGI(TAG, "Updated time in NVS");
//     }
//     return err;
// }

// esp_err_t update_time_from_nvs(void)
// {
//     nvs_handle_t my_handle;
//     esp_err_t err;

//     err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
//     if (err != ESP_OK) {
//         ESP_LOGE(TAG, "Error opening NVS");
//         goto exit;
//     }

//     int64_t timestamp = 0;

//     err = nvs_get_i64(my_handle, "timestamp", &timestamp);
//     if (err == ESP_ERR_NVS_NOT_FOUND) {
//         ESP_LOGI(TAG, "Time not found in NVS. Syncing time from SNTP server.");
//         if (fetch_and_store_time_in_nvs(NULL) != ESP_OK) {
//             err = ESP_FAIL;
//         } else {
//             err = ESP_OK;
//         }
//     } else if (err == ESP_OK) {
//         struct timeval get_nvs_time;
//         get_nvs_time.tv_sec = timestamp;
//         settimeofday(&get_nvs_time, NULL);
//     }

// exit:
//     nvs_close(my_handle);
//     return err;
// }

esp_err_t sntp_deinit()
{
    esp_netif_sntp_deinit();
    return ESP_OK;
}
