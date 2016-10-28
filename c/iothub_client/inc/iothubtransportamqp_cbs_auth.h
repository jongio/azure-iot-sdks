// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef IOTHUBTRANSPORTAMQP_CBS_AUTH_H
#define IOTHUBTRANSPORTAMQP_CBS_AUTH_H

#include "iothub_transport_ll.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum AUTHENTICATION_START_TAG
	{
		AUTHENTICATION_START_RESULT_SUCCESS,
		AUTHENTICATION_START_RESULT_ERROR,
		AUTHENTICATION_START_RESULT_TIMEOUT
	} AUTHENTICATION_START_RESULT;

	typedef enum AUTHENTICATION_STOP_TAG
	{
		AUTHENTICATION_STOP_RESULT_SUCCESS,
		AUTHENTICATION_STOP_RESULT_ERROR,
		AUTHENTICATION_STOP_RESULT_TIMEOUT
	} AUTHENTICATION_STOP_RESULT;

	typedef enum AUTHENTICATION_ERROR_TAG
	{
		AUTHENTICATION_ERROR_SAS_REFRESH_FAILED
	} AUTHENTICATION_ERROR_CODE;

	typedef void(*ON_AUTHENTICATION_START_COMPLETED)(AUTHENTICATION_START_RESULT result, void* context);
	typedef void(*ON_AUTHENTICATION_STOP_COMPLETED)(AUTHENTICATION_STOP_RESULT result, void* context);
	typedef void(*ON_AUTHENTICATION_ERROR)(AUTHENTICATION_ERROR error_code, void* context);

	typedef struct AUTHENTICATION_CONFIG_TAG
	{
		char* device_id;
		char* device_primary_key;
		char* device_secondary_key;
		char* device_sas_token;
		char* iothub_host_fqdn;

		ON_AUTHENTICATION_ERROR on_error_callback;
		void* on_error_context;
	} AUTHENTICATION_CONFIG;

	typedef struct AUTHENTICATION_STATE* AUTHENTICATION_HANDLE;

	extern AUTHENTICATION_HANDLE authentication_create(const AUTHENTICATION_CONFIG* config);
	extern int authentication_start(AUTHENTICATION_HANDLE authentication_handle, const CBS_HANDLE cbs_handle, ON_AUTHENTICATION_START_COMPLETED on_start_completed, const void* context);
	extern int authentication_stop(AUTHENTICATION_HANDLE authentication_handle, ON_AUTHENTICATION_STOP_COMPLETED on_stop_completed, const void* context);
	extern void authentication_do_work(AUTHENTICATION_HANDLE authentication_handle);
	extern void authentication_destroy(AUTHENTICATION_HANDLE authentication_handle);

	extern int authentication_set_cbs_request_timeout_secs(AUTHENTICATION_HANDLE authentication_handle, uint32 value);
	extern int authentication_set_sas_token_refresh_time_secs(AUTHENTICATION_HANDLE authentication_handle, uint32 value);
	extern int authentication_set_sas_token_lifetime_secs(AUTHENTICATION_HANDLE authentication_handle, uint32 value);

#ifdef __cplusplus
}
#endif

#endif /*IOTHUBTRANSPORTAMQP_CBS_AUTH_H*/
