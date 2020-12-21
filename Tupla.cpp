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

Tupla::Tupla(const Tupla& t) {
    data = new vector<string>(t.data->size(),"");
    for(int i = 0;i<t.data->size();i++){
        data->at(i)=t.data->at(i);
    }
}
bool Tupla::operator<(const Tupla b) const
{
    return (this->data->at(0) < b.data->at(0));
    //if (this->data->at(0) == b.data->at(0)){
    //    return false;
    //}
    //return true;
    //return this->data->size() > b.data->size();
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
    for(int i = 0; i<s.length();i++){                                                       //controlar en un futuro que si tupla.size() de 2 y el string sea de 4
                                                                                            //controlar o error o new de tupla. con el nuevo tamaño
        if(s[i]==','){
            strings++;
        }
    }

    // TODO: Ahora que ya sabemos como machacar data y ponerle un nuevo size, se puede reescribir from_string más compacta
    //       y evitar tener que adivinar el tamaño de la tupla al hacer from_string (se usaría una plantilla con tamaño
    //       por defecto que luego se modificaría)
    delete data;
    data = new vector<string>(strings,"");
    cout << "Ahora data es de tamaño " << data->size() << endl;

    //if(strings != this->size()){}
    //data = new vector<string>(strings,"");

    if(strings == 1){
        stringstream s_stream(s); //create string stream from the string
        string substr;
        getline(s_stream, substr, '['); //get first string delimited by comma
        getline(s_stream, substr, ','); //get first string delimited by comma
        //this->ip_server_1 = substr;
        s1 = substr;
        s1[s1.length() - 1] = '\0';
        data->at(0) = s1;

        //set(0, "s1");
        //Tupla(s1);
        //Meter string en data
    }else if (strings == 2){
        //scanf(s.c_str(), "[%s,%s]", &s1, &s2);
        stringstream s_stream(s); //create string stream from the string
        string substr;
        getline(s_stream, substr, '['); //get first string delimited by comma
        getline(s_stream, substr, ','); //get first string delimited by comma
        //this->ip_server_1 = substr;
        s1 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s2 = substr;
        s2[s2.length() - 1] = '\0';
        std::cout << s1 << std::endl;
        std::cout << s2 << std::endl;
        data->at(0) = s1;
        data->at(1) = s2;
        //std::cout << s1 << std::endl;
        //std::cout << s2 << std::endl;
        //set(0, "s1");
        //set(1, "s2");
        //data->at(0) = s1;
        //data->at(1) = s2;
        //Meter string en data
    }else if(strings == 3){
        stringstream s_stream(s); //create string stream from the string
        string substr;
        getline(s_stream, substr, '['); //get first string delimited by comma
        getline(s_stream, substr, ','); //get first string delimited by comma
        //this->ip_server_1 = substr;
        s1 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s2 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s3 = substr;
        s3[s3.length() - 1] = '\0';
        std::cout << s1 << std::endl;
        std::cout << s2 << std::endl;
        std::cout << s3 << std::endl;
        data->at(0) = s1;
        data->at(1) = s2;
        data->at(2) = s3;
        //Meter string en data
    }else if(strings == 4){
        //sscanf(s.c_str(), "[%s,%s,%s,%s]", &s1, &s2, &s3, &s4);
        stringstream s_stream(s); //create string stream from the string
        string substr;
        getline(s_stream, substr, '['); //get first string delimited by comma
        getline(s_stream, substr, ','); //get first string delimited by comma
        //this->ip_server_1 = substr;
        s1 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s2 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s3 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s4 = substr;
        s4[s4.length() - 1] = '\0';
        std::cout << s1 << std::endl;
        std::cout << s2 << std::endl;
        std::cout << s3 << std::endl;
        std::cout << s4 << std::endl;

        data->at(0) = s1;
        data->at(1) = s2;
        data->at(2) = s3;
        data->at(3) = s4;
        //Meter string en data
    }else if(strings == 5){
        //sscanf(s.c_str(), "[%s,%s,%s,%s,%s]", &s1, &s2, &s3, &s4, &s5);
        stringstream s_stream(s); //create string stream from the string
        string substr;
        getline(s_stream, substr, '['); //get first string delimited by comma
        getline(s_stream, substr, ','); //get first string delimited by comma
        //this->ip_server_1 = substr;
        s1 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s2 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s3 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s4 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s5 = substr;
        s5[s5.length() - 1] = '\0';
        std::cout << s1 << std::endl;
        std::cout << s2 << std::endl;
        std::cout << s3 << std::endl;
        std::cout << s4 << std::endl;
        std::cout << s5 << std::endl;
        data->at(0) = s1;
        data->at(1) = s2;
        data->at(2) = s3;
        data->at(3) = s4;
        data->at(4) = s5;
        //Meter string en data
    }else if(strings == 6){
        //sscanf(s.c_str(), "[%s,%s,%s,%s,%s,%s]", &s1, &s2, &s3, &s4, &s5, &s6);
        stringstream s_stream(s); //create string stream from the string
        string substr;
        getline(s_stream, substr, '['); //get first string delimited by comma
        getline(s_stream, substr, ','); //get first string delimited by comma
        //this->ip_server_1 = substr;
        s1 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s2 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s3 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s4 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s5 = substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        s6 = substr;
        s6[s6.length() - 1] = '\0';
        std::cout << s1 << std::endl;
        std::cout << s2 << std::endl;
        std::cout << s3 << std::endl;
        std::cout << s4 << std::endl;
        std::cout << s5 << std::endl;
        std::cout << s6 << std::endl;
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
    std::cout << "vars inicializadas" << std::endl;


    for(int i=0;i<p.size();i++) {
        std::cout << "iter" << i << std::endl;
	    rep = false;
        palabra = data->at(i);

	    if(palabra[0] == '?'){//if1
            cout<< "if\n";
	        std::cout << "palabra[0] == '?'" << std::endl;
	        for(int j = 0;j < w; j++) {
	            std::cout << "vuelta en comprobacion comodines" << std::endl;
                if(data->at(i) == Comodin[j].valor){
		            if(p.get(i) != Comodin[j].palabra){
                        std::cout << "retorno" << std::endl;
			            return false;
                    }
		            cout<<"paso aa \n";
	                rep = true;
                }
            
            } 
            if(!rep) {
                std::cout << "no es repetido, w= " << w  << "i= " << i << std::endl;
	            Comodin[w].valor = data->at(i);
	            Comodin[w].palabra = p.get(i);
                std::cout << "hola?" << std::endl;
	            w++;
            }
	    } else{
	        cout<< "else\n";
            std::cout << "palabra[0] != '?'" << std::endl;

            std::cout << data->at(i) << std::endl;
            std::cout << p.get(i) << std::endl;
            std::cout << "....." << std::endl;

            string stringTemp1 = data->at(i);
            string stringTemp2 = p.get(i);

            std::cout << stringTemp1.length() << std::endl;
            std::cout << stringTemp2.length() << std::endl;

            if((stringTemp1).compare(stringTemp2) != 0) {//if6
                std::cout << "retorno" << std::endl;
                return false;//
	        }
	    }
    }
    return true;
}
