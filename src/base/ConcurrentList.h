#include <shared_mutex>
#include <list>
#include <mutex>

template <typename T>
class ConcurrentList {
public:
    ConcurrentList() {}
    ~ConcurrentList() {}

    bool insert(T value) {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        list.push_back(value);
        return true;
    }

    bool erase(const T& value) {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        auto iter = std::find(list.beign(), list.end(), value);
        if (iter == list.end()) return false;
        list.erase(iter);
        return true;
    }

    bool erase(std::function<bool (T&)> func) {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        for (auto iter = list.begin(); iter != list.end(); iter++) {
            if (func(*iter)) {
                list.erase(iter);
            }
        }

        return true;
    }

    std::list<T> get(std::function<bool (T&)> func) {
        std::list<T> res;
        for (auto val : list) {
            if (func(val)) {
                res.push_back(val);
            }
        }

        return list;
    }
private:
    std::shared_mutex m_mutex;
    std::list<T> list;
};