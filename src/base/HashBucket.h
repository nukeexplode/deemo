#include <shared_mutex>
#include <list>

#include "HashNode.h"

template<typename Key, typename Value>
class HashBucket {
public:
    HashBucket() {};
    ~HashBucket() {};

    HashBucket(const HashBucket& map) = delete;
    HashBucket& operator=(const HashBucket& map) = delete;
    
    bool insert(const Key& key, const Value& value) {
        std::unique_lock<std::shared_mutex> lock(m_mutex);

        for (auto& node : m_list) {
            if (node->getKey() == key) {
                node->setValue(value);
                return false;
            }
        }

        m_list.push_back(std::make_unique<HashNode<Key, Value>>(key, value));
        return true;
    }

    bool erase(const Key& key) {
        std::unique_lock<std::shared_mutex> lock(m_mutex);

        for (auto iter = m_list.begin(); iter != m_list.end(); iter++) {
            if ((*iter)->getKey() == key) {
                m_list.erase(iter);
                return true;
            }
        }

        return false;
    }

    bool find(const Key& key, Value& value) {
        std::shared_lock<std::shared_mutex> lock(m_mutex);
        for (auto& node : m_list) {
            if (node->getKey() == key) {
                value = node->getValue();
                return true;
            }
        }

        return false;
    }
private:
    std::list<std::unique_ptr<HashNode<Key, Value>>> m_list;
    std::shared_mutex m_mutex;
};