#include "hooking/hooking.h"

bool hooks::request(CHttpRequest* pRequest) {
	printf("%s %s %s\n", pRequest->m_protocol, pRequest->m_base_url, pRequest->m_endpoint_data);
	RET_CALL(request, pRequest);
}