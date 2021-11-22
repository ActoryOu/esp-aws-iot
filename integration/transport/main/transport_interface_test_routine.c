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

/**
 * @file transport_interface_test_routine.c
 * @brief Integration tests for the transport interface test implementation.
 */

/* Standard header includes. */
#include <stdbool.h>
#include <string.h>

/* Include for transport interface. */
#include "transport_interface.h"

/* Include for transport interface test config. */
#include "transport_interface_test_config.h"
#include "transport_interface_test_config_defaults.h"

/* Include for transport interface routine and hook functions. */
#include "transport_interface_test_routine.h"

#if transporttestUNITY
    /* Include for Unity framework. */
    #include "unity.h"
    #include "unity_fixture.h"
#else
    /* Include for test framework if Unity is not supported. */
    #include "transport_interface_test_compatible.h"
#endif

/*-----------------------------------------------------------*/

/**
 * @brief The test message send in the test.
 */
#define TRANSPORT_TEST_EXAMPLE_MESSAGE    "Hello World!"

/**
 * @brief The disconnect message to the echo server.
 */
#define TRANSPORT_TEST_DISCONNECT_MESSAGE    "DISCONNECT"

/*-----------------------------------------------------------*/

/**
 * @brief TransportInterface pointer used in test cases.
 */
static TransportInterface_t * pTestTransport = NULL;

/**
 * @brief The buffer for the send data.
 */
static uint8_t transportTestSendBuffer[ transporttestBUFFER_SIZE ];

/**
 * @brief The buffer for the read data.
 */
static uint8_t transportTestRecvBuffer[ transporttestBUFFER_SIZE ];

#if ( transporttestUNITY == 0 )
    /**
     * @brief The test framework context to keep the test information if Unity is not supported.
     */
    TransportTestContext_t TransportTestContext = { 0 };
#endif

/**
 * @brief Test group for transport interface test.
 */
TEST_GROUP( Full_TransportInterfaceTest );

/*-----------------------------------------------------------*/

/**
 * @brief Test setup function for transport interface test.
 */
