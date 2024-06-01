#ifndef __SNTP_SYNC_H__
#define __SNTP_SYNC_H__

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif_sntp.h"
#include "lwip/ip_addr.h"
#include "esp_sntp.h"
#include "inttypes.h"

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 48
#endif

#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_CUSTOM
/**
 * @brief 
 * 
 * @param tv 
 */
void sntp_sync_time(struct timeval *tv);
#endif

/**
 * @brief Initialize SNTP with a callback function
 * 
 * @param sntp_callback Callback function for SNTP time synchronization
 * @return esp_err_t 
 */
esp_err_t sntp_initialize(void *sntp_callback);

/**
 * @brief Set the time zone to Vietnam
 */
void sntp_setTimmeZoneToVN(void);

/**
 * @brief Print SNTP servers
 */
void sntp_printServerInformation(void);

/**
 * @brief Synchronize time with SNTP servers
 * 
 * @return esp_err_t 
 */
esp_err_t sntp_syncTime(void);

/**
 * @brief Deinitialize SNTP
 * 
 * @return esp_err_t 
 */
esp_err_t sntp_deinit(void);

// /**
//  * @brief Update the system time from time stored in NVS.
//  *
//  */

// esp_err_t update_time_from_nvs(void);

// /**
//  * @brief Fetch the current time from SNTP and stores it in NVS.
//  *
//  */
// esp_err_t fetch_and_store_time_in_nvs(void*);

#endif
