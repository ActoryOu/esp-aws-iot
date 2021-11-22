#ifndef TRANSPORT_INTERFACE_TEST_CONFIG_H
#define TRANSPORT_INTERFACE_TEST_CONFIG_H

#include "logging_levels.h"

#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL   LOG_INFO
#endif
#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME    "TrasnportTest"
#endif

#include "logging_stack.h"

/**
 * @brief The buffer size of the transport interface to be tested.
 */
#define transporttestBUFFER_SIZE   ( 2047U )

/**
 * @brief Run the tranpsport interface test without unity test framework.
 */
#define transporttestUNITY  0

/**
 * @brief Disable invalid parameter test due to assertion.
 */
#define transporttestINVALID_PARAMETER_TEST  ( 1 )

#endif
