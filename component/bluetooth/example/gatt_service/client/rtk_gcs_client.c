/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_gattc.h>
#include <rtk_client_config.h>

void gattc_dump(uint16_t len, uint8_t *data, uint8_t *tag)
{
	printf("[APP] GATTC %s data(len %d): \n\r", tag, len);

	for (uint8_t j = 0; j < 2; j++) {
		for (uint16_t i = 0; i < len; i++) {
			if (!i && 0 == j) {
				printf("[HEX]: \n\r");
			} else if (!i && 1 == j) {
				printf("[ASCII]: \n\r");
			}
			if (0 == i % 16 && i && 0 == j) {
				printf("\n\r");
			}
			if (0 == j) {
				printf("%02x ", *(data + i));
			} else if (1 == j) {
				printf("%c ", *(data + i));
			}
		}
		printf("\n\r");
	}
	printf("\n\r");
}

#if !defined(RTK_BLE_MGR_LIB) || !RTK_BLE_MGR_LIB
void general_client_discover_res_hdl(void *data)
{
	rtk_bt_gattc_discover_ind_t *disc_res = (rtk_bt_gattc_discover_ind_t *)data;
	rtk_bt_status_t disc_status = disc_res->status;
	uint32_t uuid_128[4];

	if (RTK_BT_STATUS_CONTINUE == disc_status) {
		switch (disc_res->type) {
		case RTK_BT_GATT_DISCOVER_PRIMARY_ALL:
			if (BT_UUID_TYPE_16 == disc_res->disc_primary_all_per.uuid_type) {
				printf("[APP] GATTC discover primary result: start_handle: 0x%04x, end_handle: 0x%04x, UUID: %04x\r\n",
					   disc_res->disc_primary_all_per.start_handle, disc_res->disc_primary_all_per.end_handle,
					   *(uint16_t *)disc_res->disc_primary_all_per.uuid);
			} else if (BT_UUID_TYPE_128 == disc_res->disc_primary_all_per.uuid_type) {
				/* if use UUID128_VAL(disc_res->disc_primary_all_per.uuid), it may lead to address missaligned crash in kr4,
				    becasue disc_res->disc_primary_all_per.uuid may not be 4 Bytes aligned */
				memcpy(uuid_128, disc_res->disc_primary_all_per.uuid, 16);
				printf("[APP] GATTC discover primary result: start_handle: 0x%04x, end_handle: 0x%04x, UUID: "UUID128_STR,
					   disc_res->disc_primary_all_per.start_handle, disc_res->disc_primary_all_per.end_handle,
					   UUID128_VAL((uint8_t *)uuid_128));
				printf("\r\n");
			}
			break;

		case RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID:
			printf("[APP] GATTC discover primary by uuid result: start_handle: 0x%04x, end_handle: 0x%04x\r\n",
				   disc_res->disc_primary_by_uuid_per.start_handle, disc_res->disc_primary_by_uuid_per.end_handle);
			break;

		case RTK_BT_GATT_DISCOVER_INCLUDE:
			printf("[APP] GATTC discover include result: handle: 0x%04x, start_handle: 0x%04x, end_handle: 0x%04x, UUID: ",
				   disc_res->disc_inc_per.handle, disc_res->disc_inc_per.start_handle, disc_res->disc_inc_per.end_handle);
			if (BT_UUID_TYPE_16 == disc_res->disc_inc_per.uuid_type) {
				printf("%04x\r\n", *(uint16_t *)disc_res->disc_inc_per.uuid);
			} else if (BT_UUID_TYPE_128 == disc_res->disc_inc_per.uuid_type) {
				memcpy(uuid_128, disc_res->disc_inc_per.uuid, 16);
				printf(UUID128_STR, UUID128_VAL((uint8_t *)uuid_128));
				printf("\r\n");
			}
			break;

		case RTK_BT_GATT_DISCOVER_CHARACTERISTIC_ALL:
		case RTK_BT_GATT_DISCOVER_CHARACTERISTIC_BY_UUID:
			printf("[APP] GATTC discover characteritic of a service result: handle: 0x%04x, "
				   "properties: 0x%02x, value_handle: 0x%04x, UUID: ", disc_res->disc_char_all_per.handle,
				   disc_res->disc_char_all_per.properties, disc_res->disc_char_all_per.value_handle);
			if (BT_UUID_TYPE_16 == disc_res->disc_char_all_per.uuid_type) {
				printf("%04x\r\n", *(uint16_t *)disc_res->disc_char_all_per.uuid);
			} else if (BT_UUID_TYPE_128 == disc_res->disc_char_all_per.uuid_type) {
				memcpy(uuid_128, disc_res->disc_char_all_per.uuid, 16);
				printf(UUID128_STR, UUID128_VAL((uint8_t *)uuid_128));
				printf("\r\n");
			}
			break;

		case RTK_BT_GATT_DISCOVER_DESCRIPTORS_ALL:
			printf("[APP] GATTC discover discriptor all result: handle: 0x%04x, UUID: ",
				   disc_res->disc_descriptor_per.handle);
			if (BT_UUID_TYPE_16 == disc_res->disc_descriptor_per.uuid_type) {
				printf("%04x\r\n", *(uint16_t *)disc_res->disc_descriptor_per.uuid);
			} else if (BT_UUID_TYPE_128 == disc_res->disc_descriptor_per.uuid_type) {
				memcpy(uuid_128, disc_res->disc_descriptor_per.uuid, 16);
				printf(UUID128_STR, UUID128_VAL((uint8_t *)uuid_128));
				printf("\r\n");
			}
			break;

		default:
			printf("[APP] GATTC discover result event, wrong event type (%d)!\r\n", disc_res->type);
			break;
		}
	} else if (RTK_BT_STATUS_DONE == disc_status) {
		printf("[APP] GATTC discover completed. "
			   "profile_id: %d, conn_handle: %d, type: %d\r\n",
			   disc_res->profile_id, disc_res->conn_handle, disc_res->type);
		APP_PRINT_SEPARATOR();
		return;
	} else {
		printf("[APP] GATTC discover failed! "
			   "profile_id: %d, conn_handle: %d, type: %d, err: 0x%x\r\n",
			   disc_res->profile_id, disc_res->conn_handle,
			   disc_res->type, disc_res->err_code);
		APP_PRINT_SEPARATOR();
		return;
	}
}

