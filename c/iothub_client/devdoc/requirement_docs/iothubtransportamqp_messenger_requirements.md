# iothubtransportamqp_messenger Requirements


## Overview

This module provides an abstraction for the IoTHubTransportAMQP to send events and receive messages.  
   
   
## Exposed API

```c
	typedef enum MESSENGER_ERROR_TAG
	{
		MESSENGER_ERROR_CANNOT_CREATE_RECEIVER,
		MESSENGER_ERROR_RECEIVER_CLOSED_UNEXPECTEDLY
		MESSENGER_ERROR_SENDER_CLOSED_UNEXPECTEDLY,
	} MESSENGER_ERROR;

	typedef enum MESSENGER_SEND_RESULT_TAG
	{
		MESSENGER_SEND_RESULT_SUCCESS,
		MESSENGER_SEND_RESULT_ERROR
	} MESSENGER_SEND_RESULT;

	typedef void (*ON_MESSENGER_ERROR_CALLBACK)(MESSENGER_ERROR code, void* context);
	typedef void (*ON_MESSAGE_SEND_COMPLETE)(MESSENGER_SEND_RESULT send_result, void* context);
	typedef void(*ON_MESSAGE_RECEIVED)(IOTHUB_MESSAGE_HANDLE message, void* context);

	typedef struct MESSENGER_CONFIG_TAG
	{
		char* device_id;
		char* iothub_host_fqdn;
		PDLIST_ENTRY wait_to_send_list;
		ON_MESSAGE_SEND_COMPLETE on_event_send_completed_callback;
		void* on_event_send_completed_context;
		ON_MESSENGER_ERROR_CALLBACK on_error_callback;
		void* on_error_context;
	} MESSENGER_CONFIG;

	typedef struct MESSENGER_STATE* MESSENGER_HANDLE;

	extern MESSENGER_HANDLE messenger_create(const MESSENGER_CONFIG* messenger_config);
	extern int messenger_subscribe_for_messages(MESSENGER_HANDLE messenger_handle, ON_MESSAGE_RECEIVED on_message_received_callback, void* context);
	extern int messenger_unsubscribe_for_messages(MESSENGER_HANDLE messenger_handle);
	extern int messenger_start(MESSENGER_HANDLE messenger_handle, SESSION_HANDLE session_handle); 
	extern int messenger_stop(MESSENGER_HANDLE messenger_handle);
	extern void messenger_do_work(MESSENGER_HANDLE messenger_handle);
	extern void messenger_destroy(MESSENGER_HANDLE messenger_handle);
```


## messenger_create

```c
	extern MESSENGER_HANDLE messenger_create(const MESSENGER_CONFIG* messenger_config);
```