TEST_SETUP( Full_TransportInterfaceTest )
{
    /* Call the hook function implemented by developer to connect the transport interface. */
    TransportConnectHook( pTestTransport );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test tear down function for transport interface test.
 */
TEST_TEAR_DOWN( Full_TransportInterfaceTest )
{
    /* Call the hook function implemented by developer to disconnect the transport interface. */
    TransportDisconnectHook( pTestTransport );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface send with NULL network context pointer handling.
 */
TEST( Full_TransportInterfaceTest, TransportSend_NetworkContextNullPtr )
{
    char testBuffer[] = TRANSPORT_TEST_EXAMPLE_MESSAGE;
    int32_t sendResult = 0;

    /* Ensure the pTestTransport is not NULL and send function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->send != NULL );

    /* Send with NULL network context pointer should return negative value. */
    sendResult = pTestTransport->send( NULL, testBuffer, strlen( testBuffer ) );
    TEST_ASSERT_MESSAGE( sendResult < 0, "Transport interface send with NULL NetworkContext_t "
                                         "pointer should return negative value." );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface send with NULL buffer pointer handling.
 */
TEST( Full_TransportInterfaceTest, TransportSend_BufferNullPtr )
{
    int32_t sendResult = 0;

    /* Ensure the pTestTransport is not NULL and send function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->send != NULL );

    /* Send with NULL buffer pointer should return negative value. */
    sendResult = pTestTransport->send( pTestTransport->pNetworkContext, NULL, 1 );
    TEST_ASSERT_MESSAGE( sendResult < 0, "Transport interface send with NULL buffer "
                                         "pointer should return negative value." );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface send with zero bytes to send handling.
 */
TEST( Full_TransportInterfaceTest, TransportSend_ZeroBytesToSend )
{
    char testBuffer[] = TRANSPORT_TEST_EXAMPLE_MESSAGE;
    int32_t sendResult = 0;

    /* Ensure the pTestTransport is not NULL and send function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->send != NULL );

    /* Send with zeroy bytes to send should return 0. */
    sendResult = pTestTransport->send( pTestTransport->pNetworkContext, testBuffer, 0 );
    TEST_ASSERT_MESSAGE( sendResult == 0, "Transport interface send with zero bytes "
                                          "to send should return 0." );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface recv with NULL network context pointer handling.
 */
TEST( Full_TransportInterfaceTest, TransportRecv_NetworkContextNullPtr )
{
    char testBuffer[] = TRANSPORT_TEST_EXAMPLE_MESSAGE;
    int32_t recvResult = 0;

    /* Ensure the pTestTransport is not NULL and recv function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->recv != NULL );

    /* Receive with NULL network context pointer should return negative value. */
    recvResult = pTestTransport->recv( NULL, testBuffer, sizeof( testBuffer ) );
    TEST_ASSERT_MESSAGE( recvResult < 0, "Transport interface recv with NULL network "
                                         "context pointer should return negative value." );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface recv with NULL buffer pointer handling.
 */
TEST( Full_TransportInterfaceTest, TransportRecv_BufferNullPtr )
{
    char testBuffer[] = TRANSPORT_TEST_EXAMPLE_MESSAGE;
    int32_t recvResult = 0;

    /* Ensure the pTestTransport is not NULL and recv function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->recv != NULL );

    /* Receive with NULL buffer pointer should return negative value. */
    recvResult = pTestTransport->recv( pTestTransport->pNetworkContext, NULL, sizeof( testBuffer ) );
    TEST_ASSERT_MESSAGE( recvResult < 0, "Transport interface recv with NULL buffer "
                                         "pointer should return negative value." );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface recv zero bytes to receive handling.
 */
TEST( Full_TransportInterfaceTest, TransportRecv_ZeroBytesToRecv )
{
    char testBuffer[] = TRANSPORT_TEST_EXAMPLE_MESSAGE;
    int32_t recvResult = 0;

    /* Ensure the pTestTransport is not NULL and recv function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->recv != NULL );

    /* Receive with zeroy bytes to recv should return 0. */
    recvResult = pTestTransport->recv( pTestTransport->pNetworkContext, testBuffer, 0 );
    TEST_ASSERT_MESSAGE( recvResult == 0, "Transport interface recv with zero "
                                          "bytes to recv should return 0." );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface with send, receive and compare on different length.
 */
TEST( Full_TransportInterfaceTest, Transport_SendRecvVaryLength )
{
    uint32_t testSize = 0;
    uint32_t transferTotal = 0;
    int32_t transportResult = 0;

    /* Ensure the pTestTransport is not NULL and recv function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->recv != NULL );
    TEST_ASSERT( pTestTransport->send != NULL );

    /* Setup the send data buffer. */
    for( testSize = 0; testSize < transporttestBUFFER_SIZE; testSize++ )
    {
        transportTestSendBuffer[ testSize ] = ( ( uint8_t )( testSize % 256 ) );
    }

    testSize = 1;
    for( ; ; )
    {
        LogInfo( ( "testsize : %u", testSize ) );
        /* Send the data to echo server. */
        transferTotal = 0;
        while( transferTotal < testSize )
        {
            transportResult = pTestTransport->send( pTestTransport->pNetworkContext,
                                                    &transportTestSendBuffer[ transferTotal ],
                                                    testSize - transferTotal );
            TEST_ASSERT( transportResult >= 0 );
            TEST_ASSERT( testSize >= ( transferTotal + transportResult ) );
            
            transferTotal = transferTotal + transportResult;
        }

        /* Receive the data from echo server. */
        memset( transportTestRecvBuffer, 0, testSize );
        transferTotal = 0;
        while( transferTotal < testSize )
        {
            transportResult = pTestTransport->recv( pTestTransport->pNetworkContext,
                                                    &transportTestRecvBuffer[ transferTotal ],
                                                    testSize - transferTotal );
            TEST_ASSERT( transportResult >= 0 );
            TEST_ASSERT( testSize >= ( transferTotal + transportResult ) );
            
            transferTotal = transferTotal + transportResult;
        }
        
        /* Compare the result. */
        TEST_ASSERT_MESSAGE( memcmp( transportTestSendBuffer, transportTestRecvBuffer, testSize ) == 0,
            "Send and receive buffer memory compare failed" );

        /* The test size increases the factor of 2. */
        if( ( testSize * 2U ) < transporttestBUFFER_SIZE )
        {
            testSize = testSize * 2U;
        }
        else if( testSize < transporttestBUFFER_SIZE )
        {
            testSize = transporttestBUFFER_SIZE;
        }
        else
        {
            /* Test complete. */
            break;
        }
    }
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface disconnected send function return value.
 */
TEST( Full_TransportInterfaceTest, TransportSend_RemoteDisconnect )
{
    int32_t transportResult = 0;
    uint32_t transportTotal = 0;
    char disconnectMessage[] = TRANSPORT_TEST_DISCONNECT_MESSAGE;

    /* Ensure the pTestTransport is not NULL and send/recv function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->send != NULL );
    TEST_ASSERT( pTestTransport->recv != NULL );

    /* Send the disconnect command to remote server. */
    transportResult = pTestTransport->send( pTestTransport->pNetworkContext, disconnectMessage, strlen(disconnectMessage) );
    TEST_ASSERT( transportResult == strlen(disconnectMessage) );
    
    /* Send the data to the remote server. */
    do
    {
        /* The send buffer content is irrelevant. */
        transportResult = pTestTransport->send( pTestTransport->pNetworkContext,
                                                transportTestSendBuffer,
                                                transporttestBUFFER_SIZE );
        if( transportResult > 0 )
        {
            transportTotal = transportTotal + transportResult;
        }

        /* Check the transport send limits. */
        TEST_ASSERT( transportTotal < transporttestMAX_SEND_BYTES );
    } while( transportResult > 0 );
    LogInfo( ( "transportResult : %d, transportTotal : %u", transportResult, transportTotal ) );
    TEST_ASSERT_MESSAGE( transportResult < 0, "Transport send should return negative value when disconnected" );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface disconnected receive function return value.
 */
TEST( Full_TransportInterfaceTest, TransportRecv_RemoteDisconnect )
{
    int32_t transportResult = 0;
    char disconnectMessage[] = TRANSPORT_TEST_DISCONNECT_MESSAGE;

    /* Ensure the pTestTransport is not NULL and send/recv function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->send != NULL );
    TEST_ASSERT( pTestTransport->recv != NULL );

    /* Send the disconnect command to remote server. */
    transportResult = pTestTransport->send( pTestTransport->pNetworkContext, disconnectMessage, strlen(disconnectMessage) );
    TEST_ASSERT( transportResult == strlen(disconnectMessage) );

    /* Receive the data from the remote server. */
    transportResult = pTestTransport->recv( pTestTransport->pNetworkContext, transportTestRecvBuffer, transporttestBUFFER_SIZE );
    LogInfo( ( "transportResult : %d", transportResult ) );
    TEST_ASSERT_MESSAGE( transportResult < 0, "Transport receive should return negative value when disconnected" );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface receive function return value when no data to receive.
 */
TEST( Full_TransportInterfaceTest, TransportRecv_NoDataToReceive )
{
    int32_t transportResult = 0;

    /* Ensure the pTestTransport is not NULL and recv function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->recv != NULL );

    /* Receive from the echo server. No data will be returned. */
    transportResult = pTestTransport->recv( pTestTransport->pNetworkContext,
                                            transportTestRecvBuffer,
                                            transporttestBUFFER_SIZE );
    TEST_ASSERT_MESSAGE( transportResult == 0, "No data to receive should return 0" );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface receive return zero retry.
 */
TEST( Full_TransportInterfaceTest, TransportRecv_ReturnZeroRetry )
{
    int32_t transportResult = 0;

    /* Ensure the pTestTransport is not NULL and send/recv function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->send != NULL );
    TEST_ASSERT( pTestTransport->recv != NULL );

    /* Receive from the echo server. No data will be returned. */
    transportResult = pTestTransport->recv( pTestTransport->pNetworkContext,
                                            transportTestRecvBuffer,
                                            transporttestBUFFER_SIZE );
    TEST_ASSERT_MESSAGE( transportResult == 0, "No data to receive should return 0" );

    /* Send somd data to echo server. */
    transportResult = pTestTransport->send( pTestTransport->pNetworkContext,
                                            transportTestSendBuffer,
                                            transporttestBUFFER_SIZE );
    TEST_ASSERT( transportResult > 0 );

    /* Receive from the echo server again. Zero should not be returned. */
    transportResult = pTestTransport->recv( pTestTransport->pNetworkContext,
                                            transportTestRecvBuffer,
                                            transporttestBUFFER_SIZE );
    TEST_ASSERT_MESSAGE( transportResult > 0, "Retry receive should return positive value" );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface receive return zero retry.
 */
TEST( Full_TransportInterfaceTest, TransportSend_NoDataTransmitted )
{
    int32_t transportResult = 0;
    uint32_t transportTotal = 0;

    /* Ensure the pTestTransport is not NULL and recv function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->recv != NULL );
    TEST_ASSERT( pTestTransport->send != NULL );

    /* Send some data to echo server. */
    do
    {
        /* The send buffer content is irrelevant. */
        transportResult = pTestTransport->send( pTestTransport->pNetworkContext,
                                                transportTestSendBuffer,
                                                transporttestBUFFER_SIZE );
        TEST_ASSERT( transportResult >= 0 );
        transportTotal = transportTotal + transportResult;

        /* Check the transport send limits. */
        TEST_ASSERT( transportTotal < transporttestMAX_SEND_BYTES );
    } while( transportResult > 0 );
        
    LogInfo( ( "transportResult : %d, transportTotal : %u", transportResult, transportTotal ) );
    TEST_ASSERT_MESSAGE( transportResult == 0, "Transport interface send should return 0 when TX buffer full" );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test transport interface receive return zero retry.
 */
TEST( Full_TransportInterfaceTest, TransportSend_ReturnZeroRetry )
{
    int32_t transportResult = 0;
    uint32_t transportTotal = 0;

    /* Ensure the pTestTransport is not NULL and recv function is not NULL. */
    TEST_ASSERT( pTestTransport != NULL );
    TEST_ASSERT( pTestTransport->recv != NULL );
    TEST_ASSERT( pTestTransport->send != NULL );

    /* Send some data to echo server. */
    do
    {
        /* The send buffer content is irrelevant. */
        transportResult = pTestTransport->send( pTestTransport->pNetworkContext,
                                                transportTestSendBuffer,
                                                transporttestBUFFER_SIZE );
        TEST_ASSERT( transportResult >= 0 );
        transportTotal = transportTotal + transportResult;
        TEST_ASSERT( transportTotal < transporttestMAX_SEND_BYTES );
    } while( transportResult > 0 );
    LogInfo( ( "transportResult : %d, transportTotal : %u", transportResult, transportTotal ) );
    TEST_ASSERT_MESSAGE( transportResult == 0, "Transport interface send should return 0 when TX buffer full" );

    /* Receive all the data from transport interface. */
    do
    {
        transportResult = pTestTransport->recv( pTestTransport->pNetworkContext,
                                                transportTestRecvBuffer,
                                                transporttestBUFFER_SIZE );
        TEST_ASSERT( transportResult >= 0 );
    } while( transportResult > 0 );

    /* Send some data to the server should return positivie value. */
    transportResult = pTestTransport->send( pTestTransport->pNetworkContext,
                                            transportTestSendBuffer,
                                            transporttestBUFFER_SIZE );
    LogInfo( ( "Transport Result %d", transportResult ) );
    TEST_ASSERT_MESSAGE( transportResult > 0, "Transport interface send should return positive after retry" );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test group runner for transport interface test against echo server.
 */
TEST_GROUP_RUNNER( Full_TransportInterfaceTest )
{
    /* Invalid parameter test. */
    #if ( transporttestINVALID_PARAMETER_TEST == 1 )
        RUN_TEST_CASE( Full_TransportInterfaceTest, TransportSend_NetworkContextNullPtr );
        RUN_TEST_CASE( Full_TransportInterfaceTest, TransportSend_BufferNullPtr );
        RUN_TEST_CASE( Full_TransportInterfaceTest, TransportSend_ZeroBytesToSend );
        RUN_TEST_CASE( Full_TransportInterfaceTest, TransportRecv_NetworkContextNullPtr );
        RUN_TEST_CASE( Full_TransportInterfaceTest, TransportRecv_BufferNullPtr );
        RUN_TEST_CASE( Full_TransportInterfaceTest, TransportRecv_ZeroBytesToRecv );
    #endif

    /* Send and receive correctness. */
    RUN_TEST_CASE( Full_TransportInterfaceTest, Transport_SendRecvVaryLength );

    /* Transport disconnection test. */
    RUN_TEST_CASE( Full_TransportInterfaceTest, TransportSend_RemoteDisconnect );
    RUN_TEST_CASE( Full_TransportInterfaceTest, TransportRecv_RemoteDisconnect );
    
    /* Transport behavior test. */
    RUN_TEST_CASE( Full_TransportInterfaceTest, TransportRecv_NoDataToReceive );
    RUN_TEST_CASE( Full_TransportInterfaceTest, TransportRecv_ReturnZeroRetry );
    RUN_TEST_CASE( Full_TransportInterfaceTest, TransportSend_NoDataTransmitted );
    RUN_TEST_CASE( Full_TransportInterfaceTest, TransportSend_ReturnZeroRetry );
}

/*-----------------------------------------------------------*/

/**
 * @brief The transport interface test routine to run all the tests.
 */
void TransportInterfaceTestRoutine( TransportInterface_t * pTransport )
{
    /* Assign the TransportInterface_t pointer used in test cases. */
    pTestTransport = pTransport;

    #if transporttestUNITY
        /* Initialize unity. */
        UnityFixture.Verbose = 1;
        UnityFixture.GroupFilter = 0;
        UnityFixture.NameFilter = 0;
        UnityFixture.RepeatCount = 1;
        UNITY_BEGIN();
    #endif

    /* Run the test group. */
    RUN_TEST_GROUP( Full_TransportInterfaceTest );

    #if transporttestUNITY
        UNITY_END();
    #endif
}

/*-----------------------------------------------------------*/
