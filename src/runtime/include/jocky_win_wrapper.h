#ifndef JOCKY_WIN_WRAPPER_H
#define JOCKY_WIN_WRAPPER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* System & Process Information Helpers */
int32_t jocky_win_get_process_id(void);
int32_t jocky_win_get_system_info(char* out_buffer, int32_t max_len);
int64_t jocky_win_get_tick_count(void);
int32_t jocky_win_get_computer_name(char* out_buffer, int32_t max_len);

/* Memory Information Helpers */
int32_t jocky_win_get_memory_status(char* out_buffer, int32_t max_len);

/* File & Environment Helpers */
int32_t jocky_win_get_temp_path(char* out_buffer, int32_t max_len);
bool jocky_win_file_exists(const char* file_path);

/* User Interface Helpers */
int32_t jocky_win_show_msgbox(const char* title, const char* message, int32_t flags);

#ifdef __cplusplus
}
#endif

#endif /* JOCKY_WIN_WRAPPER_H */
