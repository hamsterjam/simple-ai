#include <iostream>
#include <string>
#include <exception>
#include <any>
#include <map>
#include <functional>

#include "state.h"

namespace simpleai {

    // Adds the key,value pair into the internal state container
    template<typename T> void State::add(std::string key, T value) {
        auto search = this->data.find(key);
        if (search == this->data.end()){
            std::any convertedValue(value);
            this->data.insert(std::make_pair(key, convertedValue));
        } else {
            // Rather than using an option to 'do nothing' with an already existing key insert or returning a bool
            // I use this to clearly tell the programmer that you shouldnt be doing this
            throw std::invalid_argument("Key already exists in this state");
        }
    }

    template<typename T> void State::replace(std::string key, T value) {
        auto search = this->data.find(key);
        if (search == this->data.end()) {
            // Making it explicitly clear this is a replace
            throw std::invalid_argument("Key doesn't exist in this state");
        } else {
            std::any convertedValue(value);
            data.insert_or_assign(key, convertedValue);
        }
    }
    
    void State::remove(std::string key) {
        auto search = this->data.find(key);
        if (search == this->data.end()) {
            // Making it explicitly clear that you didn't remove anything
            throw std::invalid_argument("Key doesn't exist in this state"); 
        } else {
            data.erase(key);
        }
    }

    template<typename T> T State::get(std::string key) {
        auto search = this->data.find(key);
        if (search == this->data.end()) {
            throw std::invalid_argument("Key doesn't exist in this state"); 
        } else {
            return (T(std::any_cast<T>(search->second)));
        }
    }

    std::map<std::string, const std::type_info*> State::getKeyTypes() {
        if (this->size() == 0) {
            throw std::logic_error("There are no keys in this state");   
        } else {
            std::map<std::string, const std::type_info*> returnMap;
            for (auto item = this->data.begin(); item != this->data.end(); ++item) {
                std::string key = std::string(item->first);
                const std::type_info* typeData = &item->second.type();
                returnMap.insert(std::make_pair(key, typeData));
            }
            return returnMap;
        }
    }
}
