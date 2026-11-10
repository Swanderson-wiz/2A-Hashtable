/*
 * hashtable.h
 *
 * A basic hashtable implementation.  This hashtable uses vectors,
 * rather than linked lists, to implement chaining.
 * 
 * Author: <your name here>
 */

#ifndef _HASHTABLE_PROJECT_H
#define _HASHTABLE_PROJECT_H

#include <cstddef>      // for size_t
#include <functional>   // for std::hash
#include <vector>
#include <algorithm>

/***
 * DO NOT put unscoped 'using namespace std;' in header files!
 * Instead put them at the beginning of class or function definitions
 * (as demonstrated below).
 */

template <class T, class H = std::hash<T>>
class hashtable {
public:
    // constructor
    hashtable() : table(4), inserts(0), table_size(4) { ; }

    // basic hashset operations
    void insert(const T& key) {
        if(contains(key)) return;
        inserts++;
        if( load_factor() > load_max ) {
            table_size *= 2;
            std::vector<std::vector<T>> temp_table(table_size);
            for ( auto& chain : table) {
                for ( auto& i : chain) {
                    size_t temp_hash = hash(i) % table_size;
                    temp_table[temp_hash].push_back(std::move(i));
                }//end chain iterate for
            }//end table iterate for
            table = std::move(temp_table);
        }//end resize if
        table[keychain(key)].push_back(key);
    }//end insert
    void remove(const T& key) {
        auto& chain = table[keychain(key)];
        auto target = std::remove(chain.begin(), chain.end(), key);
        if (target != chain.end()) {
            size_t removed_count = std::distance(target, chain.end());
            chain.erase(target, chain.end());
            inserts -= removed_count;
        }//end remove found if
    }//end remove
    bool contains(const T& key) {
        for (auto& i : table[keychain(key)]) {
            if ( i == key ) return true;
        }//end chain iterate for
        return false;
    }//end contains
    size_t size() {
        return inserts;
    }//end size

    // diagnostic functions
    double load_factor() {
        return static_cast<double>(inserts)/table_size;
    }//end load_factor

    // convenience method, invokes the "hash" template parameter
    // function object to get a hash code
    static size_t hash(const T &key) {
        H h;
        return h(key);
    }

    size_t keychain(const T &key) {
        return hash(key) % table_size;
    }//end keychain

private:
    std::vector<std::vector<T>> table;
    size_t inserts;
    size_t table_size;
    double load_max = 0.75;
    
};

#endif
