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

#ifndef TRANSPORT_INTERFACE_TEST_COMPATIBLE_H
#define TRANSPORT_INTERFACE_TEST_COMPATIBLE_H

/*-----------------------------------------------------------*/

/**
 * @brief Test context to keep testing information.
 */
typedef struct TransportTestContext
{
    bool testResult;       /**< @brief The variable to keep the test result. */
    uint32_t testCount;    /**< @brief The variable to count the test cases. */
    uint32_t testFailures; /**< @brief The variable to keep the number of test fail. */
} TransportTestContext_t;

/*-----------------------------------------------------------*/

/**
 * @brief Macro to run a test case defined with TEST macro.
 */
#ifndef RUN_TEST_CASE
    #define RUN_TEST_CASE( testGroup, testfunction )        \
    do{                                                     \
        _ ## testGroup ## _Setup();                         \
        TransportTestContext.testResult = true;             \
        TransportTestContext.testCount++;                   \
        _ ## testfunction ## _();                           \
        if( TransportTestContext.testResult == true )       \
        {                                                   \
            LogInfo( ( "TEST(%s) PASS", # testfunction ) ); \
        }                                                   \
        else                                                \
        {                                                   \
            LogInfo( ( "TEST(%s) FAIL", # testfunction ) ); \
            TransportTestContext.testFailures++;            \
        }                                                   \
        _ ## testGroup ## _TearDown();                      \
    } while( 0 )
#endif /* ifndef RUN_TEST_CASE */

/**
 * @brief Macro to define a test group.
 */
#ifndef TEST_GROUP
    #define TEST_GROUP( x )
#endif

/**
 * @brief Macro to define setup function to be called before running the test case.
 */
#ifndef TEST_SETUP
    #define TEST_SETUP( testGroup )    static void _ ## testGroup ## _Setup( void )
#endif

/**
 * @brief Macro to define tear down function to be called after running the test case.
 */
#ifndef TEST_TEAR_DOWN
    #define TEST_TEAR_DOWN( testGroup )    static void _ ## testGroup ## _TearDown( void )
#endif

/**
 * @brief Macro to define the test case.
 */
#ifndef TEST
    #define TEST( testGroup, testFunction )    static void _ ## testFunction ## _( void )
#endif

/**
 * @brief Test assertion macro. The test case will return fail if there is condition is false.
 */
#ifndef TEST_ASSERT
    #define TEST_ASSERT( condition )                 \
    do{                                              \
        if( ( condition ) == false )                 \
        {                                            \
            TransportTestContext.testResult = false; \
            return;                                  \
        }                                            \
    } while ( 0 )
#endif

/**
 * @brief Test assertion macro. The test case will return fail if there is condition is false.
 * Error log will be printed
 */
#ifndef TEST_ASSERT_MESSAGE
    #define TEST_ASSERT_MESSAGE( condition, message ) \
    do{                                               \
        if( ( condition ) == false )                  \
        {                                             \
            TransportTestContext.testResult = false;  \
            LogError( ( "%s", message ) );            \
            return;                                   \
        }                                             \
    } while ( 0 )
#endif

/**
 * @brief Macro to define the test group runner.
 */
#ifndef TEST_GROUP_RUNNER
    #define TEST_GROUP_RUNNER( testGroup )    static void _ ## testGroup ## _( void )
#endif

/**
 * @brief Macro to run the test group.
 */
#ifndef RUN_TEST_GROUP
    #define RUN_TEST_GROUP( testGroup )                   \
    do{                                                   \
        _ ## testGroup ## _();                            \
        LogInfo( ( "-----------------------" ) );         \
        LogInfo( ( "%u Tests %u Failures",                \
                   TransportTestContext.testCount,        \
                   TransportTestContext.testFailures ) ); \
    } while( 0 )
#endif

/*-----------------------------------------------------------*/

/**
 * @brief Define the test context structure to be used if unity is not supported.
 */
extern TransportTestContext_t TransportTestContext;

#endif /* TRANSPORT_INTERFACE_TEST_COMPATIBLE_H. */
