// Clase auxiliar para implementar la función de Hash del contenedor (unordered_multiset) de Tuplas
#include "../Tupla/Tupla.hpp"

using namespace std;

class TuplaHash {
public:
    size_t operator()(const Tupla &value) const {
        hash<string> hasher;
        // Devuelve un hash de string obtenido a partir de la primera posición de la tupla
        return hasher(value.get(0));
    };
};