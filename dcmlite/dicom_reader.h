#ifndef DCMLITE_DICOM_READER_H_
#define DCMLITE_DICOM_READER_H_

#include <cstdint>
#include <string>
#include <vector>

#include "boost/filesystem/path.hpp"

#include "dcmlite/defs.h"

namespace dcmlite {

class DataElement;
class DataSet;
class Reader;
class ReadHandler;
class Tag;

// DICOM reader reads data set from a DICOM file.
class DicomReader {
public:
  explicit DicomReader(ReadHandler* handler);

  ~DicomReader() = default;

  // Read a DICOM file.
  bool ReadFile(const boost::filesystem::path& path);

private:
  bool DoRead(Reader& reader);

  // Read data element sequentially from the file.
  // \param max_length Maximum value length to read for the current data set.
  //        Could be kUndefinedLength (0xFFFFFFFF).
  // \param check_endian Check endian type during the reading.
  // \return The length read.
  std::uint32_t Read(Reader& reader, std::size_t max_length, bool check_endian);

  bool ReadTag(Reader& file, Tag* tag);
  bool ReadUint16(Reader& file, std::uint16_t* value);
  bool ReadUint32(Reader& file, std::uint32_t* value);

  // Reverse the byte order if endian types are different.
  void AdjustBytesUint16(std::uint16_t& value) const;
  void AdjustBytesUint32(std::uint32_t& value) const;

private:
  ReadHandler* handler_;

  // Endian type of DICOM file.
  Endian endian_;

  // Explicit or implicit VR.
  bool explicit_vr_;
};

}  // namespace dcmlite

#endif  // DCMLITE_DICOM_READER_H_
