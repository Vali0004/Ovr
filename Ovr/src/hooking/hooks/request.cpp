#include "hooking/hooking.h"

bool hooks::request(CHttpRequest* pRequest) {
	std::string endpoint{ pRequest->m_endpoint_data };
	if (util::badEndpoint(endpoint)) {
		return false;
	}
	LOG_DEBUG("\n{}://{}{}/\n{}\n", pRequest->m_protocol, pRequest->m_base_url, pRequest->m_endpoint_data, pRequest->m_http_headers->data());
	bool res{ CALL(request, pRequest) };
	LOG_DEBUG("Request response: {}", pRequest->m_response->data());
	return res;
}