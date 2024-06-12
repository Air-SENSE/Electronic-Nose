/**
 * @file sdcard.h
 * @author Nguyen Nhu Hai Long ( @long27032002 )
 * @brief 
 * @version 0.1
 * @date 2022-11-29
 * @copyright Copyright (c) 2022
 */
#ifndef __FILESERVER_H__
#define __FILESERVER_H__

#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include "esp_err.h"
#include "esp_log.h"

#include "esp_vfs.h"
#include "esp_http_server.h"

/* Max length a file path can have on storage */
#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + CONFIG_SPIFFS_OBJ_NAME_LEN + 17)

/* Scratch buffer size */
#define SCRATCH_BUFSIZE  8192

#define IS_FILE_EXT(filename, ext) \
    (strcasecmp(&filename[strlen(filename) - sizeof(ext) + 1], ext) == 0)

struct file_server_data {
    /* Base path of file storage */
    char base_path[ESP_VFS_PATH_MAX + 1];

    /* Scratch buffer for temporary storage during file transfer */
    char scratch[SCRATCH_BUFSIZE];
};

esp_err_t index_html_get_handler(httpd_req_t *req);

esp_err_t favicon_get_handler(httpd_req_t *req);

esp_err_t http_response_dir_html(httpd_req_t *req, const char *dirpath);

esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filename);

const char* get_path_from_uri(char *dest, const char *base_path, const char *uri, size_t destsize);

esp_err_t download_get_handler(httpd_req_t *req);

esp_err_t delete_post_handler(httpd_req_t *req);

esp_err_t start_file_server(const char *base_path);

#endif
