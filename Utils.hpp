#ifndef Utils_hpp
#define Utils_hpp

#include <iostream>
#include <vector>
#include <cctype>

#define RESET "\e[0m"
#define RED "\e[31m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"
#define BLUE "\e[34m"
#define PURPLE "\e[35m"
#define CYAN "\e[36m"
#define GRAY "\e[37m"


std::vector<std::string> split(std::string str, std::string delimiter);
bool    isAlphaStr(std::string str);
bool	isDigitStr(std::string str);
bool    contains( std::vector<std::string> vect, std::string str );
void	eraseString( std::vector<std::string> & vect, std::string name );
void    eraseUser( std::vector<User*> & vect, std::string name );

#endif