set(app_src sht3x.c)
set(pre_req i2cdev log esp_idf_lib_helpers esp_timer)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})