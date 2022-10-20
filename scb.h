#ifndef SCB_H
#define SCB_H

#include <string>
#include <vector>

#include <QJsonArray>
#include <QJsonObject>

#include "msg.h"

namespace imas {
struct ScbSection {
  int32_t size = 0;
  int32_t offset = 0;
  std::vector<char> data;
};

struct ScbData {
  ScbSection CMD;
  ScbSection LBL;
  ScbSection MSG;
  ScbSection VCN;
  ScbSection LBN;
  ScbSection RSC;
  ScbSection RSN;
};

class SCB {
public:
  SCB();
  void loadFromFile(const std::string &filename);
  void loadFromData(std::vector<char> const &data);
  void saveToFile(const std::string &filename);
  MSG &msg_data();

private:
  template <class S> void openFromStream(S &stream);
  template<class S>  void saveToStream(S &stream);
  MSG m_msg_data;
};
} // namespace imas

#endif // SCB_H