**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_001: [**If parameter `messenger_config` is NULL, messenger_create() shall return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_002: [**If `messenger_config->device_id` is NULL, messenger_create() shall return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_003: [**If `messenger_config->iothub_host_fqdn` is NULL, messenger_create() shall return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_004: [**If `messenger_config->wait_to_send_list` is NULL, messenger_create() shall return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_005: [**If `messenger_config->on_event_send_completed_callback` is NULL, messenger_create() shall return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_006: [**messenger_create() shall allocate memory for the messenger state**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_007: [**If malloc() fails, messenger_create() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_008: [**messenger_create() shall save a copy of `messenger_config->device_id` into the messenger state**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_009: [**If STRING_construct() fails, messenger_create() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_010: [**messenger_create() shall save a copy of `messenger_config->iothub_host_fqdn` into the messenger state**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_011: [**If STRING_construct() fails, messenger_create() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_012: [**The pointer `messenger_config->wait_to_send_list` shall be saved into `state->wait_to_send_list`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_013: [**The pointer `messenger_config->on_event_send_completed_callback` shall be saved into `state->on_event_send_completed_callback`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_014: [**The pointer `messenger_config->on_event_send_completed_context` shall be saved into `state->on_event_send_completed_context`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_015: [**If no failures occurr, messenger_create() shall return a handle to the messenger state**]**


## messenger_subscribe_for_messages

```c
	extern int messenger_subscribe_for_messages(MESSENGER_HANDLE messenger_handle, ON_MESSAGE_RECEIVED on_message_received_callback, void* context);
```

**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_016: [**If `messenger_handle` is NULL, messenger_subscribe_for_messages() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_017: [**If `state->receive_messages` is already true, messenger_subscribe_for_messages() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_018: [**If `on_message_received_callback` is NULL, messenger_subscribe_for_messages() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_019: [**`on_message_received_callback` shall be saved on `state->on_message_received_callback`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_020: [**`context` shall be saved on `state->on_message_received_context`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_021: [**messenger_subscribe_for_messages() shall set `state->receive_messages` to true**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_022: [**If no failures occurr, messenger_subscribe_for_messages() shall return 0**]**


## messenger_unsubscribe_for_messages

```c
	extern int messenger_unsubscribe_for_messages(MESSENGER_HANDLE messenger_handle);
```

**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_023: [**If `messenger_handle` is NULL, messenger_unsubscribe_for_messages() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_024: [**If `state->receive_messages` is already false, messenger_unsubscribe_for_messages() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_025: [**messenger_unsubscribe_for_messages() shall set `state->receive_messages` to false**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_026: [**messenger_unsubscribe_for_messages() shall set `state->on_message_received_callback` to NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_027: [**messenger_unsubscribe_for_messages() shall set `state->on_message_received_context` to NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_028: [**If no failures occurr, messenger_unsubscribe_for_messages() shall return 0**]**


## messenger_start

```c
	extern int messenger_start(MESSENGER_HANDLE messenger_handle, SESSION_HANDLE session_handle); 
```

**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_029: [**If `messenger_handle` is NULL, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_030: [**If `session_handle` is NULL, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_031: [**If `state` is already started, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_032: [**`session_handle` shall be saved on `state->session_handle`, and the `state` marked as started**]**

**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_033: [**A parameter, named `devices_path`, shall be created concatenating `state->iothub_host_fqdn`, "/devices/" and `state->device_id`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_034: [**If `devices_path` fails to be created, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_035: [**A parameter, named `event_send_address`, shall be created concatenating "amqps://", `devices_path` and "/messages/events"**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_036: [**If `event_send_address` fails to be created, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_037: [**A `source` parameter shall be created with messaging_create_source() using an unique string label per AMQP session**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_038: [**If `source` fails to be created, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_039: [**A `target` parameter shall be created with messaging_create_target() using `event_send_address`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_040: [**If `target` fails to be created, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_041: [**A `link_name` parameter shall be created using an unique string label per AMQP session**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_042: [**If `link_name` fails to be created, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_043: [**`state->receiver_link` shall be set using link_create(), passing `state->session_handle`, `link_name`, "role_receiver", `source` and `target` as parameters**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_044: [**If link_create() fails, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_045: [**`state->receiver_link` settle mode shall be set to "receiver_settle_mode_first" using link_set_rcv_settle_mode(), **]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_046: [**If link_set_rcv_settle_mode() fails, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_047: [**`state->receiver_link` maximum message size shall be set to 65536 using link_set_max_message_size()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_048: [**If link_set_max_message_size() fails, it shall be logged and ignored.**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_049: [**`state->receiver_link` should have a property "com.microsoft:client-version" set as `CLIENT_DEVICE_TYPE_PREFIX/IOTHUB_SDK_VERSION`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_050: [**If `state->receiver_link` fails to have the client type and version set on its properties, the failure shall be ignored**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_051: [**`state->message_receiver` shall be created using messagereceiver_create(), passing the `state->receiver_link` and a state change callback**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_052: [**If messagereceiver_create() fails, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_053: [**`state->message_receiver` shall be opened using messagereceiver_open(), passing a callback to receive C2D messages**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_054: [**If messagereceiver_open() fails, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_055: [**Before returning, messenger_start() shall return release all the temporary memory it has allocated**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_056: [**If no failures occurr, messenger_start() shall return 0**]**


## messenger_stop

```c
	extern int messenger_stop(MESSENGER_HANDLE messenger_handle);
```

**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_057: [**If `messenger_handle` is NULL, messenger_stop() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_058: [**If `state` is already stopped, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_059: [**`state->message_sender` shall be stopped using messagesender_stop()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_060: [**`state->message_sender` shall be destroyed using messagesender_destroy()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_061: [**`state->message_receiver` shall be stopped using messagereceiver_stop()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_062: [**`state->message_receiver` shall be destroyed using messagereceiver_destroy()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_063: [**`state->sender_link` shall be destroyed using link_destroy()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_064: [**`state->receiver_link` shall be destroyed using link_destroy()**]**


## messenger_do_work

```c
	extern void messenger_do_work(MESSENGER_HANDLE messenger_handle);
```

**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_065: [**If `messenger_handle` is NULL, messenger_do_work() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_066: [**If `state` is not started, messenger_do_work() shall fail and return**]**


### Create a message receiver

**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_067: [**If `state->receive_messages` is true and `state->message_receiver` is NULL, a message_receiver shall be created**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_068: [**A parameter, named `devices_path`, shall be created concatenating `state->iothub_host_fqdn`, "/devices/" and `state->device_id`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_069: [**If `devices_path` fails to be created, messenger_do_work() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_070: [**A parameter, named `message_receive_address`, shall be created concatenating "amqps://", `devices_path` and "/messages/devicebound"**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_071: [**If `message_receive_address` fails to be created, messenger_do_work() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_072: [**A `target` parameter shall be created with messaging_create_target() using an unique string label per AMQP session**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_073: [**If `target` fails to be created, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_074: [**A `source` parameter shall be created with messaging_create_source() using `message_receive_address`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_075: [**If `source` fails to be created, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_076: [**A `link_name` parameter shall be created using an unique string label per AMQP session**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_077: [**If `link_name` fails to be created, messenger_start() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_078: [**`state->receiver_link` shall be set using link_create(), passing `state->session_handle`, `link_name`, "role_receiver", `source` and `target` as parameters**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_079: [**If link_create() fails, messenger_do_work() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_080: [**`state->receiver_link` settle mode shall be set to "receiver_settle_mode_first" using link_set_rcv_settle_mode(), **]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_081: [**If link_set_rcv_settle_mode() fails, messenger_do_work() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_082: [**`state->receiver_link` maximum message size shall be set to 65536 using link_set_max_message_size()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_083: [**If link_set_max_message_size() fails, it shall be logged and ignored.**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_084: [**`state->receiver_link` should have a property "com.microsoft:client-version" set as `CLIENT_DEVICE_TYPE_PREFIX/IOTHUB_SDK_VERSION`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_085: [**If `state->receiver_link` fails to have the client type and version set on its properties, the failure shall be ignored**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_086: [**`state->message_receiver` shall be created using messagereceiver_create(), passing the `state->receiver_link` and a state change callback**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_087: [**If messagereceiver_create() fails, messenger_do_work() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_088: [**`state->message_receiver` shall be opened using messagereceiver_open(), passing a callback to receive C2D messages**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_089: [**If messagereceiver_open() fails, messenger_do_work() shall fail and return**]**

The following requirements shall apply before messenger_do_work() returns:
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_090: [**If messenger_do_work() fails, it shall invoke `state->on_error_callback`, if provided, with error code MESSENGER_ERROR_CANNOT_CREATE_RECEIVER**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_091: [**Before messenger_do_work() returns, it shall destroy all the temporary memory it allocated**]**


### Destroy the message receiver

**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_092: [**If `state->receive_messages` is false and `state->message_receiver` is not NULL, it shall be destroyed**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_093: [**`state->message_receiver` shall be closed using messagereceiver_close()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_094: [**If messagereceiver_close() fails, it shall be logged and ignored**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_095: [**`state->message_receiver` shall be destroyed using messagereceiver_destroy()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_096: [**`state->message_receiver` shall be set to NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_097: [**`state->receiver_link` shall be destroyed using link_destroy()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_098: [**`state->receiver_link` shall be set to NULL**]**


### Send pending events

**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_099: [**If `state->message_sender` is not in OPEN state, messenger_do_work() shall return**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_100: [**messenger_do_work() shall move each event to be sent from `state->wait_to_send_list` to `state->in_progress_list`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_101: [**A MESSAGE_HANDLE shall be obtained out of the event's IOTHUB_MESSAGE_HANDLE instance by using message_create_from_iothub_message()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_102: [**If message_create_from_iothub_message() fails, `state->on_event_send_completed_callback` shall be invoked with result MESSENGER_SEND_RESULT_ERROR**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_103: [**If message_create_from_iothub_message() fails, messender_do_work() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_104: [**The MESSAGE_HANDLE shall be submitted for sending using messagesender_send(), passing a callback to track when the event has been sent**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_105: [**If messagesender_send() fails, the event shall be rolled back from `state->in_progress_list` to `state->wait_to_send_list`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_106: [**The MESSAGE_HANDLE shall be destroyed using message_destroy().**]**

When messagesender_send() calls back,
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_107: [**If no failure occurs, `state->on_event_send_completed_callback` shall be invoked with result MESSENGER_SEND_RESULT_SUCCESS**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_108: [**If a failure occurred, `state->on_event_send_completed_callback` shall be invoked with result MESSENGER_SEND_RESULT_ERROR**]**



## messenger_destroy

```c
	extern void messenger_destroy(MESSENGER_HANDLE messenger_handle);
```

**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_109: [**If `messenger_handle` is NULL, messenger_destroy() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_110: [**If the `state` is started, messenger_destroy() shall invoke messenger_stop(), ignoring its result**]**

Note: when events are moved from wait_to_send_list to in_progress_list, they are moved from beginning (oldest element) to end (newest).
Due to the logic used by messenger_do_work(), all elements of in_progress_list are always older than any current element on in_progress_list.
In that case, when they are rolled back, they need to go to the beginning of the wait_to_send_list. 
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_111: [**All elements of `state->in_progress_list` shall be moved to the beginning of `state->wait_to_send_list`**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_112: [**`state->iothub_host_fqdn` shall be destroyed using STRING_delete()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_113: [**`state->device_id` shall be destroyed using STRING_delete()**]**
**SRS_IOTHUBTRANSPORTAMQP_MESSENGER_09_114: [**messenger_destroy() shall destroy the messenger state with free()**]**
