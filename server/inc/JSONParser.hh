#pragma once
# include <map>
# include <fstream>
# include <iostream>
# include "Entity/Entity.hh"
# include "Enum.hh"

class JSONParser {
public:
  JSONParser();
  ~JSONParser();
  JSONParser(const JSONParser &);
  JSONParser	&operator=(const JSONParser &);
  static bool		parseFile(const std::string &);
  static JSONParser	*parse();
  
private:
  bool			eraseDelimiter(std::string &);
  bool			isAlpha(const char c);
  std::string		getKeys(std::string &);
  std::string		stringParse(std::string &);
  int			valParse(std::string &);
  std::vector<Entity>	hashParse(std::string &);
  std::vector<Entity>	hashParse(std::string &, std::vector<Entity> &);
  void			getValueForArray(Entity &, const std::string &,
					 std::string &);
  Enum::JSON		getValue(Entity &, const std::string &,
				 std::string &);
  void			_getVal(std::vector<std::vector<Entity> > &val,
				std::string &content);
  void			_getVal(std::vector<std::string> &val,
				std::string &content);
  void			_getVal(std::vector<int> &val,
				std::string &content);
  char		        getchar(const char);

  template <typename T>
  void	        getValue(std::vector<T> &val,
			 std::string &content, const char c = '\0') {
    if (((c != '\0') && (*content.begin() != c)) &&
	(!((c == '0') && isAlpha(*content.begin())))) {
      delete this;
      throw (JSONException("Parsing error"));
    }
    switch (*content.begin()) {
    case ('{'):
      content.erase(0, 1);
      _getVal(val, content);
      break ;
    case ('"'):
      _getVal(val, content);
      break ;
    default:
      if (isAlpha(*content.begin())) {
	_getVal(val, content);
	break ;
      }
      delete this;
      throw (JSONException("Parsing error"));
    }
  }

  template<typename T>
  std::vector<T>	        vectorParse(std::string &content, const char c) {
    std::vector<T>		vec;

    
    while (1) {
      getValue(vec, content, c);
      if (*content.begin() == getchar(c))
	content.erase(0, 1);
      if (*content.begin() == ']')
	break ;
      else if ((*content.begin() != ',')) {
	delete this;
	throw (JSONException("Parsing error"));
      }
      content.erase(0, 1);
    }
    content.erase(0, 1);
    return (vec);
  }


  static std::fstream	_stream;
  std::vector<Entity>	_main_entity;
};
