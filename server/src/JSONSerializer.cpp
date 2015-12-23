#include "JSONSerializer.hh"

template <typename T>
void    JSONSerializer::addContent(std::string &content,
                               const std::vector<std::pair<std::string, T> > &data) {
  for (uint64_t i = 0; i < data.size(); ++i)
    content += "\""
      + data[i].first
      + "\":"
      + std::to_string(data[i].second)
      + ",";
}

template <>
void    JSONSerializer::addContent<std::string>(std::string &content,
                                            const std::vector<std::pair<std::string, std::string> > &data) {
  for (uint64_t i = 0; i < data.size(); ++i)
    content += "\""
      + data[i].first
      + "\":\""
      + data[i].second
      + "\",";
}

template <typename T>
void    JSONSerializer::addVector(std::string &content,
                              std::vector<std::pair<std::string, std::vector<T> > > &data) {
  for (uint64_t i = 0; i < data.size(); ++i)
    {
      content += "\""
        + data[i].first
        + "\":[";
      for (uint64_t j = 0; j < data[i].second.size(); ++j)
          content += "\""
            + std::to_string(data[i].second[j])
            + "\",";
      content.pop_back();
      content += "],";
    }
}

template <>
void    JSONSerializer::addVector<std::string>(std::string &content,
                                           std::vector<std::pair<std::string, std::vector<std::string> > > &data) {
  for (uint64_t i = 0; i < data.size(); ++i)
    {
      content += "\""
        + data[i].first
        + "\":[";
      for (uint64_t j = 0; j < data[i].second.size(); ++j)
          content += "\""
            + data[i].second[j]
            + "\",";
      content.pop_back();
      content += "],";
    }
}

template <>
void    JSONSerializer::addVector<Entity>(std::string &content,
                                      std::vector<std::pair<std::string, std::vector<Entity> > > &data) {
  for (uint64_t i = 0; i < data.size(); ++i)
    {
      content += "\""
        + data[i].first
        + "\":[";
      for (uint64_t j = 0; j < data[i].second.size(); ++j)
        content += generate(data[i].second[j], "")
          + ",";
      content.pop_back();
      content += "],";
    }
}

std::string     JSONSerializer::generate(Entity &monster, const std::string &name) {
  std::string   content;

  if (!name.empty())
    content += "\""
      + name
      + "\":";
  content += "{";
  try {
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
    for (uint64_t i = 0; i < tmpEntity.size(); ++i)
      content += generate(tmpEntity[i].second, tmpEntity[i].first) + ",";
    content.pop_back();
  } catch (ComponentManagerException) {}
  content += "}";
  return content;
}
