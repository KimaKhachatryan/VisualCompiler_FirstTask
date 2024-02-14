#ifndef ANALYZER_H
#define ANALYZER_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

struct Value {
	std::string type;
	std::string value;
	std::string name;
	size_t line;
	size_t pos;
};

class analyzer {
public:
    //ctor and dtor
    explicit analyzer(char* fileName);
    ~analyzer();

//FUNCTIONALITY

    //writes into typeinfo file all posible declarations
    void write_typeinfo();

    //reads given file
    void read_file();
    
    //writes the all information of result container into the result file  
    void write_result(); 

    //function call operator
    void operator() ();

//HELPERS
    
    //tokenizes string and saves it in all_text and result if needed
    void save_string();

    //first of all string(which contains all one line) tokenized by semicolon
    std::vector<std::string> tokenizeBySemicolon(std::string str);

    //string which tokenized by semicolon then tokenized by space 
    std::vector<std::string> tokenizeBySpace(std::string str);

    //this function creats Value type objects from declarations and saves in containers
    //if string is not declaration, object saves  in result container also
    void make_object(std::vector<std::string> vec, size_t position);

    //checks if the token is keyword saved in symbol_table
    bool is_keyword(std::string str);

    //returns an iterator to the object which name is given string
    std::vector<Value>::iterator find_in_all_text(std::string str);

 private:
    size_t line_number;
    std::string m_str;
    std::ifstream read;
    std::vector<std::string> symbol_table;
    std::vector<Value> all_text; 
    std::vector<Value> result;   

};

#include "analyzer.hpp"

#endif