void general_client_read_res_hdl(void *data)
{
	rtk_bt_gattc_read_ind_t *read_res = (rtk_bt_gattc_read_ind_t *)data;
	rtk_bt_status_t read_status = read_res->status;
	uint16_t handle = 1;

	if (RTK_BT_STATUS_FAIL == read_status) {
		printf("[APP] GATTC read failed, "
			   "profile_id: %d, conn_handle: %d, type: %d, err: 0x%x\r\n",
			   read_res->profile_id, read_res->conn_handle,
			   read_res->type, read_res->err_code);
		APP_PRINT_SEPARATOR();
		return;
	} else if (RTK_BT_STATUS_CONTINUE == read_status) {
		if (!read_res->by_handle.len || !read_res->by_handle.value) {
			printf("[APP] GATT client read value is empty!\r\n");
			return;
		}
		switch (read_res->type) {
		case RTK_BT_GATT_CHAR_READ_BY_HANDLE:
			handle = read_res->by_handle.handle;
			printf("[APP] GATT client read result, profile_id: %d, conn_handle: %d, "
				   "type: %d, status: %d, handle: 0x%04x\r\n",
				   read_res->profile_id, read_res->conn_handle,
				   read_res->type, read_status, handle);
			gattc_dump(read_res->by_handle.len, read_res->by_handle.value, (uint8_t *)"read result");
			break;

		case RTK_BT_GATT_CHAR_READ_BY_UUID:
			handle = read_res->by_uuid_per.handle;
			printf("[APP] GATT client read result, profile_id: %d, conn_handle: %d, "
				   "type: %d, status: %d, handle: 0x%04x\r\n",
				   read_res->profile_id, read_res->conn_handle,
				   read_res->type, read_status, handle);
			gattc_dump(read_res->by_uuid_per.len, read_res->by_uuid_per.value, (uint8_t *)"read result");
			break;

		case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE:
			printf("[APP] GATT client read result, profile_id: %d, conn_handle: %d, "
				   "type: %d, status: %d\r\n",
				   read_res->profile_id, read_res->conn_handle,
				   read_res->type, read_status);
			gattc_dump(read_res->multiple_variable_per.len, read_res->multiple_variable_per.value, (uint8_t *)"read result");
			break;

		default:
			break;
		}
	} else if (RTK_BT_STATUS_DONE == read_status) {
		printf("[APP] GATTC read completed, "
			   "profile_id: %d, conn_handle: %d, type: %d\r\n",
			   read_res->profile_id, read_res->conn_handle, read_res->type);
		APP_PRINT_SEPARATOR();
	}

}

void general_client_write_res_hdl(void *data)
{
	rtk_bt_gattc_write_ind_t *write_res = (rtk_bt_gattc_write_ind_t *)data;
	rtk_bt_status_t write_status =  write_res->status;

	if (RTK_BT_STATUS_FAIL == write_status) {
		printf("[APP] GATTC write failed, "
			   "profile_id: %d, conn_handle: %d, type: %d, err: 0x%x\r\n",
			   write_res->profile_id, write_res->conn_handle,
			   write_res->type, write_res->err_code);
		return;
	} else if (RTK_BT_STATUS_DONE == write_status) {
		printf("[APP] GATTC write completed, "
			   "profile_id: %d, conn_handle: %d, type: %d, handle: 0x%x\r\n",
			   write_res->profile_id, write_res->conn_handle,
			   write_res->type, write_res->handle);
	} else {
		return;
	}
}

