/*
Microsoft Azure IoT Device Libraries

Copyright (c) Microsoft Corporation
All rights reserved.

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the Software), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*BEING TO BE MODIFIED*/
#define MYACCOUNT "<<Enter your account name here>>"
#define MYKEY "<<Enter your key here>>"
/*END TO BE MODIFIED*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "httpapiex.h"
#include "strings.h"
#include "hmacsha256.h"
#include "base64.h"
#include "map.h"

/*only download 1MB at a time and then be close the conenction*/
#define MAX_BLOB_CHUNK_LENGHT (1024*1024)

#define X_MS_DATE "x-ms-date"
#define X_MS_VERSION "x-ms-version"
#define X_MS_VERSION_VALUE "2014-02-14"
#define X_MS_RANGE "x-ms-range"
#define COMP "comp"
#define LIST "list"
#define CONTAINER_NAME "<Container><Name>"
#define BLOB_NAME "<Blob><Name>"
#define RESTYPE "restype"
#define CONTAINER "container"

char *toXMSDATE(const struct tm *timeptr)
{
    static char result[50];

    static const char wday_name[7][4] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
    static const char mon_name[12][4] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    sprintf(result, "%.3s, %.2d %.3s %d %.2d:%.2d:%.2d GMT",
        wday_name[timeptr->tm_wday],
        timeptr->tm_mday,
        mon_name[timeptr->tm_mon],
        1900 + timeptr->tm_year,

        timeptr->tm_hour,
        timeptr->tm_min, timeptr->tm_sec
        );
    return result;
}

char *toXMSRANGE(size_t startRange)
{
    static char result[100];
    sprintf(result, "bytes=%d-%d", (int)startRange, (int)startRange+MAX_BLOB_CHUNK_LENGHT);
    return result;
}

/*returns 0 if the file doiesn't exist*/
size_t getFileSize(const char* fileName)
{
    size_t result;
    FILE* fin = fopen(fileName, "rb");
    if (fin == NULL)
    {
        result = 0;
    }
    else
    {
        if (fseek(fin, 0L, SEEK_END) != 0)
        {
            result = 0;
            printf("unable to fseek!!!\r\n");
            printf("XXXXXXX results will be garbled XXXXXXXXXXX\r\n");
        }
        else
        {
            result = ftell(fin);
        }
        fclose(fin);
    }
    return result;
}

void listBlobs(const char* containerName);

