# iothubtransportamqp_core_cbs_key Requirements


## Overview

This module provides the code implementation of the AMQP transport to support authentication through CBS using device keys to create SAS tokens.
   

## Dependencies

This module will depend on the following modules:

iothubtransportamqp_core_interface
iothubtransportamqp_core_common
iothubtransportamqp_cbs_auth
iothubtransportamqp_messenger
azure-c-shared-utilities
azure-c-uamqp


## Interface 

This module implements the AMQPTRANSPORT_CORE_INTERFACE.
Refer to the iothubtransportamqp_core_interface module for details.


## Exposed API

```c
typedef struct AMQPTRANSPORT_DEVICE_CONFIG_KEY_TAG
{
    const char* device_id;
    const char* device_primary_key;
    const char* device_secondary_key;
} AMQPTRANSPORT_DEVICE_CONFIG_KEY;

extern const AMQPTRANSPORT_CORE_INTERFACE* AMQP_transport_CBS_device_key_interface(void);
```

AMQP_transport_CBS_device_key_interface(void) returns the following function pointers:

AMQPTRANSPORT_CORE_HANDLE amqptransport_core_create(const char* iothub_host_fqdn, ON_AMQPTRANSPORT_ERROR on_error, const void* context);
void amqptransport_core_destroy(AMQPTRANSPORT_CORE_HANDLE handle);
int amqptransport_core_start(AMQPTRANSPORT_CORE_HANDLE handle, const xio_handle base_io, ON_AMQPTRANSPORT_START_COMPLETE on_start_complete, const void* context);
int amqptransport_core_stop(AMQPTRANSPORT_CORE_HANDLE handle, ON_AMQPTRANSPORT_STOP_COMPLETE on_stop_complete, const void* context);
void amqptransport_core_do_work(AMQPTRANSPORT_CORE_HANDLE handle);
int  amqptransport_core_subscribe_for_messages(AMQPTRANSPORT_DEVICE_HANDLE device_handle);
int  amqptransport_core_unsubscribe_for_messages(AMQPTRANSPORT_DEVICE_HANDLE device_handle);
AMQPTRANSPORT_DEVICE_HANDLE amqptransport_core_register_device(AMQPTRANSPORT_CORE_HANDLE handle, AMQPTRANSPORT_DEVICE_CONFIG* device_config);
int amqptransport_core_unregister_device(AMQPTRANSPORT_DEVICE_HANDLE device_handle);
int amqptransport_core_get_send_status(AMQPTRANSPORT_DEVICE_HANDLE device_handle, AMQPTRANSPORT_SEND_STATUS* send_status);
int amqptransport_core_set_option(AMQPTRANSPORT_CORE_HANDLE handle, const char* name, const void* value);


## amqptransport_core_create

