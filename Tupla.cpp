//*****************************************************************
// File:   Tupla.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2020
// Coms:
//*****************************************************************

#include <vector>
#include <string>
#include <cassert>
#include "Tupla.hpp"
#include <iostream> //                                                              <<-------------------------------------BOrrame pls
#include <sstream>  // stringstream para reemplazar sscanf
#include <regex>

Tupla::Tupla(int n) {
    data = new vector<string>(n,"");
}

Tupla::Tupla(string s1) {
    data = new vector<string>(1,s1);
}

Tupla::Tupla(string s1, string s2) {
    data = new vector<string>(2,"");
    data->at(0) = s1;
    data->at(1) = s2;
    //alternativamente se podía haber hecho como
    //data = new vector<string>{s1,s2};
}

Tupla::Tupla(string s1, string s2, string s3) {
    data = new vector<string>(3,"");
    data->at(0) = s1;
    data->at(1) = s2;
    data->at(2) = s3;
}

Tupla::Tupla(string s1, string s2, string s3, string s4) {
    data = new vector<string>(4,"");
    data->at(0) = s1;
    data->at(1) = s2;
    data->at(2) = s3;
    data->at(3) = s4;
}

Tupla::Tupla(string s1, string s2, string s3, string s4, string s5) {
    data = new vector<string>(5,"");
    data->at(0) = s1;
    data->at(1) = s2;
    data->at(2) = s3;
    data->at(3) = s4;
    data->at(4) = s5;
}

Tupla::Tupla(string s1, string s2, string s3, string s4, string s5, string s6) {
    data = new vector<string>(6,"");
    data->at(0) = s1;
    data->at(1) = s2;
    data->at(2) = s3;
    data->at(3) = s4;
    data->at(4) = s5;
    data->at(5) = s6;
}

Tupla::Tupla(Tupla& t) {
    data = new vector<string>(t.size(),"");
    for(int i = 0;i<t.size();i++){
        data->at(i) = t.get(i);
    }
}

Tupla::Tupla(const Tupla &t) {
    data = new vector<string>(t.data->size(), "");
    for (int i = 0; i < t.data->size(); i++) {
        data->at(i) = t.data->at(i);
    }
}

void Tupla::igual(Tupla &t) {
    for (int i = 0; i < t.data->size(); i++) {
        data->at(i) = t.data->at(i);
    }
}

bool Tupla::operator<(const Tupla b) const {    //FIxme :eliminar
    return (this->data->at(0) <= b.data->at(0));
    //if (this->data->at(0) == b.data->at(0)){
    //    return false;
    //}
    //return true;
    //return this->data->size() > b.data->size();
}

// Operador iguales para el unordered multiset, devuelve true si y solo si las dos tuplas tienen el mismo tamaño y contenido
bool Tupla::operator ==(const Tupla& b) const {                                                                          // TODO: Ver si se puede poner con &
    if (data->size() != b.size()) {
        return false; // ya sabemos que no son iguales
    } else {
        for (int i = 0; i < data->size(); i++) {
            if (data->at(i) != b.get(i)) {
                return false; // si hay una posición en la que sus strings no sean iguales, se devuelve false
            }
        }
        return true; // ya sabemos que lo son
    }
}

// Operador hash
//std::hash<Tupla>Tupla::operator()() {                                                                         // TODO: Ver si se puede poner con &
    // hash basado en la posición 0 de la tupla (podría ser cualquiera, pero de esta forma es general a todas)
    //const std::hash<std::string> m_stringHash {};

    //return m_stringHash(data->at(0));

    //return hash<string>()(data->at(0));
//}

Tupla::~Tupla() {
    delete data;
}

int Tupla::size() const{
    return data->size();
}

string Tupla::to_string() const{
    string res = "[";
    for (int i=0; i<data->size()-1; i++) {
        res = res + data->at(i) + ",";
    }
    res = res + data->at(data->size()-1) + "]";
    return res;
}

string Tupla::get(int pos) const{
    return data->at(pos);
}

void Tupla::set(int pos, string value) {
    data->at(pos) = value;
}

void Tupla::trocear(string elem[],int strings, string s) {
    stringstream s_stream(s); //create string stream from the string
    string substr;
    int i;
    getline(s_stream, substr, '['); //get first string delimited by comma
    for(i = 0; i < strings-1; i++){
        getline(s_stream, substr, ','); //get first string delimited by comma
        elem[i]=substr;
    }
    getline(s_stream, substr, ']');
    elem[i]=substr;
    for (int j = 0; j < elem->size() ; ++j) {
    }
}

void Tupla::from_string(string s) {
    assert(s.length()>2 && s[0]=='[' && s[s.length()-1]==']');
    int strings = 1;
    string elem[6];

    for(int i = 0; i<s.length();i++){                                                       //controlar en un futuro que si tupla.size() de 2 y el string sea de 4
                                                                                            //controlar o error o new de tupla. con el nuevo tamaño
        if(s[i]==','){
            strings++;
        }
    }
    delete data;
    data = new vector<string>(strings,"");
    trocear(elem,strings,s);

    for(int i = 0; i < strings; i++){
        set(i,elem[i]);
    }

}

bool Tupla::match(Tupla p) const{
    if (p.size() != data->size()) { // si las tuplas tienen dif tamaño
        return false;
    }
    bool rep = false;
    int w = 0;
    string palabra = " ";

    struct comodines{
	    string valor; 	    
        string palabra; 	
    };

    comodines Comodin[p.size()];

    // Regex para matchear con comodines
    regex e("\\?[A-Z]");

    for(int i=0;i<p.size();i++) {
        //std::cout << "iter" << i << std::endl;
	    rep = false;
        palabra = data->at(i);

	    if(regex_match(palabra, e)){//if1
	        for(int j = 0;j < w; j++) {
                if(data->at(i) == Comodin[j].valor){
		            if(p.get(i) != Comodin[j].palabra){
			            return false;
                    }
	                rep = true;
                }
            } 
            if(!rep) {
	            Comodin[w].valor = data->at(i);
	            Comodin[w].palabra = p.get(i);
	            w++;
            }
	    } else{
            string stringTemp1 = data->at(i);
            string stringTemp2 = p.get(i);
            if((stringTemp1).compare(stringTemp2) != 0) {//if6
                return false;//
	        }
	    }
    }
    return true;
}
