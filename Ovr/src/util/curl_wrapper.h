#pragma once
#include "pch/pch.h"

inline size_t writeCallback(char* buffer, size_t size, size_t nitems, void* data) {
    u64 totalSize{ size * nitems };
    reinterpret_cast<std::string*>(data)->append(buffer, totalSize);
    return totalSize;
}
class curlWrapper {
public:
    curlWrapper() {
        curl = curl_easy_init();
    }
    ~curlWrapper() {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    template <typename ...a>
    bool setEasyOpt(CURLoption option, a&&... args) {
        return curl_easy_setopt(curl, option, args...);
    }
    bool setUrl(const std::string& url) {
        return setEasyOpt(CURLOPT_URL, url.c_str());
    }
    bool setFollowLocation(bool follow) {
        return setEasyOpt(CURLOPT_FOLLOWLOCATION, follow);
    }
    bool setHeader(const std::string& header) {
        m_headers = curl_slist_append(m_headers, header.c_str());
        return m_headers;
    }
    bool setHeaders(const std::vector<std::string>& headers) {
        for (const auto& header : headers) {
            setHeader(header);
        }
        return setEasyOpt(CURLOPT_HTTPHEADER, m_headers);
    }
    bool setPostFields(const std::string& fields) {  
        if (bool result{ setEasyOpt(CURLOPT_POSTFIELDS, fields.c_str()) }) {
            return setEasyOpt(CURLOPT_POSTFIELDSIZE, fields.length());
        }
        return false;
    }
    bool setWriteFunction(curl_write_callback callback) {
        return setEasyOpt(CURLOPT_WRITEFUNCTION, callback);
    }
    bool setWriteData(void* data) {
        return setEasyOpt(CURLOPT_WRITEDATA, data);
    }
    bool perform() {
        CURLcode result{ curl_easy_perform(curl) };
        if (result != CURLE_OK) {
            LOG(Fatal, "cURL error: {}", curl_easy_strerror(result));
            return false;
        }
        return true;
    }
    std::string post(const std::string& url, const std::string& data, const std::vector<std::string>& headers = {}) {
        setUrl(url);
        setEasyOpt(CURLOPT_POST, true);
        setFollowLocation(true);
        setHeaders(headers);
        setPostFields(data);
        setWriteFunction(writeCallback);
        setWriteData(&responseBody);
        if (!perform()) {
            return "bad_res";
        }
        return responseBody;
    }
private:
    CURL* curl;
    struct curl_slist* m_headers{};
    std::string responseBody{};
    std::string responseHeaders{};
};