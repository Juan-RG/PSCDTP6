// Clase auxiliar para implementar la función de Hash del unordered_multiset de Tuplas
#include "Tupla.hpp"
using namespace std;

class TuplaHash {
public:
    size_t operator()(const Tupla &value) const {
        hash<string> hasher;

        return hasher(value.get(0));
        //return std::hash<std::string>(value.get(0));
    };
};