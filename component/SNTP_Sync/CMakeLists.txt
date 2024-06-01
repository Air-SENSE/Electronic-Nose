set(app_src sntp_sync.c)
set(pre_req lwip log esp_netif)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})