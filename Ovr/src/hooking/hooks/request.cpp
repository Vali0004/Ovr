#include "hooking/hooking.h"

bool hooks::request(CHttpRequest* pRequest) {
	if (util::badEndpoint(pRequest->m_endpoint_data)) {
		strcpy(pRequest->m_base_url, "amazon.com"); //Checks if server cert is from AWS
	}
	RET_CALL(request, pRequest);
}