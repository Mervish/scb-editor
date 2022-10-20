#include "msg.h"

#include <QJsonArray>

namespace  {
  constexpr char padding_literal = 0xCD;
  constexpr auto offset_data_size = 0x10;
}

namespace imas {
MSG::MSG() {}

void MSG::loadFromFile(const std::string &filename) {
  std::ifstream stream(filename, std::ios_base::binary);
  if (!stream.is_open()) {
    return;
  }
  openFromStream(stream);
}

QJsonValue MSG::toJson() {
  QJsonArray msg_json;
  for (auto const &msg : m_entries) {
    msg_json.append(QJsonValue(QString::fromStdU16String(msg.data)));
  }
  return msg_json;
}

bool MSG::fromJson(QJsonValue const &json) {
  auto const array = json["msg"].toArray();
  if (array.size() != m_entries.size()) {
    return false;
  }

  m_entries.clear();
  for (auto const &value : array) {
    m_entries.push_back({.data = value.toString().toStdU16String()});
  }
  return true;
}

void MSG::loadFromData(const std::vector<char> &data) {
  boost::iostreams::array_source asource(data.data(), data.size());
  boost::iostreams::stream<boost::iostreams::array_source> data_stream{asource};
  openFromStream(data_stream);
}

void MSG::saveToFile(const std::string &filename)
{
  std::ofstream stream(filename, std::ios_base::binary);
  if(!stream.is_open()){
    return;
  }
  saveToStream(stream);
}


template <class S> void MSG::openFromStream(S &stream) {
  std::string label;
  label.resize(4);
  stream.read(label.data(), 4);
  stream.seekg(msg_count_offset);
  auto count = imas::tools::readShort(stream);
  m_entries.resize(count);
  stream.seekg(msg_header_offset);
  for (auto &entry : m_entries) {
    entry.map.size = imas::tools::readLong(stream);
    entry.map.offset = imas::tools::readLong(stream);
  }
  imas::tools::evenReadStream(stream);
  // msg uses offsets relative to the zero point.
  // Moreover, zero point is not defined in the header, so i'm just gonna assume
  // it goes straight after header
  size_t const zero_point = stream.tellg();
  for (auto &entry : m_entries) {
    stream.seekg(zero_point + entry.map.offset);
    entry.data.resize(entry.map.size / 2);
    for (auto &wide : entry.data) {
      wide = imas::tools::readShort(stream);
    }
  }
}

template<class S>
void MSG::saveToStream(S &stream)
{
  //Let's fill header
  stream.write("MSG", 3);
  tools::padStream(stream, 0, 12);
  stream.put(0x45);
  tools::padStream(stream, 0, 16);
  //Write string count
  tools::writeShort(stream, m_entries.size()); //string count
  tools::padStream(stream, 0, 4);
  int16_t str_size = std::accumulate(m_entries.begin(), m_entries.end(), 0, [](const auto &a, const auto &b){
    return a + (b.data.size() * 2);
  });
  tools::writeShort(stream, str_size); //string data size
  tools::writeShort(stream, 0x10); //idk what this does
  auto entry_count = m_entries.size();
  int16_t header_size = 16 + (entry_count % 2 ? (entry_count * 8) + 8 : (entry_count * 8));
  tools::writeShort(stream, header_size); //header size
  tools::padStream(stream, 0, 4);
  int32_t text_offset = 0;
  for(auto const& string: m_entries){
    int32_t str_size = string.data.size() * 2;
    tools::writeLong(stream, str_size);
    tools::writeLong(stream, text_offset);
    text_offset += str_size;
  }
  tools::evenWriteStream(stream, padding_literal);
  for(auto const& string: m_entries){
    for (auto const &wide : string.data) {
      tools::writeShort(stream, wide);
    }
  }
  //Finalizing
  tools::evenWriteStream(stream, padding_literal);
  int size = stream.tellp().operator long long() - 32;
  stream.seekp(offset_data_size);
  tools::writeLong(stream, size);
}

} // namespace imas
