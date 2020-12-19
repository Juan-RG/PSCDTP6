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
        data->at(i)=t.get(i);
    }
}

Tupla::~Tupla() {
    delete data;
}

int Tupla::size() {
    return data->size();
}

string Tupla::to_string() {
    string res = "[";
    for (int i=0; i<data->size()-1; i++) {
        res = res + data->at(i) + ",";
    }
    res = res + data->at(data->size()-1) + "]";
    return res;
}

string Tupla::get(int pos) {
    return data->at(pos);
}

void Tupla::set(int pos, string value) {
    data->at(pos) = value;
}
void Tupla::from_string(string s) {
    assert(s.length()>2 && s[0]=='[' && s[s.length()-1]==']');
    int strings = 1;
    string s1,s2,s3,s4,s5,s6;
    string buffer;

    for(int i = 0; i<s.length();i++){
        if(s[i]==','){
            strings++;
        }
    }
    //data = new vector<string>(strings,"");

    if(strings == 1){
       sscanf(buffer.c_str(), "[%s]", &s1);
        //this->Tupla.set(0, s1);
        data->at(0) = s1;
        //Tupla(s1);
        //Meter string en data
    }else if(strings == 2){
        int i = sscanf(s.c_str(), "[%s,%s]", &s1, &s2);
        cout<<"asda" << i + "\n";
        //s1 = "hola";
        //s2 = "asda";
        data->at(0) = s1;
        data->at(1) = s2;
        //Meter string en data
    }else if(strings == 3){
        sscanf(s.c_str(), "[%s,%s,%s]", &s1, &s2, &s3);
        data->at(0) = s1;
        data->at(1) = s2;
        data->at(2) = s3;
        //Meter string en data
    }else if(strings == 4){
        sscanf(s.c_str(), "[%s,%s,%s,%s]", &s1, &s2, &s3, &s4);
        data->at(0) = s1;
        data->at(1) = s2;
        data->at(2) = s3;
        data->at(3) = s4;
        //Meter string en data
    }else if(strings == 5){
        sscanf(s.c_str(), "[%s,%s,%s,%s,%s]", &s1, &s2, &s3, &s4, &s5);
        data->at(0) = s1;
        data->at(1) = s2;
        data->at(2) = s3;
        data->at(3) = s4;
        data->at(4) = s5;
        //Meter string en data
    }else if(strings == 6){
        sscanf(s.c_str(), "[%s,%s,%s,%s,%s,%s]", &s1, &s2, &s3, &s4, &s5, &s6);
        data->at(0) = s1;
        data->at(1) = s2;
        data->at(2) = s3;
        data->at(3) = s4;
        data->at(4) = s5;
        data->at(5) = s6;
        //Meter string en data
    }
    
}

bool Tupla::match(Tupla p) {
    bool rep = false;
    int w = 0;
    string palabra;
    struct comodines{
	    string valor; 	    
        string palabra; 	
    };
    comodines Comodin[p.size()];
    for(int i=0;i<p.size();i++){
	    rep = false;
        palabra = data->at(i);
	    if(palabra[0] == '?'){//if1
	        for(int j=0;j<w;j++){
                if(data->at(i) == Comodin[j].valor){
		            if(p.get(i) != Comodin[j].palabra){
			            return false;
                    }
	                rep = true;
                }
            
            } 
            if(!rep){
	            Comodin[w].valor = data->at(i);
	            Comodin[w].palabra = p.get(i);
                w++;
            }
	    } else{
            if(data->at(i) != p.get(i)){//if6
                return false;
	        }   
	    }
    }
}
