# iothubtransportamqp_cbs_auth Requirements

## Overview

This module provides APIs for authenticating devices through Azure IoT Hub CBS.  
   
   
## Exposed API

```c
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
```


### authentication_create

```c
AUTHENTICATION_HANDLE authentication_create(const AUTHENTICATION_CONFIG* config)
```

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_001: [**If parameter `config` or `config->device_id` are NULL, authentication_create() shall fail and return NULL.**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_002: [**If device keys and SAS token are NULL, authentication_create() shall fail and return NULL.**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_003: [**If device keys and SAS token are both provided, authentication_create() shall fail and return NULL.**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_004: [**If `config->iothub_host_fqdn` is NULL, authentication_create() shall fail and return NULL.**]**

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_005: [**authentication_create() shall allocate memory for a new authenticate state structure AUTHENTICATION_STATE.**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_006: [**If malloc() fails, authentication_create() shall fail and return NULL.**]**

Note: the AUTHENTICATION_STATE instance shall be referred to as `state` throughout the remaining requirements.

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_007: [**authentication_create() shall save a copy of `config->deviceId` into the AUTHENTICATION_STATE instance.**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_008: [**If STRING_construct() fails to copy `config->deviceId`, authentication_create() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_009: [**If `device_config->device_sas_token` is not NULL, authentication_create() shall save a copy into the AUTHENTICATION_STATE instance.**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_010: [**If STRING_construct() fails to copy `device_config->device_sas_token`, authentication_create() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_011: [**If provided, authentication_create() shall save a copy of `config->device_primary_key` into the AUTHENTICATION_STATE instance.**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_012: [**If STRING_construct() fails to copy `config->device_primary_key`, authentication_create() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_013: [**If provided, authentication_create() shall save a copy of `config->device_secondary_key` into the AUTHENTICATION_STATE instance.**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_014: [**If STRING_construct() fails to copy `config->device_secondary_key`, authentication_create() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_015: [**If provided, authentication_create() shall save a copy of `iothub_host_fqdn` into the AUTHENTICATION_STATE instance.**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_016: [**If STRING_clone() fails to copy `iothub_host_fqdn`, authentication_create() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_017: [**authentication_create() shall save `on_error_callback` and `on_error_context` into `state->on_error_callback` and `state->on_error_context`.**]**

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_018: [**If any failure occurs, authentication_create() shall free any memory it allocated previously, including the AUTHENTICATION_STATE**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_019: [**authentication_create() shall set `state->cbs_request_timeout_secs` with the default value of UINT32_MAX**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_020: [**authentication_create() shall set `state->sas_token_lifetime_secs` with the default value of one hour**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_021: [**authentication_create() shall set `state->sas_token_refresh_time_secs` with the default value of 30 minutes**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_022: [**If no failure occurs, authentication_create() shall return a reference to the AUTHENTICATION_STATE handle**]**


### authentication_start

```c
int authentication_start(AUTHENTICATION_HANDLE authentication_handle, const CBS_HANDLE cbs_handle, ON_AUTHENTICATION_START_COMPLETED on_start_completed, const void* context)
```

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_023: [**If authentication_handle is NULL, authentication_start() shall fail and return __LINE__ as error code**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_024: [**If authenticate state has been started already, authentication_start() shall fail and return __LINE__ as error code**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_025: [**If `cbs_handle` is NULL, authentication_start() shall fail and return __LINE__ as error code**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_026: [**authentication_start() shall save `cbs_handle` on state->cbs_handle**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_027: [**authentication_start() shall store references to `on_start_completed` and `context` on state->on_start_completed_callback and state->on_start_completed_context**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_028: [**If no failures occurr, authentication_start() shall mark the `state` as started and return 0**]**


### authentication_stop

```c
int authentication_stop(AUTHENTICATION_HANDLE authentication_handle, ON_AUTHENTICATION_STOP_COMPLETED on_stop_completed, const void* context)
```

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_029: [**If `authentication_handle` is NULL, authentication_stop() shall fail and return __LINE__ as error code**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_030: [**If authenticate state is not started, authentication_stop() shall fail and return __LINE__ as error code**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_031: [**authentication_stop() shall store references to `on_stop_completed` and `context` on state->on_stop_completed_callback and state->on_stop_completed_context**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_032: [**authentication_stop() shall mark the `state` as stopped and return success code 0**]**


### authentication_do_work

```c
void authentication_do_work(AUTHENTICATION_HANDLE authentication_handle)
```

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_033: [**If authentication_handle is NULL, authentication_do_work() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_034: [**If the `state` has not been started, authentication_do_work() shall fail and return**]**


##### SAS token authentication

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_035: [**If `state->device_sas_token` is provided, authentication_do_work() shall put it to CBS**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_036: [**A STRING_HANDLE, referred to as `devices_path`, shall be created from the following parts: iothub_host_fqdn + "/devices/" + device_id**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_037: [**If `devices_path` failed to be created, authentication_do_work() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_038: [**The SAS token provided shall be sent to CBS using cbs_put_token(), using `servicebus.windows.net:sastoken` as token type and `devices_path` as audience**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_039: [**If cbs_put_token() succeeds, authentication_do_work() shall internally mark the state authentication as in progress**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_040: [**If cbs_put_token() succeeds, `current_sas_token_put_time` with the current time**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_041: [**If cbs_put_token() fails, authentication_do_work() shall fail and return**]**

These requirements apply to authentication_do_work() before it returns:
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_042: [**If authentication_do_work() failed, `state->on_start_completed_callback` shall be invoke if provided, with result AUTHENTICATION_START_RESULT_ERROR**]**

These requirements apply to the callback of cbs_put_token():
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_043: [**If the cbs_put_token() callback result is CBS_OPERATION_RESULT_OK the state shall be marked as authenticated**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_044: [**If the cbs_put_token() callback result is CBS_OPERATION_RESULT_OK, `state->on_start_completed_callback` shall be invoke if provided, with result AUTHENTICATION_START_RESULT_SUCCESS**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_045: [**If the cbs_put_token() callback result is not CBS_OPERATION_RESULT_OK the state shall be marked as not started**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_046: [**If the cbs_put_token() callback result is not CBS_OPERATION_RESULT_OK, `state->on_start_completed_callback` shall be invoke if provided, with result AUTHENTICATION_START_RESULT_ERROR**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_047: [**After `state->on_start_completed_callback` is invoked, it shall be set to NULL as well as `state->on_start_completed_context`**]**


##### Device Key authentication

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_048: [**authentication_do_work() shall create a SAS token using `state->device_primary_key` and put it to CBS**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_049: [**If putting a SAS token created using `state->device_primary_key` fails and `state->device_secondary_key` is not provided, authentication_do_work() shall fail**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_050: [**If putting a SAS token created using `state->device_primary_key` fails, authentication_do_work() shall create a SAS token using `state->device_secondary_key` if provided and put it to CBS**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_051: [**If putting a SAS token created using `state->device_secondary_key` fails, authentication_do_work() shall fail**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_052: [**If putting a SAS token succeeds, authentication_do_work() shall internally mark the state authentication as in progress**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_053: [**If authentication_do_work() fails, `state->on_start_completed_callback` shall be invoke if provided, with result AUTHENTICATION_START_RESULT_ERROR**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_054: [**After `state->on_start_completed_callback` is invoked, it shall be set to NULL as well as `state->on_start_completed_context`**]**

The requirements below apply to the creation of the SAS token and putting it to CBS:
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_055: [**The SAS token expiration time shall be calculated adding `sas_token_lifetime` to the current number of seconds since epoch time UTC**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_056: [**A STRING_HANDLE, referred to as `devices_path`, shall be created from the following parts: iothub_host_fqdn + "/devices/" + device_id**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_057: [**If `devices_path` failed to be created, authentication_do_work() shall fail and return an error code**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_058: [**The SAS token shall be created using SASToken_Create(), passing the selected device key, device_path, sasTokenKeyName and expiration time as arguments**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_059: [**If SASToken_Create() fails, authentication_do_work() shall fail**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_060: [**The SAS token shall be sent to CBS using cbs_put_token(), using `servicebus.windows.net:sastoken` as token type and `devices_path` as audience**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_061: [**If cbs_put_token() succeeds, authentication_do_work() shall set `current_sas_token_put_time` with the current time**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_062: [**authentication_do_work() shall free the memory allocated for the new SAS token using STRING_delete()**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_063: [**If cbs_put_token() fails, authentication_do_work() shall fail**]**


#### Authentication timeout

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_064: [**If the device authentication is in progress, authentication_do_work() shall check for timeout against `cbs_request_timeout`**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_065: [**If the authentication has timed out, authentication_do_work() shall invoke `state->on_start_completed_callback`, if provided, with status AUTHENTICATION_START_RESULT_TIMEOUT**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_066: [**If the authentication has timed out, authentication_do_work() shall set `state->on_start_completed_callback` and `state->on_start_completed_context` to NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_067: [**If the authentication has timed out, authentication_do_work() shall mark the state as not started**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_068: [**authentication_do_work() shall return**]**


#### De-authentication

These are the actions triggered on authentication_do_work() due to the previous call of authentication_stop().

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_069: [**authentication_do_work() shall de-authenticate the device only if the state authentication is in progress or authenticated, and authentication_stop() has been invoked**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_070: [**A STRING_HANDLE, referred to as `devices_path`, shall be created from the following parts: iothub_host_fqdn + "/devices/" + device_id**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_071: [**If `devices_path` failed to be created, authentication_do_work() shall fail**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_072: [**cbs_delete_token() shall be invoked, using `servicebus.windows.net:sastoken` as token type and `devices_path` as audience**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_073: [**If cbs_delete_token() succeeds, authentication_do_work() shall internally mark the state de-authentication as in progress**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_074: [**If cbs_put_token() succeeds, `current_sas_token_put_time` shall be set to 0**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_075: [**If cbs_put_token() fails, authentication_do_work() shall fail**]**

The following requirements apply before authentication_do_work() returns:
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_076: [**If authentication_do_work() fails, `state->on_start_completed_callback` shall be invoke if provided, with result AUTHENTICATION_START_RESULT_ERROR**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_077: [**After `state->on_start_completed_callback` is invoked, it shall be set to NULL as well as `state->on_start_completed_context`**]**

These requirements apply to the callback of cbs_delete_token():
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_078: [**If the cbs_delete_token() callback result is CBS_OPERATION_RESULT_OK, `state->on_start_completed_callback` shall be invoke if provided, with result AUTHENTICATION_START_RESULT_SUCCESS**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_079: [**If the cbs_delete_token() callback result is not CBS_OPERATION_RESULT_OK, `state->on_start_completed_callback` shall be invoke if provided, with result AUTHENTICATION_START_RESULT_ERROR**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_080: [**After `state->on_start_completed_callback` is invoked, it shall be set to NULL as well as `state->on_start_completed_context`**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_081: [**The authentication state shall be marked not started**]**


#### De-authentication timeout

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_082: [**If the device de-authentication is in progress, authentication_do_work() shall check for timeout against `cbs_request_timeout`**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_083: [**If the de-authentication has timed out, authentication_do_work() shall invoke `state->on_stop_completed_callback`, if provided, with status AUTHENTICATION_STOP_RESULT_TIMEOUT**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_084: [**If the de-authentication has timed out, authentication_do_work() shall set `state->on_stop_completed_callback` and `state->on_stop_completed_context` to NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_085: [**If the de-authentication has timed out, authentication_do_work() shall mark the state as not started**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_086: [**authentication_do_work() shall return**]**


#### SAS token refresh

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_087: [**If device is authenticated against CBS using device keys, authentication_do_work() shall check if it needs to be refreshed against `state->sas_token_refresh_time_secs`**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_088: [**If SAS token does not need to be refreshed, authentication_do_work() shall return**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_089: [**If SAS token needs to be refreshed, authentication_do_work() shall create a SAS token using `state->device_primary_key` and put it to CBS**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_090: [**If putting a SAS token created using `state->device_primary_key` fails and `state->device_secondary_key` is not provided, authentication_do_work() shall fail**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_091: [**If putting a SAS token created using `state->device_primary_key` fails, authentication_do_work() shall create a SAS token using `state->device_secondary_key` if provided and put it to CBS**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_092: [**If putting a SAS token created using `state->device_secondary_key` fails, authentication_do_work() shall fail**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_093: [**If putting a SAS token succeeds, authentication_do_work() shall internally mark the state authentication as refreshing**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_094: [**If authentication_do_work() fails, `state->on_error_callback` shall be invoke if provided, with result AUTHENTICATION_ERROR_SAS_REFRESH_FAILED**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_095: [**If authentication_do_work() fails, the state shall be marked as not started**]**

The requirements below apply to the creation of the SAS token and putting it to CBS:
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_096: [**The SAS token expiration time shall be calculated adding `sas_token_lifetime` to the current number of seconds since epoch time UTC**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_097: [**A STRING_HANDLE, referred to as `devices_path`, shall be created from the following parts: iothub_host_fqdn + "/devices/" + device_id**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_098: [**If `devices_path` failed to be created, authentication_do_work() shall fail and return an error code**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_099: [**The SAS token shall be created using SASToken_Create(), passing the selected device key, device_path, sasTokenKeyName and expiration time as arguments**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_100: [**If SASToken_Create() fails, authentication_do_work() shall fail**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_101: [**The SAS token shall be sent to CBS using cbs_put_token(), using `servicebus.windows.net:sastoken` as token type and `devices_path` as audience**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_102: [**If cbs_put_token() succeeds, authentication_do_work() shall set `current_sas_token_put_time` with the current time**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_103: [**authentication_do_work() shall free the memory allocated for the new SAS token using STRING_delete()**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_104: [**If cbs_put_token() fails, authentication_do_work() shall fail**]**


#### SAS-token refresh timeout

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_105: [**If the SAS-token refresh is in progress, authentication_do_work() shall check for timeout against `cbs_request_timeout`**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_106: [**If the SAS token refresh has timed out, `state->on_error_callback` shall be invoke if provided, with result AUTHENTICATION_ERROR_SAS_REFRESH_FAILED**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_107: [**If the SAS token refresh has timed out, authentication_do_work() shall mark the state as not started**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_108: [**If the SAS token refresh has not timed out, authentication_do_work() shall return**]**


### authentication_set_cbs_request_timeout_secs

```c
int authentication_set_cbs_request_timeout_secs(AUTHENTICATION_HANDLE authentication_handle, uint32 value)
```

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_109: [**If `authentication_handle` is NULL, authentication_set_cbs_request_timeout_secs() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_110: [**`value` shall be saved on `state->cbs_request_timeout_secs`**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_111: [**authentication_set_cbs_request_timeout_secs() shall return 0**]**


### authentication_set_cbs_request_timeout_secs

```c
int authentication_set_sas_token_refresh_time_secs(AUTHENTICATION_HANDLE authentication_handle, uint32 value)
```

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_112: [**If `authentication_handle` is NULL, authentication_set_sas_token_refresh_time_secs() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_113: [**`value` shall be saved on `state->sas_token_refresh_time_secs`**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_114: [**authentication_set_sas_token_refresh_time_secs() shall return 0**]**


### authentication_set_sas_token_lifetime_secs

```c
int authentication_set_sas_token_lifetime_secs(AUTHENTICATION_HANDLE authentication_handle, uint32 value)
```

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_115: [**If `authentication_handle` is NULL, authentication_set_sas_token_lifetime_secs() shall fail and return __LINE__**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_116: [**`value` shall be saved on `state->sas_token_lifetime_secs`**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_117: [**authentication_set_sas_token_lifetime_secs() shall return 0**]**


### authentication_destroy

```c
void authentication_destroy(AUTHENTICATION_HANDLE authentication_handle)
```

**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_118: [**If authentication_handle is NULL, authentication_destroy() shall fail and return**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_119: [**If the `state` is authenticated, the SAS token shall be deleted using cbs_delete_token()**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_120: [**authentication_destroy() shall destroy `device_id` saved on the state using STRING_delete()**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_121: [**authentication_destroy() shall destroy `sas_token` saved on the state using STRING_delete()**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_122: [**authentication_destroy() shall destroy `device_primary_key` saved on the state using STRING_delete()**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_123: [**authentication_destroy() shall destroy `device_secondary_key` saved on the state using STRING_delete()**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_124: [**authentication_destroy() shall destroy `iothub_host_fqdn` saved on the state using STRING_delete()**]**
**SRS_IOTHUBTRANSPORTAMQP_AUTH_09_125: [**authentication_destroy() shall free the authentication state**]**

