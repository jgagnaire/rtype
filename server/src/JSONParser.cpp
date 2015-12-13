#include <sstream>
#include "JSONParser.hh"
#include "ServerError.hh"

std::fstream JSONParser::_stream("");

JSONParser::JSONParser() {}

JSONParser::~JSONParser() {}

inline
JSONParser::JSONParser(const JSONParser &jp) {
  if (this != &jp)
    this->_main_entity = jp._main_entity;
}

inline
JSONParser	&JSONParser::operator=(const JSONParser &jp) {
  if (this != &jp)
    this->_main_entity = jp._main_entity;
  return (*this);
}

bool	JSONParser::parseFile(const std::string & pathname) {
  if (JSONParser::_stream.is_open())
    JSONParser::_stream.close();
  JSONParser::_stream.open(pathname, std::fstream::in);
  return (JSONParser::_stream.is_open());
}

inline
bool		JSONParser::eraseDelimiter(std::string &content) {
  if (*content.begin() != ':')
    return (false);
  content.erase(0, 1);
  return (true);
}

inline
bool		JSONParser::isAlpha(const char c) { return (c >= '0' && c <= '9'); }

int		JSONParser::valParse(std::string &content) {
  int			val = 0;
  int			stock;
  std::istringstream	is;
  std::string		tmp;

  for (auto it = content.begin(); (it != content.end()) && isAlpha(*it); ++it)
    ++val;
  tmp = content.substr(0, val);
  content.erase(0, val);
  is.str(tmp);
  is >> stock;
  return (stock);
}

Enum::JSON	JSONParser::getValue(Entity &entity, const std::string &key,
				     std::string &content) {
  eraseDelimiter(content);
  switch (*content.begin()) {
  case ('{'):
    content.erase(0, 1);
    entity.manager.add(key, hashParse(content));
    return (Enum::IS_HASH);
  case ('"'):
    entity.manager.add(key, stringParse(content));
    return (Enum::IS_STRING);
  case ('['):
    content.erase(0, 1);
    getValueForArray(entity, key, content);
    return (Enum::IS_TAB);
  default:
    if (isAlpha(*content.begin())) {
      entity.manager.add(key, valParse(content));
      return (Enum::IS_NUM);
    }
  }
  delete this;
  throw (JSONException("Parsing error"));
}

inline 
std::string	JSONParser::stringParse(std::string &content) { return (getKeys(content)); }

std::string	JSONParser::getKeys(std::string &content) {
  int		val = 0;
  std::string	tmp;

  if (*content.begin() != '\"')
    return ("");
  content.erase(0, 1);
  for (auto it = content.begin(); (it != content.end()) && *it != '"'; ++it)
    ++val;
  tmp = content.substr(0, val);
  content.erase(0, val + 1);
  return (tmp);
}

void	JSONParser::getValueForArray(Entity &entity, const std::string &key,
				     std::string &content) {
  switch (*content.begin()) {
  case ('{'):
    entity.manager.add(key, vectorParse<std::vector<Entity> >(content, '{'));
    break ;
  case ('"'):
    entity.manager.add(key, vectorParse<std::string>(content, '"'));
    break ;
  default:
    if (isAlpha(*content.begin())) {
      entity.manager.add(key, vectorParse<int>(content, '0'));
      break ;
    }
    delete this;
    throw (JSONException("Parsing error"));
  }
}

JSONParser		*JSONParser::parse() {
  std::string	        content;
  std::string		tmp;
  JSONParser		*j = new JSONParser();
  Entity		entity;

  if (!JSONParser::_stream.is_open())
    throw JSONException("Open failed");
  while (!JSONParser::_stream.eof()) {
    std::getline(JSONParser::_stream, tmp, '\n');
    content += tmp;
  }
  content.erase(std::remove_if(content.begin(), content.end(),
		 [](const char c) {
		   return ((c == ' ') || (c == '\t') || (c == '\n'));
			       }), content.end());
  j->hashParse(content, j->_main_entity);
  return (j);
}

inline
std::vector<Entity>    	JSONParser::hashParse(std::string &content) {
  std::vector<Entity>	vec_entity;

  return (hashParse(content, vec_entity));
}


std::vector<Entity>    	JSONParser::hashParse(std::string &content,
					      std::vector<Entity> &vec_entity) {
  std::string		tmp;
  Enum::JSON		ret;

  while (1) {
    Entity		entity;

    tmp = getKeys(content);
    if (tmp.empty()) {
      delete this;
      throw (JSONException("Parsing error"));
    }
    ret = getValue(entity, tmp, content);
    vec_entity.push_back(entity);
    if (*content.begin() == '}')
      break ;
    else if (*content.begin() != ',') {
      delete this;
      throw (JSONException("Parsing error"));
    }
    content.erase(0, 1);
  }
  if (ret == Enum::IS_HASH)
    content.erase(0, 1);
  return (vec_entity);
}

inline
char			JSONParser::getchar(const char c) {
  if (c == '{')
    return ('}');
  if (c == '[')
    return (']');
  return (c);
}

inline
void			JSONParser::_getVal(std::vector<std::vector<Entity> > &val,
					    std::string &content) {
  val.push_back(hashParse(content));
}

inline
void			JSONParser::_getVal(std::vector<std::string> &val,
					    std::string &content) {
  val.push_back(stringParse(content));
}

inline
void			JSONParser::_getVal(std::vector<int> &val,
					    std::string &content) {
  val.push_back(valParse(content));
}
