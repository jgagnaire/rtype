#include <sstream>
#include <algorithm>
#include "JSONParser.hh"

std::fstream JSONParser::_stream("");
std::string  JSONParser::s_content("");
std::string  JSONParser::_serializedEntity("");

JSONParser::JSONParser() {}

JSONParser::~JSONParser() {}

JSONParser::JSONParser(const JSONParser &jp) {
  if (this != &jp)
    this->_main_entity = jp._main_entity;
}

JSONParser	&JSONParser::operator=(const JSONParser &jp) {
  if (this != &jp)
    this->_main_entity = jp._main_entity;
  return (*this);
}

template <typename T>
void	JSONParser::addContent(std::string &content,
			       const std::vector<std::pair<std::string, T> > &data) {
  for (std::size_t i = 0; i < data.size(); ++i)
    content += "\""
      + data[i].first
      + "\":"
      + std::to_string(data[i].second)
      + ",";
}

template <>
void	JSONParser::addContent<std::string>(std::string &content,
					    const std::vector<std::pair<std::string, std::string> > &data) {
  for (std::size_t i = 0; i < data.size(); ++i)
    content += "\""
      + data[i].first
      + "\":\""
      + data[i].second
      + "\",";
}

template <typename T>
void	JSONParser::addVector(std::string &content,
			      std::vector<std::pair<std::string, std::vector<T> > > &data) {
  for (std::size_t i = 0; i < data.size(); ++i)
    {
      content += "\""
	+ data[i].first
	+ "\":[";
      for (std::size_t j = 0; j < data[i].second.size(); ++j)
	  content += "\""
	    + std::to_string(data[i].second[j])
	    + "\",";
      content.pop_back();
      content += "],";
    }
}

template <>
void	JSONParser::addVector<std::string>(std::string &content,
					   std::vector<std::pair<std::string, std::vector<std::string> > > &data) {
  for (std::size_t i = 0; i < data.size(); ++i)
    {
      content += "\""
	+ data[i].first
	+ "\":[";
      for (std::size_t j = 0; j < data[i].second.size(); ++j)
	  content += "\""
	    + data[i].second[j]
	    + "\",";
      content.pop_back();
      content += "],";
    }
}

template <>
void	JSONParser::addVector<Entity>(std::string &content,
				      std::vector<std::pair<std::string, std::vector<Entity> > > &data) {
  for (std::size_t i = 0; i < data.size(); ++i)
    {
      content += "\""
	+ data[i].first
	+ "\":[";
      for (std::size_t j = 0; j < data[i].second.size(); ++j)
	content += generate(data[i].second[j], "")
	  + ",";
      content.pop_back();
      content += "],";
    }
}

std::string	JSONParser::generate(Entity &monster, const std::string &name) {
  std::string	content;

  if (!name.empty())
    content += "\""
      + name
      + "\":";
  content += "{";
  addContent(content, monster.manager.getAll<int>());
  addContent(content, monster.manager.getAll<float>());
  addContent(content, monster.manager.getAll<std::string>());
  auto vectorInt = monster.manager.getAll<std::vector<int> >();
  addVector(content, vectorInt);
  auto vectorFloat = monster.manager.getAll<std::vector<float> >();
  addVector(content, vectorFloat);
  auto vectorString = monster.manager.getAll<std::vector<std::string> >();
  addVector(content, vectorString);
  auto vectorEntity = monster.manager.getAll<std::vector<Entity> >();
  addVector(content, vectorEntity);
  auto tmpEntity = monster.manager.getAll<Entity>();
  for (std::size_t i = 0; i < tmpEntity.size(); ++i)
    content += generate(tmpEntity[i].second, tmpEntity[i].first) + ",";
  content.pop_back();
  content += "}";
  return content;
}

bool	JSONParser::parseFile(const std::string & pathname) {
  if (JSONParser::_stream.is_open())
    JSONParser::_stream.close();
  JSONParser::_stream.open(pathname, std::fstream::in);
  return (JSONParser::_stream.is_open());
}


bool		JSONParser::eraseDelimiter(std::string &content) {
  if (*content.begin() != ':')
    return (false);
  content.erase(0, 1);
  return (true);
}

