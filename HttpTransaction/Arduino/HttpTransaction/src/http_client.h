#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_

extern "C" {
    #include "iot/http.h"
    #include "iot/ntsetting.h"
}

typedef
enum _http_trx_set_exec_mode_e
{
	EM_WAIT_NEW_EXEC_MODE_E = -1,
	EM_STOP_E = 0,
	EM_RUN_ONCE_E,
	EM_RUN_INTERVAL_E,
}
HTTP_TRX_SET_EXEC_MODE_E;

typedef struct HTTP_TRX_SET_EXEC_MODE_T
{
    HTTP_TRX_SET_EXEC_MODE_E m;
}HTTP_TRX_SET_EXEC_MODE;

#define EM_WAIT_NEW_EXEC_MODE   ((HTTP_TRX_SET_EXEC_MODE){EM_WAIT_NEW_EXEC_MODE_E})
#define EM_STOP                 ((HTTP_TRX_SET_EXEC_MODE){EM_STOP_E})
#define EM_RUN_ONCE             ((HTTP_TRX_SET_EXEC_MODE){EM_RUN_ONCE_E})
#define EM_RUN_INTERVAL         ((HTTP_TRX_SET_EXEC_MODE){EM_RUN_INTERVAL_E})

//
enum _http_trx_set_status
{
	IDLE,
	RUNNING
};
int8_t http_trx_job(void);

void http_trx_set_status(int8_t status);
int8_t http_trx_get_status(void);
void http_trx_set_exec_interval_ms(unsigned long interval_ms);
unsigned long http_trx_get_exec_interval_ms(void);
void http_trx_set_rx_buffer(char *rx_buffer);
char * http_trx_get_rx_buffer(void);
void http_trx_set_exec_mode(HTTP_TRX_SET_EXEC_MODE exec_mode);
HTTP_TRX_SET_EXEC_MODE_E http_trx_get_exec_mode(void);

#define HTTP_TRX_RX_BUFFER_MAX_SIZE 400

#endif //HTTP_CLIENT_H_
