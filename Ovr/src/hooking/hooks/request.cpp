#include "hooking/hooking.h"

bool hooks::request(CHttpRequest* pRequest) {
	std::string endpoint{ pRequest->m_endpoint_data };
	if (util::badEndpoint(endpoint)) {
		return false;
	}
	RET_CALL(request, pRequest);
}