bool		JSONParser::isFloat(const std::string &str) {
  bool		find_dot = false;

  for (auto it = str.begin(); it != str.end(); ++it) {
    if (*it == '.' && !find_dot)
      find_dot = true;
    else if (!isNum(*it))
      return (find_dot);
  }
  return (find_dot);
}

bool		JSONParser::isNum(const char c) { return (c >= '0' && c <= '9'); }

float			JSONParser::floatParse(std::string &content) {
  int			val = 0;
  double		stock;
  std::istringstream	is;
  std::string		tmp;
  bool			find_dot = false;

  for (auto it = content.begin(); it != content.end(); ++it) {
    if (*it == '.' && !find_dot)
      find_dot = true;
    else if (!isNum(*it))
      break ;    
    ++val;
  }
  tmp = content.substr(0, val);
  content.erase(0, val);
  is.str(tmp);
  is >> stock;
  return (static_cast<float>(stock));
}

int			JSONParser::valParse(std::string &content) {
  int			val = 0;
  int			stock;
  std::istringstream	is;
  std::string		tmp;

  for (auto it = content.begin(); (it != content.end()) && isNum(*it); ++it)
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
    if (isFloat(content)) {
      entity.manager.add(key, floatParse(content));
      return (Enum::IS_FLOAT);
    }
    if (isNum(*content.begin())) {
      entity.manager.add(key, valParse(content));
      return (Enum::IS_NUM);
    }
  }
  delete this;
  throw (JSONException("Parsing error"));
}
 
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
    entity.manager.add(key, vectorParse<Entity>(content, '{'));
    break ;
  case ('"'):
    entity.manager.add(key, vectorParse<std::string>(content, '"'));
    break ;
  default:
    if (isFloat(content)) {
      entity.manager.add(key, vectorParse<double>(content, '0'));
      break ;
    }
    if (isNum(*content.begin())) {
      entity.manager.add(key, vectorParse<int>(content, '0'));
      break ;
    }
    delete this;
    throw (JSONException("Parsing error"));
  }
}

const std::string	&JSONParser::getContent() { return (JSONParser::s_content); }

JSONParser		*JSONParser::parse(const std::string &str) {
  std::string	        content;
  std::string		tmp;
  JSONParser		*j = new JSONParser();
  Entity		entity;

  if (!str.empty())
    content = str;
  else {
    if (!JSONParser::_stream.is_open())
      throw JSONException("Open failed");
    while (!JSONParser::_stream.eof()) {
      std::getline(JSONParser::_stream, tmp, '\n');
      content += tmp;
    }
  }
  content.erase(std::remove_if(content.begin(), content.end(),
		 [](const char c) {
		   return ((c == ' ') || (c == '\t') || (c == '\n'));
			       }), content.end());
  JSONParser::s_content = content;
  j->hashParse(content, j->_main_entity);
  return (j);
}

Entity			&JSONParser::getEntity() { return(_main_entity); }

Entity			JSONParser::hashParse(std::string &content) {
  Entity		entity;

  return (hashParse(content, entity));
}


Entity			&JSONParser::hashParse(std::string &content,
				      Entity &entity) {
  std::string		tmp;

  while (1) {
    tmp = getKeys(content);
    if (tmp.empty()) {
      delete this;
      throw (JSONException("Parsing error"));
    }
    getValue(entity, tmp, content);
    if (content.empty() || *content.begin() == '}') {
      break ;
    }
    else if (*content.begin() != ',') {
      delete this;
      throw (JSONException("Parsing error"));
    }
    content.erase(0, 1);
  }
  content.erase(0, 1);
  return (entity);
}


char			JSONParser::getchar(const char c) {
  if (c == '{')
    return ('}');
  if (c == '[')
    return (']');
  return (c);
}


void			JSONParser::_getVal(std::vector<Entity> &val,
					    std::string &content) {
  val.push_back(hashParse(content));
}


void			JSONParser::_getVal(std::vector<std::string> &val,
					    std::string &content) {
  val.push_back(stringParse(content));
}


void			JSONParser::_getVal(std::vector<int> &val,
					    std::string &content) {
  val.push_back(valParse(content));
}

void			JSONParser::_getVal(std::vector<double> &val,
					    std::string &content) {
  val.push_back(floatParse(content));
}
