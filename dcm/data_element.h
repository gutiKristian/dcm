#ifndef DCM_DATA_ELEMENT_H_
#define DCM_DATA_ELEMENT_H_

#include <cstdint>

#include "dcm/defs.h"

namespace dcm {

class Visitor;

class DataElement {
public:
  // VR will be queried from the data dictionary.
  DataElement(Tag tag, ByteOrder byte_order = ByteOrder::LE);

  DataElement(Tag tag, VR vr, ByteOrder byte_order = ByteOrder::LE);

  virtual ~DataElement() = default;

  virtual void Accept(Visitor& visitor) const;

  // ---------------------------------------------------------------------------

  Tag tag() const { return tag_; }

  VR vr() const { return vr_; }

  std::uint32_t length() const { return length_; }

  void set_length(std::uint32_t length) { length_ = length; }

  // Get raw value buffer.
  const Buffer& buffer() const { return buffer_; }

  // Set raw value buffer.
  // The buffer size must be even: 2, 4, 8, ...
  bool SetBuffer(Buffer&& buffer);

  // Set byte order and swap bytes for numeric values.
  // Return false if not applicable.
  bool SetByteOrder(ByteOrder byte_order);

  // Calculate the length of the whole element.
  virtual std::uint32_t GetElementLength(VR::Type vr_type,
                                         bool recursively = true) const;

  // ---------------------------------------------------------------------------

  // Get (actual) value multiplicity.
  // To get the expected VM of a tag, use dict::GetVM() instead.
  std::size_t GetVM() const;

  // ---------------------------------------------------------------------------
  // String

  bool GetString(std::string* value) const;

  std::string GetString() const;

  bool GetStringArray(std::vector<std::string>* values) const;

  bool SetString(const std::string& value);

  bool SetStringArray(const std::vector<std::string>& values);

  // ---------------------------------------------------------------------------
  // US (Unsigned Short)

  bool GetUint16(std::uint16_t* value) const {
    return GetNumber(VR::US, value);
  }

  bool SetUint16(std::uint16_t value) {
    return SetNumber(VR::US, value);
  }

  bool GetUint16Array(std::vector<std::uint16_t>* values) const {
    return GetNumberArray(VR::US, values);
  }

  bool SetUint16Array(const std::vector<std::uint16_t>& values) {
    return SetNumberArray(VR::US, values);
  }

  // ---------------------------------------------------------------------------
  // SS (Signed Short)

  bool GetInt16(std::int16_t* value) const {
    return GetNumber(VR::SS, value);
  }

  bool SetInt16(std::int16_t value) {
    return SetNumber(VR::SS, value);
  }

  bool GetInt16Array(std::vector<std::int16_t>* values) const {
    return GetNumberArray(VR::SS, values);
  }

  bool SetInt16Array(const std::vector<std::int16_t>& values) {
    return SetNumberArray(VR::SS, values);
  }

  // ---------------------------------------------------------------------------
  // UL (Unsigned Long)

  bool GetUint32(std::uint32_t* value) const {
    return GetNumber(VR::UL, value);
  }

  bool SetUint32(std::uint32_t value) {
    return SetNumber(VR::UL, value);
  }

  bool GetUint32Array(std::vector<std::uint32_t>* values) const {
    return GetNumberArray(VR::UL, values);
  }

  bool SetUint32Array(const std::vector<std::uint32_t>& values) {
    return SetNumberArray(VR::UL, values);
  }

  // ---------------------------------------------------------------------------
  // SL (Signed Long)

  bool GetInt32(std::int32_t* value) const {
    return GetNumber(VR::SL, value);
  }

  bool SetInt32(std::int32_t value) {
    return SetNumber(VR::SL, value);
  }

  bool GetInt32Array(std::vector<std::int32_t>* values) const {
    return GetNumberArray(VR::SL, values);
  }

  bool SetInt32Array(const std::vector<std::int32_t>& values) {
    return SetNumberArray(VR::SL, values);
  }

  // ---------------------------------------------------------------------------
  // FL (Floating Point Single)

  bool GetFloat32(float32_t* value) const {
    return GetNumber(VR::FL, value);
  }

  bool SetFloat32(float32_t value) {
    return SetNumber(VR::FL, value);
  }

  bool GetFloat32Array(std::vector<float32_t>* values) const {
    return GetNumberArray(VR::FL, values);
  }

  bool SetFloat32Array(const std::vector<float32_t>& values) {
    return SetNumberArray(VR::FL, values);
  }

  // ---------------------------------------------------------------------------
  // FD (Floating Point Double)

  bool GetFloat64(float64_t* value) const {
    return GetNumber(VR::FD, value);
  }

  bool SetFloat64(float64_t value) {
    return SetNumber(VR::FD, value);
  }

  bool GetFloat64Array(std::vector<float64_t>* values) const {
    return GetNumberArray(VR::FD, values);
  }

  bool SetFloat64Array(const std::vector<float64_t>& values) {
    return SetNumberArray(VR::FD, values);
  }

private:
  void DoSetString(const std::string& value);

  template <typename T>
  bool GetNumber(VR vr, T* value) const {
    return GetNumber(vr, sizeof(T), value);
  }

  template <typename T>
  bool SetNumber(VR vr, T value) {
    return SetNumber(vr, sizeof(T), &value);
  }

  template <typename T>
  bool GetNumberArray(VR vr, std::vector<T>* values) const {
    vr_.set_code(vr.code());
    const std::size_t count = GetVM();
    values->resize(count);
    return GetNumberArray(vr, sizeof(T), count, &(*values)[0]);
  }

  template <typename T>
  bool SetNumberArray(VR vr, const std::vector<T>& values) {
    // TODO: What if values is empty?
    return SetNumberArray(vr, sizeof(T), values.size(), &values[0]);
  }

  bool GetNumber(VR vr, std::size_t size, void* value) const;

  bool SetNumber(VR vr, std::size_t size, void* value);

  bool GetNumberArray(VR vr, std::size_t size, std::size_t count,
                      void* values) const;

  bool SetNumberArray(VR vr, std::size_t size, std::size_t count,
                      const void* values);

  void SwapBytes(std::size_t size);

protected:
  // Tag key.
  Tag tag_;

  // Value Representation.
  mutable VR vr_;

  // Little endian or big endian.
  ByteOrder byte_order_;

  // Value length.
  // 0xFFFFFFFF (-1) for SQ element.
  // Identical to the buffer size if the buffer is not empty.
  std::uint32_t length_;

private:
  // Raw buffer (i.e., bytes) of the value.
  Buffer buffer_;
};

}  // namespace dcm

#endif  // DCM_DATA_ELEMENT_H_
