#include <shared_mutex>
#include <list>

template<typename Key, typename Value>
class HashNode {
public:
    HashNode() {};
    HashNode(Key key_, Value value_) : key(key_), value(value_) {}
    ~HashNode() {};

    HashNode(const HashNode& map) = delete;
    HashNode& operator=(const HashNode& map) = delete;
    
    const Key& getKey() {
        return key;
    }

    void setValue(const Value& val) {
        value = value;
    }

    Value getValue() {
        return value;
    }
private:
    Key key;
    Value value;
};