BUFFER_HANDLE getAzureStorageServiceBody(const char* relativePath, MAP_HANDLE queryParameters, bool useRange, size_t startRange, bool* readPastEnd)
{
    BUFFER_HANDLE result = NULL;
    HTTPAPIEX_HANDLE httpHandle = HTTPAPIEX_Create(MYACCOUNT ".blob.core.windows.net");
    if (httpHandle == NULL)
    {
        printf("error: unable to HTTPAPIEX_Create\r\n");
    }
    else
    {
        HTTP_HEADERS_HANDLE httpRequestHeaders = HTTPHeaders_Alloc();
        if (httpRequestHeaders == NULL)
        {
            printf("error: unable to HTTPHeaders_Alloc\r\n");
        }
        else
        {
            time_t theTime = time(NULL);
            if (theTime == (time_t)(-1))
            {
                printf("time failed\r\n");
            }
            else
            {
                struct tm* gmTime = gmtime(&theTime);
                if (gmTime == NULL)
                {
                    printf("gmtime failed\r\n");
                }
                else
                {
                    char* x_ms_date_value = toXMSDATE(gmTime);
                    char* x_ms_range_value = useRange ? toXMSRANGE(startRange) : NULL;
                    if (!(
                        (HTTPHeaders_ReplaceHeaderNameValuePair(httpRequestHeaders, X_MS_DATE, x_ms_date_value) == HTTP_HEADERS_OK) &&
                        (useRange ? (HTTPHeaders_ReplaceHeaderNameValuePair(httpRequestHeaders, X_MS_RANGE, x_ms_range_value) == HTTP_HEADERS_OK) : 1) &&
                        (HTTPHeaders_ReplaceHeaderNameValuePair(httpRequestHeaders, X_MS_VERSION, X_MS_VERSION_VALUE) == HTTP_HEADERS_OK)
                        ))
                    {
                        printf("HTTPHeaders_ReplaceHeaderNameValuePair failed\r\n");
                    }
                    else
                    {
#if 0
                        from https ://msdn.microsoft.com/en-us/library/azure/dd179428.aspx
                        VERB + "\n" +
                            Content - Encoding + "\n" +
                            Content - Language + "\n" +
                            Content - Length + "\n" +
                            Content - MD5 + "\n" +
                            Content - Type + "\n" +
                            Date + "\n" +
                            If - Modified - Since + "\n" +
                            If - Match + "\n" +
                            If - None - Match + "\n" +
                            If - Unmodified - Since + "\n" +
                            Range + "\n" +
                            CanonicalizedHeaders +
                            CanonicalizedResource
#endif

#if 0

                            1. Beginning with an empty string(""), append a forward slash(/ ), followed by the name of the account that owns the resource being accessed.
                            2. Append the resource''s encoded URI path, without any query parameters.
                            3. Append a new - line character(\n) after the resource name.
                            4. Retrieve all query parameters on the resource URI, including the comp parameter if it exists.
                            5. Convert all parameter names to lowercase.
                            6. Sort the query parameters lexicographically by parameter name, in ascending order.
                            7. URL - decode each query parameter name and value.
                            8. Append each query parameter name and value to the string in the following format, making sure to include the colon(:) between the name and the value :
                        parameter - name : parameter - value
                            9. If a query parameter has more than one value, sort all values lexicographically, then include them in a comma - separated list :
                        parameter - name : parameter - value - 1, parameter - value - 2, parameter - value - n
                            10. Append a new - line character(\n) after each name - value pair.

#endif
                            STRING_HANDLE stringToSign = STRING_new();
                        if (stringToSign == NULL)
                        {
                            printf("failed to STRING_new\r\n");
                        }
                        else
                        {
                            if (!(
                                (STRING_concat(stringToSign, "GET\n") == 0) && /*VERB +"\n" +*/
                                (STRING_concat(stringToSign, "\n") == 0) && /*Content-Encoding + "\n"*/ /*no Content-Encoding*/
                                (STRING_concat(stringToSign, "\n") == 0) && /*Content-Language + "\n"*/ /*no Content-Language*/
                                (STRING_concat(stringToSign, "0\n") == 0) && /*Content-Length + "\n"*/ /*no Content-Length - actually it is zero*/
                                (STRING_concat(stringToSign, "\n") == 0) && /*Content-MD5 + "\n"*/ /*no Content-MD5*/
                                (STRING_concat(stringToSign, "\n") == 0) && /*Content-Type + "\n" */ /*no Content-Type*/
                                (STRING_concat(stringToSign, "\n") == 0) && /*Date + "\n" */ /*no Date*/
                                (STRING_concat(stringToSign, "\n") == 0) && /*If-Modified-Since + "\n"*/ /*no If-Modified-Since */
                                (STRING_concat(stringToSign, "\n") == 0) && /*If-Match + "\n"*/ /*no If-Match*/
                                (STRING_concat(stringToSign, "\n") == 0) && /*If-None-Match + "\n"*/ /*no If-None-Match*/
                                (STRING_concat(stringToSign, "\n") == 0) && /*If-Unmodified-Since + "\n"*/ /*no If-Unmodified-Since*/
                                (STRING_concat(stringToSign, "\n") == 0) && /*Range + "\n"*/ /*no Range*/
                                (
                                /*canonicalized headers - for now no logic, order is hard coded*/
                                (STRING_concat(stringToSign, X_MS_DATE) == 0) && /*x-ms-date*/
                                (STRING_concat(stringToSign, ":") == 0) && /*:*/
                                (STRING_concat(stringToSign, x_ms_date_value) == 0) && /*:*/
                                (STRING_concat(stringToSign, "\n") == 0) && /*\n*/

                                
                                (useRange?
                                    (
                                        (STRING_concat(stringToSign, X_MS_RANGE) == 0) && /*x-ms-range*/
                                        (STRING_concat(stringToSign, ":") == 0) && /*:*/
                                        (STRING_concat(stringToSign, x_ms_range_value) == 0) && /*bytes=0-511*/
                                        (STRING_concat(stringToSign, "\n") == 0) /*\n*/
                                    ):
                                    1
                                ) &&
                                

                                (STRING_concat(stringToSign, X_MS_VERSION) == 0) && /*x-ms-version*/
                                (STRING_concat(stringToSign, ":") == 0) && /*:*/
                                (STRING_concat(stringToSign, X_MS_VERSION_VALUE) == 0) && /*2014-02-14*/
                                (STRING_concat(stringToSign, "\n") == 0) /*\n*/
                                ) &&
                                (
                                /*canonicalized resource*/
                                (STRING_concat(stringToSign, "/" MYACCOUNT) == 0) && /*1. Beginning with an empty string (""), append a forward slash (/), followed by the name of the account that owns the resource being accessed.*/
                                (STRING_concat(stringToSign, "/") == 0) && /*2. Append the resource's encoded URI path, without any query parameters.*/
                                (STRING_concat(stringToSign, relativePath) == 0) && /*2. Append the resource's encoded URI path, without any query parameters.*/
                                (STRING_concat(stringToSign, "\n") == 0) /*3. Append a new-line character (\n) after the resource name. */

                                )
                                ))
                            {
                                printf("unable to obtain the string to sign\r\n");
                            }
                            else
                            {
                                /*
                                4. Retrieve all query parameters on the resource URI, including the comp parameter if it exists.
                                5. Convert all parameter names to lowercase.
                                6. Sort the query parameters lexicographically by parameter name, in ascending order.
                                7. URL-decode each query parameter name and value.
                                8. Append each query parameter name and value to the string in the following format, making sure to include the colon (:) between the name and the value:
                                parameter-name:parameter-value
                                9. If a query parameter has more than one value, sort all values lexicographically, then include them in a comma-separated list:
                                parameter-name:parameter-value-1,parameter-value-2,parameter-value-n
                                10. Append a new-line character (\n) after each name-value pair. (they mean: except the last one)
                                */
                                const char*const* keys;
                                const char*const* values;
                                size_t nQueryParameters;

                                if (Map_GetInternals(queryParameters, &keys, &values, &nQueryParameters) != MAP_OK)
                                {
                                    printf("unable to Map_GetInternals\r\n");
                                }
                                else
                                {
                                    size_t i;
                                    for (i = 0; i < nQueryParameters; i++)
                                    {
                                        if (!(
                                            ((i>0) ? (STRING_concat(stringToSign, "\n") == 0) : 1) &&
                                            (STRING_concat(stringToSign, keys[i]) == 0) &&
                                            (STRING_concat(stringToSign, ":") == 0) &&
                                            (STRING_concat(stringToSign, values[i]) == 0)
                                            ))
                                        {
                                            printf("failed to STRING_concat\r\n");
                                            break;
                                        }
                                    }

                                    if (i < nQueryParameters)
                                    {
                                        /*do nothing, adding query parameters failed*/
                                    }
                                    else
                                    {
                                        BUFFER_HANDLE keyDecoded = Base64_Decoder(MYKEY);
                                        if (keyDecoded == NULL)
                                        {
                                            printf("unable to decode Base64_decode\r\n");
                                        }
                                        else
                                        {
                                            BUFFER_HANDLE hash = BUFFER_new();
                                            if (hash == NULL)
                                            {
                                                printf("unable to BUFFER_new\r\n");
                                            }
                                            else
                                            {
                                                //printf("going to HMAC the following: '%s'\n", STRING_c_str(stringToSign));
                                                if (HMACSHA256_ComputeHash(BUFFER_u_char(keyDecoded), BUFFER_length(keyDecoded), (const unsigned char*)STRING_c_str(stringToSign), STRING_length(stringToSign), hash) != HMACSHA256_OK)
                                                {
                                                    printf("cannot HMACSHA256_ComputeHash\r\n");
                                                }
                                                else
                                                {
                                                    STRING_HANDLE base64EncodedHash = Base64_Encode(hash);
                                                    if (base64EncodedHash == NULL)
                                                    {
                                                        printf("unable to Base64_Encode\r\n");
                                                    }
                                                    else
                                                    {
                                                        STRING_HANDLE authorizationValue = STRING_construct("SharedKey " MYACCOUNT ":");
                                                        if (authorizationValue == NULL)
                                                        {
                                                            printf("unableto STRING_construct\r\n");
                                                        }
                                                        else
                                                        {
                                                            if (STRING_concat_with_STRING(authorizationValue, base64EncodedHash) != 0)
                                                            {
                                                                printf("unable to STRING_concat_with_STRING\r\n");
                                                            }
                                                            else
                                                            {
                                                                //printf("HMACSHA256_ComputeHash success!!! %s \r\n", STING_c_str(authorizationValue));
                                                                if (
                                                                    HTTPHeaders_ReplaceHeaderNameValuePair(httpRequestHeaders, "authorization", STRING_c_str(authorizationValue)) != HTTP_HEADERS_OK
                                                                    )
                                                                {
                                                                    printf("unable to authorize headers");
                                                                }
                                                                else
                                                                {
                                                                    unsigned int httpStatus = 0;
                                                                    BUFFER_HANDLE responseBuffer = BUFFER_new();
                                                                    if (responseBuffer == NULL)
                                                                    {
                                                                        printf("unable to BUFFER_new\r\n");
                                                                    }
                                                                    else
                                                                    {
                                                                        STRING_HANDLE httpRelativePath = STRING_construct("/");
                                                                        if (httpRelativePath == NULL)
                                                                        {
                                                                            printf("failed to STRING_construct\r\n");
                                                                        }
                                                                        else
                                                                        {
                                                                            if (!(
                                                                                (STRING_concat(httpRelativePath, relativePath) == 0) &&
                                                                                (STRING_concat(httpRelativePath, "?") == 0)
                                                                                ))
                                                                            {
                                                                                printf("unable to STRING_concat\r\n");
                                                                            }
                                                                            else
                                                                            {
                                                                                for (i = 0; i < nQueryParameters; i++)
                                                                                {
                                                                                    if (!(
                                                                                        ((i>0) ? (STRING_concat(httpRelativePath, "&") == 0) : 1) &&
                                                                                        (STRING_concat(httpRelativePath, keys[i]) == 0) &&
                                                                                        (STRING_concat(httpRelativePath, "=") == 0) &&
                                                                                        (STRING_concat(httpRelativePath, values[i]) == 0)
                                                                                        ))
                                                                                    {
                                                                                        printf("unable to STRING_concat\r\n");
                                                                                        break;
                                                                                    }
                                                                                }

                                                                                if (i < nQueryParameters)
                                                                                {
                                                                                    /*something went wrong... exit*/
                                                                                }
                                                                                else
                                                                                {
                                                                                    if (HTTPAPIEX_ExecuteRequest(httpHandle, HTTPAPI_REQUEST_GET, STRING_c_str(httpRelativePath), httpRequestHeaders, NULL, &httpStatus, NULL, responseBuffer) != HTTPAPIEX_OK)
                                                                                    {
                                                                                        printf("failed to HTTPAPIEX_ExecuteRequest\r\n");
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        if (httpStatus >= 300)
                                                                                        {
                                                                                            /*https://msdn.microsoft.com/en-us/library/azure/ee691967.aspx*/
                                                                                            if (useRange && httpStatus == 416)
                                                                                            {
                                                                                                *readPastEnd = true;
                                                                                                /*print nothing, this is just reading past the end of the container*/
                                                                                            }
                                                                                            else
                                                                                            {
                                                                                                *readPastEnd = false;
                                                                                                printf("Http status unsuccesfull %d\r\n", httpStatus);
                                                                                            }
                                                                                        }
                                                                                        else
                                                                                        {
                                                                                            result = BUFFER_clone(responseBuffer);
                                                                                            if (result == NULL)
                                                                                            {
                                                                                                *readPastEnd = false;
                                                                                                printf("BUFFER_clone failed\r\n");
                                                                                            }
                                                                                            else
                                                                                            {
                                                                                                /*all is fine, buffer content is returned*/
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                        BUFFER_delete(responseBuffer);
                                                                    }
                                                                }
                                                            }
                                                            STRING_delete(authorizationValue);
                                                        }
                                                        STRING_delete(base64EncodedHash);
                                                    }
                                                }
                                                BUFFER_delete(hash);
                                            }
                                            BUFFER_delete(keyDecoded);
                                        }
                                    }
                                }
                            }
                            STRING_delete(stringToSign);
                        }
                    }
                }
            }
            HTTPHeaders_Free(httpRequestHeaders);
        }
        HTTPAPIEX_Destroy(httpHandle);
    }
    return result;
}

/*this does https://msdn.microsoft.com/en-us/library/azure/dd179440.aspx*/
void getBlob(const char* containerName, const char* blobName)
{
    /*if blob exists on disk, delete it*/
    (void)remove(blobName);

    MAP_HANDLE getBlobQueryParameters = Map_Create(NULL);
    printf("iterateContainers : THIS IS NOT PRODUCTION QUALITY CODE!!!! DO NOT USE!!!!\r\n");
    if (getBlobQueryParameters == NULL)
    {
        printf("unable to Map_Create\r\n");
    }
    else
    {
        if (!(
            (Map_AddOrUpdate(getBlobQueryParameters, "timeout", "100") == MAP_OK) /*let's have a timeout here*/
            ))
        {
            printf("failed Map_AddOrUpdate\r\n");
        }
        else
        {
            STRING_HANDLE temp = STRING_construct(containerName);
            if (temp == NULL)
            {
                printf("STRING_construct failed\r\n");
            }
            else
            {
                if (!(
                    (STRING_concat(temp, "/") == 0) &&
                    (STRING_concat(temp, blobName) == 0)
                    ))
                {
                    printf("STRING_concat failed\r\n");
                }
                else
                {
                    BUFFER_HANDLE thePartialBlob;
                    do
                    {
                        size_t fileSize = getFileSize(blobName);
                        bool wasReadPastEnd = false;
                        thePartialBlob = getAzureStorageServiceBody(STRING_c_str(temp), getBlobQueryParameters, true, fileSize, &wasReadPastEnd);
                        if (thePartialBlob == NULL)
                        {
                            if (wasReadPastEnd)
                            {
                                printf("SUCCESS: finished download %s\r\n", blobName);
                            }
                            else
                            {
                                printf("ERROR: fatal error downloading %s\r\n", blobName);
                            }
                        }
                        else
                        {
                            FILE* fout = fopen(blobName, "ab"); /*append binary*/
                            if (fout == NULL)
                            {
                                printf("unable to fopen\r\n");
                            }
                            else
                            {
                                if (fwrite(BUFFER_u_char(thePartialBlob), BUFFER_length(thePartialBlob), 1, fout) != 1)
                                {
                                    printf("cannot fwrite\r\n");
                                }
                                else
                                {
                                    //uncomment this to get some progress :)
                                    printf("partial blob (bytes from %d - %d) %s saved to disk!!!\r\n", (int) fileSize, (int)(fileSize + BUFFER_length(thePartialBlob)), blobName);
                                }
                                fclose(fout);
                            }
                            BUFFER_delete(thePartialBlob);
                        }
                    } while (thePartialBlob != NULL);
                }
                STRING_delete(temp);
            }
        }
        Map_Destroy(getBlobQueryParameters);
    }
}

/*this does https://msdn.microsoft.com/en-us/library/azure/dd135734.aspx*/
void listBlobs(const char* containerName)
{
    MAP_HANDLE listBlobsQueryParameters = Map_Create(NULL);
    printf("iterateContainers : THIS IS NOT PRODUCTION QUALITY CODE!!!! DO NOT USE!!!!\r\n");
    if (listBlobsQueryParameters == NULL)
    {
        printf("unable to Map_Create\r\n");
    }
    else
    {
        if (!(
            (Map_AddOrUpdate(listBlobsQueryParameters, "comp", "list") == MAP_OK) &&
            (Map_AddOrUpdate(listBlobsQueryParameters, "restype", "container") == MAP_OK)
            ))
        {
            printf("failed Map_AddOrUpdate\r\n");
        }
        else
        {
            bool dummy;
            BUFFER_HANDLE containers = getAzureStorageServiceBody(containerName, listBlobsQueryParameters, false, 0, &dummy);
            if (containers == NULL)
            {
                printf("unable to getAzureStorageServiceBody\r\n");
            }
            else
            {
                STRING_HANDLE responseString = STRING_construct_n((const char*)BUFFER_u_char(containers) + 3, BUFFER_length(containers) - 3); /*3 because unknown, maybe BOM... */
                if (responseString == NULL)
                {
                    printf("unable to STRING_construct_n\r\n");
                }
                else
                {
                    char blobName[1000];
                    const char* seekHereContainerName = STRING_c_str(responseString);
                    const char* whereIsStarts;
                    while ((whereIsStarts = strstr(seekHereContainerName, BLOB_NAME)) != NULL)
                    {
                        size_t pos = 0;
                        whereIsStarts += strlen(BLOB_NAME);
                        while (whereIsStarts[pos] != '<')
                        {
                            blobName[pos] = whereIsStarts[pos];
                            pos++;
                        }
                        blobName[pos] = '\0';
                        printf("DETECTED blob name = %s\r\n", blobName);

                        getBlob(containerName, blobName);

                        seekHereContainerName = whereIsStarts + pos;
                    }
                    STRING_delete(responseString);
                }

                BUFFER_delete(containers);
            }
        }
        Map_Destroy(listBlobsQueryParameters);
    }
}

/*this does https://msdn.microsoft.com/en-us/library/azure/dd179352.aspx*/
void listContainers(void)
{
    MAP_HANDLE listContainersQueryParameters = Map_Create(NULL);
    printf("iterateContainers : THIS IS NOT PRODUCTION QUALITY CODE!!!! DO NOT USE!!!!\r\n");
    if (listContainersQueryParameters == NULL)
    {
        printf("unable to Map_Create\r\n");
    }
    else
    {
        if (!(
            (Map_AddOrUpdate(listContainersQueryParameters, "comp", "list") == MAP_OK)
            ))
        {
            printf("failed Map_AddOrUpdate\r\n");
        }
        else
        {
            bool dummy; 
            BUFFER_HANDLE containers = getAzureStorageServiceBody("", listContainersQueryParameters, false, 0, &dummy);
            if (containers == NULL)
            {
                printf("unable to getAzureStorageServiceBody\r\n");
            }
            else
            {
                STRING_HANDLE responseString = STRING_construct_n((const char*)BUFFER_u_char(containers) + 3, BUFFER_length(containers) - 3); /*3 because unknown, maybe BOM... */
                if (responseString == NULL)
                {
                    printf("unable to STRING_construct_n\r\n");
                }
                else
                {
                    char containerName[1000];
                    const char* seekHereContainerName = STRING_c_str(responseString);
                    const char* whereIsStarts;
                    while ((whereIsStarts = strstr(seekHereContainerName, CONTAINER_NAME)) != NULL)
                    {
                        size_t pos = 0;
                        whereIsStarts += strlen(CONTAINER_NAME);
                        while (whereIsStarts[pos] != '<')
                        {
                            containerName[pos] = whereIsStarts[pos];
                            pos++;
                        }
                        containerName[pos] = '\0';
                        printf("DETECTED container name = %s\r\n", containerName);
                        listBlobs(containerName);

                        seekHereContainerName = whereIsStarts + pos;
                    }
                    STRING_delete(responseString);
                }

                BUFFER_delete(containers);
            }
        }
        Map_Destroy(listContainersQueryParameters);
    }
}


int iothub_client_file_download_sample_run(void)
{
    listContainers();
    return 0;
}
