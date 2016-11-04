// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>

#include "iothub_client.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "iothubtransportmqtt.h"

#ifdef MBED_BUILD_TIMESTAMP
#include "certs.h"
#endif // MBED_BUILD_TIMESTAMP

static const char* connectionString = "HostName=DeviceTwin-jenkins-hub.azure-devices.net;DeviceId=000000anpo;SharedAccessKey=a/6Gy6T20fZ/5dWJc6dxrLj74aGOWhLsKhfrM71AQMo=";

#define CALL_REPORTED_PROPERTIES

static const char* reportedProperties = "{\n    \"lastOilChangeDate\": \"2224\"\n}";

void reportedPropertiesCallback(int status_code, void* userContextCallback)
{
    (void)(status_code, userContextCallback);
    //printf("reported properties say: %d %p\n", status_code, userContextCallback);
}

static void desiredPropertiesCallback(DEVICE_TWIN_UPDATE_STATE update_state, const unsigned char* payLoad, size_t size, void* userContextCallback)
{

    (void)update_state;
    //printf("received DESIRED PROPERTIES:\n");
    //printf("%*.*s", (int)size, (int)size, (const char*)payLoad);

    //printf("\n");

#ifdef CALL_REPORTED_PROPERTIES
    IOTHUB_CLIENT_HANDLE h = userContextCallback;
    if (IoTHubClient_SendReportedState(h, (const unsigned char*)reportedProperties, strlen(reportedProperties), reportedPropertiesCallback, NULL) != IOTHUB_CLIENT_OK)
    {
        //printf("not able to send reported properties");
    }
    else
    {
        //printf("totally able to send reported state\n");
    }
    
#endif

}

void iothub_client_sample_devicetwin_run(void)
{
    IOTHUB_CLIENT_HANDLE iotHubClientHandle;

    iotHubClientHandle = IoTHubClient_CreateFromConnectionString(connectionString, MQTT_Protocol);
    if (iotHubClientHandle == NULL)
    {
        printf("failure in IoTHubClient_CreateFromConnectionString\n");
    }
    else
    {
        if (IoTHubClient_SetDeviceTwinCallback(iotHubClientHandle, desiredPropertiesCallback, iotHubClientHandle) != IOTHUB_CLIENT_OK)
        {
            printf("failure in IoTHubClient_SetDeviceTwinCallback");
        }
        else
        {
            printf("press any key to continue\n");
            getchar();
        }
        IoTHubClient_Destroy(iotHubClientHandle);
    }
}

int main(void)
{
    (void)platform_init();
    iothub_client_sample_devicetwin_run();
    (void)platform_deinit();
    return 0;
}
