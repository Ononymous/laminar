#include <vector>

#include "riot.h"

std::vector<std::string_view> split(const std::string_view &s,
                                    const std::string &sep) {
    std::vector<std::string_view> result;
    std::string_view token;
    size_t last = 0;
    size_t next = 0;

    while ((next = s.find(sep, last)) != std::string_view::npos) {
        token = s.substr(last, next - last);
        result.push_back(token);
        last = next + sep.size();
    }
    token = s.substr(last);
    result.push_back(token);

    return result;
}

void split(const std::string_view &s, const std::string &sep,
           std::function<void(std::string_view &)> &f) {
    std::string_view token;
    size_t last = 0;
    size_t next = 0;

    while ((next = s.find(sep, last)) != std::string_view::npos) {
        token = s.substr(last, next - last);
        f(token);
        last = next + sep.size();
    }
    token = s.substr(last);
    f(token);
}

std::string serialize(const std::unordered_map<std::string, double> &m) {
    std::string result = "";
    for (auto &[k, v] : m) {
        result += k;
        result += '~';
        result += std::to_string(v);
        result += '\t';
    }

    if (result != "") {
        result.pop_back();  // remove the trailing '\t'
    }

    return result;
}

std::unordered_map<std::string, double> deserialize(const std::string_view &s) {
    std::unordered_map<std::string, double> m;

    std::function<void(std::string_view &)> add_to_map =
        [&](std::string_view &kv_pair) {
            std::string_view key = kv_pair.substr(0, kv_pair.find('~'));
            std::string_view value = kv_pair.substr(kv_pair.find('~') + 1);
            if (key != " ") {
                m[std::string(key)] = atof(std::string(value).c_str());
            }
        };

    split(s, "\t", add_to_map);

    return m;
}