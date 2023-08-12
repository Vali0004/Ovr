#include "hooking/hooking.h"

bool hooks::request(CHttpRequest* pRequest) {
	requestData data{};
	data.protocol = pRequest->m_protocol;
	data.baseUrl = pRequest->m_base_url;
	data.endpoint = pRequest->m_endpoint_data;
	data.url = std::format("{}://{}{}", pRequest->m_protocol, pRequest->m_base_url, pRequest->m_endpoint_data);
	std::string endpoint{ data.endpoint };
	data.response = pRequest->m_response->m_data;
	data.contentLength = std::to_string(pRequest->m_content_length);
	data.addHeadersFromString(pRequest->m_http_headers->m_data);
	g_requests.push_back(data);
	if (util::badEndpoint(endpoint)) {
		return false;
	}
	RET_CALL(request, pRequest);
}