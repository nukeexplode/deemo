#include <mutex>
#include <unordered_map>
#include <functional>
#include <vector>

#include "HashBucket.h"

template <typename Key,  typename Value, typename hash_func = std::hash<Key>>
class ConcurrentHashmap {
public:
    ConcurrentHashmap(int size = 100000) : m_hash_size(size) {
        m_hash_table.resize(m_hash_size);
    };
    ~ConcurrentHashmap() {};

    ConcurrentHashmap(const ConcurrentHashmap& map) = delete;
    ConcurrentHashmap& operator=(const ConcurrentHashmap& map) = delete;

    bool insert(const Key& key, const Value& value) {
        int rank = static_cast<int>(m_func(key) % m_hash_size);
        if (m_hash_table[rank] == nullptr) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_hash_table[rank] == nullptr) {
                m_hash_table[rank] = std::make_unique<HashBucket<Key, Value>>();
            }
        }
        return m_hash_table[rank]->insert(key, value);
    }

    bool erase(const Key& key) {
        int rank = static_cast<int>(m_func(key) % m_hash_size);
        if (m_hash_table[rank] == nullptr) {
            return false;
        }
        return m_hash_table[rank]->erase(key);
    }

    bool find(const Key& key, Value& value) {
        int rank = static_cast<int>(m_func(key) % m_hash_size);
        if (m_hash_table[rank] == nullptr) {
            return false;
        }
        return m_hash_table[rank]->find(key, value);
    }

private:
    std::vector<std::unique_ptr<HashBucket<Key, Value>>> m_hash_table;
    const uint32_t m_hash_size;
    hash_func m_func;
    std::mutex m_mutex;
};