# iothubtransportamqp_core_common Requirements

## Overview

This is a library that contains common functions used by the implementations of the iothubtransportamqp_core modules.

## Dependencies

This module will depend on the following modules:

azure-c-shared-utilities


## Exposed API

```c
extern int get_seconds_since_epoch(double* seconds);
```

## get_seconds_since_epoch

```c
int get_seconds_since_epoch(double* seconds);
```

**SRS_IOTHUBTRANSPORTAMQP_CORE_COMMON_09_001: [**If parameter `seconds`, get_seconds_since_epoch() shall fail and return NULL**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_COMMON_09_002: [**The current time shall be obtained with get_time((time_t)NULL)**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_COMMON_09_003: [**If get_time() fails, get_seconds_since_epoch() shall fail and return __NULL__**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_COMMON_09_004: [**The number of seconds since epoch shall be obtained with get_difftime()**]**
**SRS_IOTHUBTRANSPORTAMQP_CORE_COMMON_09_005: [**If no failures occurr, get_seconds_since_epoch() shall return the result of get_difftime()**]**


