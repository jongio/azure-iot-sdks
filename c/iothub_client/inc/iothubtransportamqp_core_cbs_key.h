// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef IOTHUBTRANSPORTAMQP_CORE_CBS_KEY
#define IOTHUBTRANSPORTAMQP_CORE_CBS_KEY

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct AMQPTRANSPORT_DEVICE_CONFIG_KEY_TAG
	{
		const char* device_id;
		const char* device_primary_key;
		const char* device_secondary_key;
	} AMQPTRANSPORT_DEVICE_CONFIG_KEY;

	extern const AMQPTRANSPORT_CORE_INTERFACE* AMQP_transport_CBS_device_key_interface(void);

#ifdef __cplusplus
}
#endif

#endif /*IOTHUBTRANSPORTAMQP_CORE_CBS_KEY*/
