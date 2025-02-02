/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __ATCMD_BT_UTILS_H__
#define __ATCMD_BT_UTILS_H__

#include <basic_types.h>
#include <stdint.h>

#if defined(CONFIG_ATCMD_IO_UART) && CONFIG_ATCMD_IO_UART
#include <log_service.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(CONFIG_ATCMD_IO_UART) && CONFIG_ATCMD_IO_UART

#if defined(ATCMD_BT_CUT_DOWN) && ATCMD_BT_CUT_DOWN
#define BT_AT_PRINT(fmt, args...)
#else /* ATCMD_BT_CUT_DOWN */
#define BT_AT_PRINT(fmt, args...) at_printf(fmt, ##args)
#endif /* ATCMD_BT_CUT_DOWN */

#else /* CONFIG_ATCMD_IO_UART */
#define BT_AT_PRINT(fmt, args...)
#endif /* CONFIG_ATCMD_IO_UART */

#define BT_AT_PRINTOK() BT_AT_PRINT("OK\r\n")
#define BT_AT_PRINTERROR() BT_AT_PRINT("ERROR\r\n")

void ble_addr_to_str(void *paddr, char *str, uint32_t len);

int str_to_int(char *str);

bool hexdata_str_to_bd_addr(char *str, uint8_t *addr_buf, uint8_t buf_len);

bool hexnum_str_to_array(char *str, uint8_t *byte_arr, uint8_t arr_len);

bool hexdata_str_to_array(char *str, uint8_t *byte_arr, uint8_t arr_len);

#ifdef __cplusplus
}
#endif

#endif /* __ATCMD_BT_UTILS_H__ */