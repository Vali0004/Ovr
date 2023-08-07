#include "hooking/hooking.h"

bool hooks::request(CHttpRequest* pRequest) {
	LOG_DEBUG("\n{}://{}{}/\n{}\n", pRequest->m_protocol, pRequest->m_base_url, pRequest->m_endpoint_data, pRequest->m_http_headers->m_header_data);
	if (util::badEndpoint(pRequest->m_endpoint_data)) {
		return false;
	}
	RET_CALL(request, pRequest);
}