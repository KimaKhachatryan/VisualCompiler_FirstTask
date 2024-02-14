#ifndef FUNCTION_HPP
#define FUNCTIONS_HPP

analyzer::analyzer(char* fileName)
{
    read.open(fileName);

    //check if file opened correctly
	if (!read.is_open()) {
		std::cout << "Something is wrong...((" << std::endl;
		std::cout << "Does the file not exist?"
			  << std::endl;
		exit(0); 
	}

    symbol_table = {"const", "unsigned", "signed", "short", "long", "int", "float", "double", "char", "bool"};

    line_number = 1;
}


analyzer::~analyzer()
{
    read.close();
}

void analyzer::read_file()
{
	//read all text file
	while (!read.eof()) {
		std::getline(read, m_str);
		
		//checks if string contains only white spaces
		if (m_str.find_first_not_of(" \t\n\v\f\r") == std::string::npos) {
			++line_number;
			continue;
		}

		//tokenizes string and saves it in all_text and result if needed
		save_string();
		++line_number;	
	}
}

void analyzer::write_typeinfo()
{
    std::ofstream write;

    write.open("TypeInfo.txt");

	if (!write.is_open()) {
		std::cout << "Something is wrong...((" << std::endl;
		std::cout << "Does the file not exist?"
				  << std::endl;
		exit(0); 
	}

    write << "int val;" << "\n"
          << "unsigned val;" << "\n"
          << "signed val;" << "\n"
          << "const signed val;" << "\n"
          << "const unsigned val;" << "\n"
          << "unsigned int val;" << "\n"
          << "signed int val;" << "\n"
          << "const unsigned int val;" << "\n"
          << "const signed int val;" << "\n"
          << "short val;" << "\n"
          << "const short val;" << "\n"
          << "unsigned short int val;" << "\n"
          << "unsigned short val;" << "\n"
          << "const unsigned short int val;" << "\n"
          << "const unsigned short val;" << "\n"
          << "long val;" << "\n"
          << "const long val;" << "\n"
          << "unsigned long int val;" << "\n"
          << "unsigned long val;" << "\n"
          << "const unsigned long int val;" << "\n"
          << "const unsigned long val;" << "\n"
          << "long long val;" << "\n"
          << "const long long val;" << "\n"
          << "unsigned long long int val;" << "\n"
          << "unsigned long long val;" << "\n"
          << "const unsigned long long int val;" << "\n"
          << "const unsigned long long val;" << "\n"
          << "unsigned char val;" << "\n"
          << "signed char val;" << "\n"
          << "const unsigned char val;" << "\n"
          << "const signed char val;" << "\n"
          << "float val;" << "\n"
          << "const float val;" << "\n"
          << "doubl val;" << "\n"
          << "const doubl val;" << "\n"
          << "long doubl val;" << "\n"
          << "const long doubl val;" << "\n"
          << "long long doubl val;" << "\n"
          << "const long long doubl val;" << "\n"
          << "bool val;" << "\n"
          << "const bool val;" << std::endl;


    write.close();

}

void analyzer::write_result()
{
    std::ofstream write;

    write.open("result.txt");

	if (!write.is_open()) {
		std::cout << "Something is wrong...((" << std::endl;
		std::cout << "Does the file not exist?"
				  << std::endl;
		exit(0); 
	}

    write << std::setw(5) << std::internal << "Type"
          << std::setw(20) << std::internal << "Value"
          << std::setw(20) << std::internal << "Name"
          << std::setw(20) << std::internal <<  "Line"
          << std::setw(20) << std::internal << "Position" << std::endl;

    for (size_t i = 0; i < result.size(); ++i) {
        write << std::setw(5) << std::internal << result[i].type
          << std::setw(20) << std::internal << result[i].value
          << std::setw(20) << std::internal << result[i].name
          << std::setw(20) << std::internal << result[i].line
          << std::setw(20) << std::internal << result[i].pos << std::endl;
    }


    write.close();

}

//function call operator
void analyzer::operator() ()
{
    write_typeinfo();
    read_file();
    write_result(); 
}

void analyzer::save_string()
{ 
    std::vector<std::string> vec;

    //tokenizes by semicolon and save into temporary vector
    vec = tokenizeBySemicolon(m_str);

    //iterates over the vector and tokenizes it by space
    //then passes small tokenized vector to make_object
    for(size_t i = 0; i < vec.size(); ++i) {
        make_object (tokenizeBySpace(vec[i]), i);
    }
}


std::vector<std::string> analyzer::tokenizeBySemicolon(std::string str) 
{
    std::vector<std::string> vec;
    std::string small_str;
    std::istringstream ss(str); 

    while (std::getline(ss, small_str, ';')) {
        vec.push_back(small_str);
    }

    return vec;
}

std::vector<std::string> analyzer::tokenizeBySpace(std::string str) 
{
    std::vector<std::string> vec;
    std::string token;
    std::istringstream ss(str); 

	while (ss >> token) {
		vec.push_back (token); 
	}


    // If the stringstream has reached the end and there's no error, returns vector
    if (ss.eof() && !ss.fail()) {
        return vec;
    } else {
        // If there's an error, clears the flags and then returns vector
        ss.clear();
        return vec;
    }

    return vec;
}

void analyzer::make_object(std::vector<std::string> vec, size_t position)
{
    Value object;

    //if tokenized string is declaration creates object from it and saves into an all_text
    if (is_keyword(vec[0])) {
        std::string type;
        size_t i = 0;

        //creates type short abbreviation
        while (is_keyword(vec[i])) {
            type += vec[i][0];
            
            if (vec[i] == "short") {
                type += 'h';
            }

            ++i;
        } 

        object.type = type;
        object.name = vec[i];
        object.line = line_number;
        object.pos = position;

        //if value is not initialized its value is Garbage
        if (vec.size() == i + 1) {
            object.value = "GARBAGE";
        } else {
            object.value = vec[i + 2];
        }
    //if string is not declaration 
    //find_in_all_text function searches in all_text whether its contains object with given name
    //if the object exists it is copied into result
    //if not is created locally
    } else {
        auto it = find_in_all_text(vec[0]);
        if (it != all_text.end()) {
            result.push_back(*it);
        } else {
            object.type = "-";
            object.name = vec[0];
            object.line = line_number;
            object.pos = position;
            object.value = "Garbage";
            result.push_back(object);
        }
    }

    all_text.push_back(object);
} 

bool analyzer::is_keyword(std::string str)
{
    for (std::vector<std::string>::iterator it = symbol_table.begin(); it != symbol_table.end(); ++it) {
        if (*it == str) {
            return true;
        }
    }

    return false;
}

std::vector<Value>::iterator analyzer::find_in_all_text(std::string str)
{
    for (size_t i = 0; i < all_text.size(); ++i) {
        if (all_text[i].name == str) {
            return all_text.begin() + i;
        }
    }

    return all_text.end();
}



#endif
