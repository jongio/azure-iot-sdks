// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef IOTHUBTRANSPORTAMQP_AMQP_CONNECTION_H
#define IOTHUBTRANSPORTAMQP_AMQP_CONNECTION_H

#include "azure_c_shared_utility\xio.h"
#include "azure_uamqp_c\session.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum CONNECTION_ERROR_CODE
	{
		CONNECTION_ERROR,
		CONNECTION_CBS_ERROR
	};

	typedef (void)(*ON_CONNECTION_ERROR_CALLBACK)(CONNECTION_ERROR_CODE error_code, const void* context);

	typedef struct AMQP_CONNECTION_CONFIG_TAG
	{
		const char* iothub_host_fqdn;
		XIO_HANDLE xio_handle;
		bool create_sasl_io;
		bool create_cbs_connection;
		bool is_trace_on;

		ON_CONNECTION_ERROR_CALLBACK on_error_callback;
		const void* on_error_context;
	} AMQP_CONNECTION_CONFIG;

	typedef struct AMQP_CONNECTION_STATE* AMQP_CONNECTION_HANDLE;

	AMQP_CONNECTION_HANDLE amqp_connection_create(AMQP_CONNECTION_CONFIG* config);
	void amqp_connection_destroy(AMQP_CONNECTION_HANDLE conn_handle);
	void amqp_connection_do_work(AMQP_CONNECTION_HANDLE conn_handle);
	int amqp_connection_get_session(AMQP_CONNECTION_HANDLE conn_handle, SESSION_HANDLE* session_handle);
	int amqp_connection_get_cbs_handle(AMQP_CONNECTION_HANDLE conn_handle, CBS_HANDLE* cbs_handle);

#ifdef __cplusplus
}
#endif

#endif /*IOTHUBTRANSPORTAMQP_AMQP_CONNECTION_H*/
