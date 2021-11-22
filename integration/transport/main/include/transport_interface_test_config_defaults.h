/*
 * FreeRTOS Integration Toolkit preview
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef TRANSPORT_INTERFACE_TEST_CONFIG_DEFAULTS_H
#define TRANSPORT_INTERFACE_TEST_CONFIG_DEFAULTS_H

/**
 * @brief Macro that is called in the transport interface test for logging "Error" level
 * messages.
 *
 * To enable error level logging in the transport interface test, this macro should be mapped to the
 * application-specific logging implementation that supports error logging.
 *
 * @note This logging macro is called in the transport interface test with parameters wrapped in
 * double parentheses to be ISO C89/C90 standard compliant. For a reference
 * POSIX implementation of the logging macros, refer to core_mqtt_config.h files, and the
 * logging-stack in demos folder of the
 * [AWS IoT Embedded C SDK repository](https://github.com/aws/aws-iot-device-sdk-embedded-C).
 *
 * <b>Default value</b>: Error logging is turned off, and no code is generated for calls
 * to the macro in the transport interface test on compilation.
 */
#ifndef LogError
    #define LogError( message )
#endif

/**
 * @brief Macro that is called in the transport interface test for logging "Warning" level
 * messages.
 *
 * To enable warning level logging in the transport interface test, this macro should be mapped to the
 * application-specific logging implementation that supports warning logging.
 *
 * @note This logging macro is called in the transport interface test with parameters wrapped in
 * double parentheses to be ISO C89/C90 standard compliant. For a reference
 * POSIX implementation of the logging macros, refer to core_mqtt_config.h files, and the
 * logging-stack in demos folder of the
 * [AWS IoT Embedded C SDK repository](https://github.com/aws/aws-iot-device-sdk-embedded-C/).
 *
 * <b>Default value</b>: Warning logs are turned off, and no code is generated for calls
 * to the macro in the transport interface test on compilation.
 */
#ifndef LogWarn
    #define LogWarn( message )
#endif

/**
 * @brief Macro that is called in the transport interface test for logging "Info" level
 * messages.
 *
 * To enable info level logging in the transport interface test, this macro should be mapped to the
 * application-specific logging implementation that supports info logging.
 *
 * @note This logging macro is called in the transport interface test with parameters wrapped in
 * double parentheses to be ISO C89/C90 standard compliant. For a reference
 * POSIX implementation of the logging macros, refer to core_mqtt_config.h files, and the
 * logging-stack in demos folder of the
 * [AWS IoT Embedded C SDK repository](https://github.com/aws/aws-iot-device-sdk-embedded-C/).
 *
 * <b>Default value</b>: Info logging is turned off, and no code is generated for calls
 * to the macro in the transport interface test on compilation.
 */
#ifndef LogInfo
    #define LogInfo( message )
#endif

/**
 * @brief Macro that is called in the transport interface test for logging "Debug" level
 * messages.
 *
 * To enable debug level logging from transport interface test, this macro should be mapped to the
 * application-specific logging implementation that supports debug logging.
 *
 * @note This logging macro is called in the transport interface test with parameters wrapped in
 * double parentheses to be ISO C89/C90 standard compliant. For a reference
 * POSIX implementation of the logging macros, refer to core_mqtt_config.h files, and the
 * logging-stack in demos folder of the
 * [AWS IoT Embedded C SDK repository](https://github.com/aws/aws-iot-device-sdk-embedded-C/).
 *
 * <b>Default value</b>: Debug logging is turned off, and no code is generated for calls
 * to the macro in the transport interface test on compilation.
 */
#ifndef LogDebug
    #define LogDebug( message )
#endif

/**
 * @brief The buffer size of the transport interface to be tested.
 */
#ifndef transporttestBUFFER_SIZE
    #define transporttestBUFFER_SIZE   ( 2048U )
#endif

/**
 * @brief Run the tranpsport interface test with unity test framework.
 */
#ifndef transporttestUNITY
    #define transporttestUNITY    ( 1U )
#endif

/**
 * @brief The maximum bytes to send to fill the TX buffer.
 */
#ifndef transporttestMAX_SEND_BYTES
    #define transporttestMAX_SEND_BYTES ( 40 * 1024 * 1024U )
#endif

/**
 * @brief Run invalid parameter test due to assertion.
 */
#ifndef transporttestINVALID_PARAMETER_TEST
    #define transporttestINVALID_PARAMETER_TEST  ( 1 )
#endif

#endif /* TRANSPORT_INTERFACE_TEST_CONFIG_DEFAULTS_H. */
