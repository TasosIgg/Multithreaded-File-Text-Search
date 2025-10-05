#include <vector>
#include <utility> // for pair
#include <string>  

std::vector<std::pair<int, std::string>>
searchFileCollect(const std::string& filePath,
                  const std::string& keyword,
                  bool useRegex,
                  bool caseInsensitive);
