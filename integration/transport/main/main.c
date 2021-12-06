/* transport layer integration toolkit */
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "tls_freertos.h"
#include "protocol_examples_common.h"

#include "transport_test_config.h"
#include "transport_interface_tests.h"

#if transportTestIS_ESP32
    #define transportTestLog(...) ESP_LOGI(TAG, __VA_ARGS__)
    #define main(...) app_main()
#else
    #define transportTestLog(...) printf(__VA_ARGS__)
#endif //#if transportTestIS_ESP32

static const char *TAG = "transportTest";

#ifndef ROOT_CA_PEM
    #if CONFIG_BROKER_CERTIFICATE_OVERRIDDEN == 1
    static const uint8_t root_cert_auth_pem_start[]  = "-----BEGIN CERTIFICATE-----\n" CONFIG_BROKER_CERTIFICATE_OVERRIDE "\n-----END CERTIFICATE-----";
    #else
    extern const uint8_t root_cert_auth_pem_start[]   asm("_binary_root_cert_auth_pem_start");
    #endif
    extern const uint8_t root_cert_auth_pem_end[]   asm("_binary_root_cert_auth_pem_end");
#endif

#ifndef CLIENT_CERTIFICATE_PEM
    extern const uint8_t client_cert_pem_start[] asm("_binary_client_crt_start");
    extern const uint8_t client_cert_pem_end[] asm("_binary_client_crt_end");
#endif
#ifndef CLIENT_PRIVATE_KEY_PEM
    extern const uint8_t client_key_pem_start[] asm("_binary_client_key_start");
    extern const uint8_t client_key_pem_end[] asm("_binary_client_key_end");
#endif

/**
 * @brief Path of the file containing the echo server's certificate.
 *
 * @note This certificate should be PEM-encoded.
 * @note This path is relative from the demo binary created. Update
 * SERVER_CA_CERT_PATH to the absolute path if this demo is executed from elsewhere.
 */
#ifndef SERVER_CA_CERT_PATH
    #define SERVER_CA_CERT_PATH    "./certs/server.pem"
#endif

/**
 * @brief Path of the file containing the client's CA certificate.
 *
 * @note This certificate should be PEM-encoded.
 * @note This path is relative from the demo binary created. Update
 * CLIENT_CERT_PATH to the absolute path if this demo is executed from elsewhere.
 */
#ifndef CLIENT_CERT_PATH
    #define CLIENT_CERT_PATH    "./certs/client.pem"
#endif

/**
 * @brief Path of the file containing the client's private key.
 *
 * @note This private key should be PEM-encoded.
 * @note This path is relative from the demo binary created. Update
 * CLIENT_PRIVATE_KEY_PATH to the absolute path if this demo is executed from elsewhere.
 */
#ifndef CLIENT_PRIVATE_KEY_PATH
    #define CLIENT_PRIVATE_KEY_PATH    "./certs/client.key"
#endif

/**
 * @brief Transport timeout in milliseconds for transport send and receive.
 */
#define TRANSPORT_SEND_RECV_TIMEOUT_MS      ( 1000 )

void TransportInit( TransportInterface_t * pTransport )
{
    ServerInfo_t serverInfo;
    const char hostName[] = transportTestECHO_SERVER_HOSTNAME;
    NetworkCredentials_t opensslCredentials;
    TlsTransportStatus_t retStatus = TLS_TRANSPORT_SUCCESS;

    /* Initialize information to connect to the echo server. */
    serverInfo.pHostName = hostName;
    serverInfo.hostNameLength = strlen( hostName );
    serverInfo.port = transportTestECHO_SERVER_PORT;

    /* Initialize credentials for establishing TLS session. */
    memset( &opensslCredentials, 0, sizeof( NetworkCredentials_t ) );

    /* Initialize credentials for establishing TLS session. */
    opensslCredentials.pRootCa = ( const unsigned char * ) root_cert_auth_pem_start;
    opensslCredentials.rootCaSize = root_cert_auth_pem_end - root_cert_auth_pem_start;

    opensslCredentials.pClientCert = ( const unsigned char * ) client_cert_pem_start;
    opensslCredentials.clientCertSize = client_cert_pem_end - client_cert_pem_start;
    opensslCredentials.pPrivateKey = ( const unsigned char * ) client_key_pem_start;
    opensslCredentials.privateKeySize = client_key_pem_end - client_key_pem_start;
    
    #if transportTestECHO_SERVER_OPTION_DISABLE_SNI == 0
        opensslCredentials.disableSni = 0;
    #endif //#if transportTestECHO_SERVER_OPTION_SNI

    retStatus = TLS_FreeRTOS_Connect( pTransport->pNetworkContext,
                    serverInfo.pHostName,
                    serverInfo.port,
                    &opensslCredentials,
                    TRANSPORT_SEND_RECV_TIMEOUT_MS,
                    TRANSPORT_SEND_RECV_TIMEOUT_MS );

    if( retStatus!=TLS_TRANSPORT_SUCCESS ){
        transportTestLog( "Cannot connect to server, ret=%d.\n", retStatus );
    }
}

void TransportDeinit( TransportInterface_t * pTransport )
{
    TLS_FreeRTOS_Disconnect( pTransport->pNetworkContext );
}

void TransportTestDelay( uint32_t delayMs )
{
	const TickType_t xDelay = delayMs / portTICK_PERIOD_MS;
	vTaskDelay( xDelay );
}

int8_t setSocketOperationHandler()
{
    #if transportTestIS_ESP32
        //do nothing
    #else
        static void catch_function(int signo)
        {
            transportTestLog( "Interactive attention signal caught. %d\r\n", signo );
        }

        if( signal( SIGPIPE, catch_function ) == SIG_ERR )
        {
            transportTestLog( "An error occurred while setting a signal handler.\n" );
            return -1;
        }
    #endif //#if transportTestIS_ESP32

    return 0;
}

int main( int argc, char** argv )
{
    /* Allocat the NetworkContext on the stack. */
    NetworkContext_t xNetworkContext = { 0 };
    TransportInterface_t xTransport = { 0 };

    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    /* Setup the signal handler for SIGPIPE. */
    if( setSocketOperationHandler() != 0 ){
        transportTestLog( "An error occurred while setting socket operation handler.\n" );
        return -1;
    }

    /* Setup the transport interface. */
    xTransport.pNetworkContext = &xNetworkContext;
    xTransport.send = TLS_FreeRTOS_send;
    xTransport.recv = TLS_FreeRTOS_recv;
    
    RunTransportInterfaceTests( &xTransport );
    return 0;
}
