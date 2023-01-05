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
  //Other modules should ignore size and offset, instead manipulating with 'data';
  char label[4];
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
  void rebuild();
  MSG &msg_data();

private:
  template<class S> void openFromStream(S &stream);
  template<class S>  void saveToStream(S &stream);
  void updateSectionData();
  std::array<char, 0x5C> m_header_cache;
  ScbData m_sections;
  std::vector<ScbSection *> m_sections_agg = {
      &m_sections.CMD, &m_sections.LBL, &m_sections.MSG, &m_sections.VCN,
      &m_sections.LBN, &m_sections.RSC, &m_sections.RSN};
  //Custom data parsers
  MSG m_msg_data;
};
} // namespace imas

#endif // SCB_H
