#ifndef MSG_H
#define MSG_H

#include "../BNAManager/bnacommontypes.h"
#include "streamtools.h"

#include <QJsonValue>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

namespace imas {

  struct MSGEntry{
    ByteMap map;
    std::u16string data;
  };

  constexpr int32_t msg_count_offset = 0x20;
  constexpr int32_t msg_header_offset = 0x30;

  class MSG
  {
  public:
    MSG();
    void loadFromFile(const std::string &filename);
    void loadFromData(std::vector<char> const& data);
    void saveToFile(const std::string &filename);
    QJsonValue toJson();
    bool fromJson(const QJsonValue &json);
  protected:
    template<class S>
    void openFromStream(S &stream);
    template<class S>
    void saveToStream(S &stream);
  private:
    std::vector<char> m_static_header; //Data coming before string header
    std::vector<MSGEntry> m_entries;
  };
}


#endif // MSG_H
