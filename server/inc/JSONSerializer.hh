#ifndef		JSONSERIALIZER_HH_
# define	JSONSERIALIZER_HH_

# include	<string>
# include	<vector>
# include	"Entity.hh"

class JSONSerializer
{
public:
  JSONSerializer() {}
  ~JSONSerializer() {}

  static std::string generate(Entity &, std::string const &);

private:

  template <typename T>
  static void  addContent(std::string &content,
                          const std::vector<std::pair<std::string, T> > &data);
  template <typename T>
  static void   addVector(std::string &content,
                          std::vector<std::pair<std::string, std::vector<T> > > &data);
};

#endif		// !JSONSERIALIZER_HH_
