#include "scb.h"

#include "streamtools.h"
#include <QDebug>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <filesystem>
#include <fstream>
#include <ranges>

namespace {
constexpr int32_t offset_name = 0x30;
constexpr int32_t offset_sections = 112;
constexpr char padding_literal = 0xCC;
//constexpr int32_t msg_offset = 148;
constexpr auto offset_data_size = 0x10;
} // namespace

namespace imas {
SCB::SCB() {}

void SCB::loadFromFile(const std::string &filename) {
  std::ifstream stream(filename, std::ios_base::binary);
  if (!stream.is_open()) {
    return;
  }
  openFromStream(stream);
}

void SCB::loadFromData(const std::vector<char> &data) {
  boost::iostreams::array_source asource(data.data(), data.size());
  boost::iostreams::stream<boost::iostreams::array_source> data_stream{asource};
  openFromStream(data_stream);
}

MSG &SCB::msg_data() { return m_msg_data; }

template <class S> void SCB::openFromStream(S &stream) {
  stream.seekg(offset_sections);

  ScbData sections;
  std::vector<ScbSection *> sections_agg = {
      &sections.CMD, &sections.LBL, &sections.MSG, &sections.VCN,
      &sections.LBN, &sections.RSC, &sections.RSN};
  for (auto section : sections_agg) {
    stream.ignore(4);
    section->size = imas::tools::readLong(stream);
    section->offset = imas::tools::readLong(stream);
    stream.ignore(4);
  }
  std::ranges::sort(sections_agg, [](ScbSection *left, ScbSection *right) {
    return left->offset < right->offset;
  });
  for (auto section : sections_agg) {
    stream.seekg(section->offset);
    section->data.resize(section->size);
    stream.read(section->data.data(), section->size);
  }

  /*std::filesystem::path file_out(filename);
  file_out.replace_extension(".msg");
  std::ofstream msg_strem(file_out, std::ios_base::binary);
  msg_strem.write(sections.MSG.data.data(), sections.MSG.size);*/
  m_msg_data.loadFromData(sections.MSG.data);

  qInfo() << "This is it.";
}

void SCB::saveToFile(const std::string &filename)
{
  std::ofstream stream(filename, std::ios_base::binary);
  if(!stream.is_open()){
    return;
  }
  saveToStream(stream);
}

template<class S>
void SCB::saveToStream(S &stream)
{
  //Let's fill header
  stream.write("SCB", 3);
  tools::padStream(stream, 0, 8);
  stream.put(0x45);
  tools::padStream(stream, 0, 3);
  stream.put(0x45);
  tools::padStream(stream, 0, 16);

  //Finalizing
  tools::evenWriteStream(stream, padding_literal);
  int size = stream.tellp().operator long long() - 32;
  stream.seekp(offset_data_size);
  tools::writeLong(stream, size);
}

} // namespace imas