void general_client_notify_hdl(void *data)
{
	rtk_bt_gattc_cccd_value_ind_t *ntf_ind = (rtk_bt_gattc_cccd_value_ind_t *)data;

	if (!ntf_ind->len || !ntf_ind->value) {
		printf("[APP] GATTC notify received value is empty!\r\n");
		return;
	}
	printf("[APP] GATTC notify received, profile_id: %d, conn_handle: %d, handle: 0x%x\r\n",
		   ntf_ind->profile_id, ntf_ind->conn_handle, ntf_ind->value_handle);
	gattc_dump(ntf_ind->len, ntf_ind->value, (uint8_t *)"notify event");
}

void general_client_indicate_hdl(void *data)
{
	rtk_bt_gattc_cccd_value_ind_t *indicate_ind = (rtk_bt_gattc_cccd_value_ind_t *)data;

	if (!indicate_ind->len || !indicate_ind->value) {
		printf("[APP] GATTC indicate received value is empty!\r\n");
		return;
	}
	printf("[APP] GATTC indicate received, profile_id: %d, conn_handle: %d, handle: 0x%x\r\n",
		   indicate_ind->profile_id, indicate_ind->conn_handle, indicate_ind->value_handle);
	gattc_dump(indicate_ind->len, indicate_ind->value, (uint8_t *)"indicate event");
}

void general_client_cccd_enable_hdl(void *data)
{
	rtk_bt_gattc_cccd_update_ind_t *cccd_update =
		(rtk_bt_gattc_cccd_update_ind_t *)data;
	rtk_bt_status_t status = cccd_update->status;
	if (RTK_BT_STATUS_DONE == status) {
		printf("[APP] GATTC enable cccd succeed, "
			   "profile_id: %d, conn_handle: %d, handle: 0x%x\r\n",
			   cccd_update->profile_id, cccd_update->conn_handle,
			   cccd_update->cccd_handle);
	} else {
		printf("[APP] GATTC enable cccd failed, "
			   "profile_id: %d, conn_handle: %d, handle: 0x%x, err: 0x%x\r\n",
			   cccd_update->profile_id, cccd_update->conn_handle,
			   cccd_update->cccd_handle, cccd_update->err_code);
	}
}

void general_client_cccd_disable_hdl(void *data)
{
	rtk_bt_gattc_cccd_update_ind_t *cccd_update = (rtk_bt_gattc_cccd_update_ind_t *)data;
	rtk_bt_status_t status = cccd_update->status;
	if (RTK_BT_STATUS_DONE == status) {
		printf("[APP] GATTC disable cccd succeed, "
			   "profile_id: %d, conn_handle: %d, handle: 0x%x\r\n",
			   cccd_update->profile_id, cccd_update->conn_handle,
			   cccd_update->cccd_handle);
	} else {
		printf("[APP] GATTC disable cccd failed, "
			   "profile_id: %d, conn_handle: %d, handle: 0x%x, err: 0x%x\r\n",
			   cccd_update->profile_id, cccd_update->conn_handle,
			   cccd_update->cccd_handle, cccd_update->err_code);
	}
}

rtk_bt_evt_cb_ret_t general_client_app_callback(uint8_t event, void *data)
{
	switch (event) {
	case RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND:
		general_client_discover_res_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_READ_RESULT_IND:
		general_client_read_res_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_WRITE_RESULT_IND:
		general_client_write_res_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_NOTIFY_IND:
		general_client_notify_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_INDICATE_IND:
		general_client_indicate_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_CCCD_ENABLE_IND:
		general_client_cccd_enable_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_CCCD_DISABLE_IND:
		general_client_cccd_disable_hdl(data);
		break;
	default:
		break;
	}

	return RTK_BT_EVT_CB_OK;
}

uint16_t general_client_attach_conn(uint16_t conn_handle)
{
	(void)conn_handle;
	return RTK_BT_OK;
}

uint16_t general_client_detach_conn(uint16_t conn_handle)
{
	(void)conn_handle;
	return RTK_BT_OK;
}

uint16_t general_client_add(void)
{
	return rtk_bt_gattc_register_profile(GCS_CLIENT_PROFILE_ID);
}

uint16_t general_client_delete(void)
{
	return RTK_BT_OK;
}

#else
/* NEW GATT CLIENT does NOT support general client */
rtk_bt_evt_cb_ret_t general_client_app_callback(uint8_t event, void *data)
{
	(void)event;
	(void)data;
	return RTK_BT_EVT_CB_OK;
}

uint16_t general_client_attach_conn(uint16_t conn_handle)
{
	(void)conn_handle;
	return 0;
}

uint16_t general_client_detach_conn(uint16_t conn_handle)
{
	(void)conn_handle;
	return 0;
}

uint16_t general_client_add(void)
{
	return 0;
}

uint16_t general_client_delete(void)
{
	return 0;
}

#endif