```c
AMQPTRANSPORT_CORE_HANDLE amqptransport_core_create(const char* iothub_host_fqdn, ON_AMQPTRANSPORT_ERROR on_error, const void* context);
```

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_001: [**If iothub_host_fqdn is NULL, amqptransport_core_create() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_002: [**amqptransport_core_create() shall allocate memory for the AMQPTRANSPORT_STATE**]**
Note: the instance of AMQPTRANSPORT_STATE shall be referred to as `state` through the remaining requirements.

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_003: [**If malloc() fails, amqptransport_core_create() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_004: [**A copy of `iothub_host_fqdn` shall be saved on `state->iothub_host_fqdn`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_005: [**If STRING_construct() fails, amqptransport_core_create() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_006: [**If no failures occurr, amqptransport_core_create() shall return the `state` handle as AMQPTRANSPORT_CORE_HANDLE**]**


## amqptransport_core_destroy

```c
void amqptransport_core_destroy(AMQPTRANSPORT_CORE_HANDLE handle);
```

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_007: [**If `handle` is NULL, amqptransport_core_destroy() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_008: [**If `state` is started, amqptransport_core_stop() shall be invoked**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_009: [**If amqptransport_core_stop() fails, amqptransport_core_destroy() shall ignore it and continue**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_010: [**amqptransport_core_destroy() shall unregister all its registered devices using amqptransport_core_unregister_device()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_011: [**`state->amqp_connection` shall be destroyed using amqp_connection_destroy()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_012: [**The `state` fields shall be destroyed and the memory allocated for the `state` shall be freed**]**


## amqptransport_core_start

```c
int amqptransport_core_start(AMQPTRANSPORT_CORE_HANDLE handle, const xio_handle base_io, ON_AMQPTRANSPORT_START_COMPLETE on_start_complete, const void* context);
```

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_013: [**If `handle` is NULL, amqptransport_core_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_014: [**If `base_io` is NULL, amqptransport_core_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_015: [**If `state` is already started, amqptransport_core_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_016: [**amqptransport_core_start() shall create the `state->amqp_connection` with support for CBS using amqp_connection_create(), passing `base_io` as argument**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_017: [**If amqp_connection_create() fails, amqptransport_core_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_018: [**If there are registered devices, amqptransport_core_start() shall start each registered device**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_019: [**The `cbs_handle` shall be obtained from `state->amqp_connection` using amqp_connection_get_cbs_handle()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_020: [**Each registered device `device_state->authentication_handle` shall be started using authentication_start() and passing the `cbs_handle` as argument**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_021: [**authentication_start() shall be invoked passing `on_authentication_complete_callback`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_022: [**If authentication_start() fails, amqptransport_core_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_023: [**If authentication_start() succeeds, `device_state` shall be marked as authenticating**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_024: [**The `session_handle` shall be obtained from `state->amqp_connection` using amqp_connection_get_session()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_025: [**Each registered device `device_state->messenger` shall be started using messenger_start and passing the `session_handle` as argument**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_026: [**If messenger_start() fails, amqptransport_core_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_027: [**If no failures occurr, `on_start_complete` shall be invoked passing `context`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_028: [**If no failures occurr, amqptransport_core_start() shall return 0**]**


## amqptransport_core_stop

```c
int amqptransport_core_stop(AMQPTRANSPORT_CORE_HANDLE handle, ON_AMQPTRANSPORT_STOP_COMPLETE on_stop_complete, const void* context);
```

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_029: [**If `handle` is NULL, amqptransport_core_stop() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_030: [**If `state` is already stopped, amqptransport_core_stop() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_031: [**If there are registered devices, amqptransport_core_stop() shall stop each registered device**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_032: [**amqptransport_core_stop() shall stop each registered device `device_state->authentication_handle` using authentication_stop()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_033: [**If authentication_stop() fails, amqptransport_core_stop() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_034: [**amqptransport_core_stop() shall stop each registered device `device_state->messenger` using messsenger_stop()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_035: [**If messenger_stop() fails, amqptransport_core_stop() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_036: [**`state->amqp_connection` shall be destroyed using amqp_connection_destroy()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_037: [**If no failures occurr, `on_stop_complete` shall be invoked passing `context`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_038: [**If no failures occurr, amqptransport_core_stop() shall return 0**]**


## amqptransport_core_do_work

```c
void amqptransport_core_do_work(AMQPTRANSPORT_CORE_HANDLE handle);
```

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_039: [**If `handle` is NULL, amqptransport_core_do_work() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_040: [**If `state` is not started, amqptransport_core_do_work() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_041: [**If `state->registered_devices` is empty, amqptransport_core_do_work() shall return**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_042: [**amqptransport_core_do_work() shall iterate through each `state->registered_devices` process authentication and message I/O**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_043: [**amqptransport_core_do_work() shall invoke authentication_do_work() over each `device_state->authentication_handle`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_044: [**If the current `device_state` is not authenticated, amqptransport_core_do_work() shall skip to the next registered device**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_045: [**amqptransport_core_do_work() shall invoke messenger_do_work() over each `device_state->messenger`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_046: [**amqptransport_core_do_work() shall invoke amqp_connection_do_work() over `state->amqp_connection`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_047: [**If on_amqp_connection_error_callback is invoked, `state->on_error` shall be invoked with AMQPTRANSPORT_ERROR_CONNECTION_FAILURE**]**


## amqptransport_core_subscribe_for_messages

```c
int  amqptransport_core_subscribe_for_messages(AMQPTRANSPORT_DEVICE_HANDLE device_handle);
```

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_048: [**If `device_handle` is NULL, amqptransport_core_do_work() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_049: [**If the device is not in `state->registered_devices`, amqptransport_core_do_work() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_050: [**amqptransport_core_do_work() shall invoke messenger_subscribe_for_messsages() on `device_state->messenger` and return its result**]**


## amqptransport_core_unsubscribe_for_messages

```c
int  amqptransport_core_unsubscribe_for_messages(AMQPTRANSPORT_DEVICE_HANDLE device_handle);
```

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_051: [**If `device_handle` is NULL, amqptransport_core_do_work() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_052: [**If the device is not in `state->registered_devices`, amqptransport_core_do_work() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_053: [**amqptransport_core_do_work() shall invoke messenger_unsubscribe_for_messsages() on `device_state->messenger` and return its result**]**


## amqptransport_core_register_device

```c
AMQPTRANSPORT_DEVICE_HANDLE amqptransport_core_register_device(AMQPTRANSPORT_CORE_HANDLE handle, AMQPTRANSPORT_DEVICE_CONFIG* device_config);
```

Note: `device_config` is expected to be an instance of AMQPTRANSPORT_DEVICE_CONFIG_KEY.

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_054: [**If `handle` is NULL, amqptransport_core_register_device() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_055: [**If `device_config` is NULL, amqptransport_core_register_device() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_056: [**If the device is already registered, amqptransport_core_register_device() shall fail and return NULL**]**

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_057: [**amqptransport_core_register_device() shall allocate memory for the AMQPTRANSPORT_DEVICE_STATE**]**
Note: the instance of AMQPTRANSPORT_DEVICE_STATE shall be referred to as `device_state` through the remaining requirements.

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_058: [**If malloc() fails, amqptransport_core_register_device() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_059: [**A copy of `device_config` shall be saved on `device_state->config`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_060: [**If `device_config` fails to be copied, amqptransport_core_register_device() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_061: [**`device_state->messenger` shall be set using messenger_create()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_062: [**If messenger_create() fails, amqptransport_core_register_device() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_063: [**`device_state->authentication_handle` shall be set using authentication_create(), passing the device keys**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_064: [**If authentication_create() fails, amqptransport_core_register_device() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_065: [**If `state` is started, amqptransport_core_register_device() shall start the `device_state`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_066: [**The `session_handle` shall be obtained from `state->amqp_connection` using amqp_connection_get_session()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_067: [**The `device_state->messenger` shall be started using messenger_start() and passing the `session_handle` as argument**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_068: [**If messenger_start() fails, amqptransport_core_register_device() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_069: [**If any failure occurs, amqptransport_core_register_device() shall free all the memory it has allocated**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_070: [**If no failures occurr, amqptransport_core_register_device() shall add `device_state` to `state->registered_devices`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_071: [**If no failures occurr, amqptransport_core_register_device() shall return `device_state` as AMQPTRANSPORT_DEVICE_HANDLE**]**


## amqptransport_core_unregister_device

```c
int amqptransport_core_unregister_device(AMQPTRANSPORT_DEVICE_HANDLE device_handle);
```

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_072: [**If `device_handle` is NULL, amqptransport_core_do_work() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_073: [**If the device is not in `state->registered_devices`, amqptransport_core_do_work() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_074: [**`device_state->messenger` shall be destroyed using messenger_destroy()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_075: [**`device_state->authentication_handle` shall be destroyed using authentication_destroy()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_076: [**The `device_state` shall be removed from `state->registered_devices`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_077: [**`device_state` shall be destroyed using free()**]**


## amqptransport_core_get_send_status

```c
int amqptransport_core_get_send_status(AMQPTRANSPORT_DEVICE_HANDLE device_handle, AMQPTRANSPORT_SEND_STATUS* send_status);
```

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_078: [**If `device_handle` is NULL, amqptransport_core_get_send_status() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_079: [**If `send_status` is NULL, amqptransport_core_get_send_status() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_080: [**The `device_state->messenger` send status shall ve retrieved using messenger_get_send_status()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_081: [**If messenger_get_send_status() fails, amqptransport_core_get_send_status() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_082: [**If the result is MESSENGER_SEND_STATUS_BUSY, `send_state` shall be set to AMQPTRANSPORT_SEND_STATUS_BUSY**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_083: [**If the result is MESSENGER_SEND_STATUS_IDLE, `send_state` shall be set to AMQPTRANSPORT_SEND_STATUS_IDLE**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_084: [**If no failures occurr, amqptransport_core_get_send_status() shall return 0**]**


## amqptransport_core_set_option

```c
int amqptransport_core_set_option(AMQPTRANSPORT_CORE_HANDLE handle, const char* name, const void* value);
```

**SRS_IOTHUBTRANSPORTAMQP_CORE_09_085: [**If `handle` is NULL, amqptransport_core_do_work() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_086: [**If `name` is NULL, amqptransport_core_do_work() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_087: [**If `name` is "cbs_request_timeout_secs", amqptransport_core_do_work() shall invoke authentication_set_cbs_request_timeout_secs() on each `device_state->authentication_handle` passing `value`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_088: [**If authentication_set_cbs_request_timeout_secs() fails, amqptransport_core_do_work() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_089: [**If `name` is "sas_token_refresh_time_secs", amqptransport_core_do_work() shall invoke authentication_set_sas_token_refresh_time_secs() on each `device_state->authentication_handle` passing `value`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_090: [**If authentication_set_sas_token_refresh_time_secs() fails, amqptransport_core_do_work() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_091: [**If `name` is "sas_token_lifetime_secs", amqptransport_core_do_work() shall invoke authentication_set_sas_token_lifetime_secs() on each `device_state->authentication_handle` passing `value`**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_092: [**If authentication_set_sas_token_lifetime_secs() fails, amqptransport_core_do_work() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_093: [**If `name` does not match any suported option, amqptransport_core_do_work() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_09_094: [**If no failures occurr, amqptransport_core_do_work() shall return 0**]**
