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

#ifndef TRANSPORT_INTERFACE_TEST_ROUTINE_H
#define TRANSPORT_INTERFACE_TEST_ROUTINE_H

/**
 * @brief Hook function to connect the transport interface.
 *
 * The hook function called in the transport interface test routine.
 * This function should connect the transport interface.
 *
 * @param[in] pTransport The transport interface to use with the context.
 */
void TransportConnectHook( TransportInterface_t * pTransport );

/**
 * @brief Hook function to disconnect the transport interface.
 *
 * The hook function called in the transport interface test routine.
 * This function should disconnect the transport interface.
 *
 * @param[in] pTransport The transport interface to use with the context.
 */
void TransportDisconnectHook( TransportInterface_t * pTransport );

/**
 * @brief Entry function of Transport interface test.
 *
 * The test routine is design to verify transport interface implementation and has
 * no platform dependency. Developers call this test routine in their application
 * to start the test.
 *
 * @param[in] pTransport transport interface pointer.
 */
void TransportInterfaceTestRoutine( TransportInterface_t * pTransport );

#endif /* TRANSPORT_INTERFACE_TEST_ROUTINE_H. */
