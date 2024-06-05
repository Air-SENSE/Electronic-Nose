set(app_src FileServer.c)
set(pre_req vfs fatfs esp_http_server)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req}
                    EMBED_FILES "favicon.ico" "upload_script.html")
