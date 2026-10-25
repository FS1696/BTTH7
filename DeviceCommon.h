/****************/
// Mã sinh viên: [ĐIỀN MÃ SINH VIÊN]
// Họ tên:       [ĐIỀN HỌ TÊN]
/****************/
#ifndef DEVICE_COMMON_H
#define DEVICE_COMMON_H

#include <string>
#include <stdexcept>

// Trang thai nghiep vu cua thiet bi.
enum class DeviceStatus {
    Available,
    Borrowed,
    UnderMaintenance
};

std::string StatusToString(DeviceStatus status);
bool IsBlank(const std::string& value);

// Interface chung cho cac doi tuong co ma dinh danh.
class IIdentifiable {
public:
    virtual const std::string& GetId() const = 0;
    virtual ~IIdentifiable() {}
};

class DuplicateIdException : public std::runtime_error {
private:
    std::string duplicateId;
public:
    explicit DuplicateIdException(const std::string& id);
    const std::string& GetDuplicateId() const;
};

class NoSuitableItemException : public std::runtime_error {
public:
    NoSuitableItemException();
};

class DeviceNotReadyException : public std::runtime_error {
private:
    std::string deviceId;
    DeviceStatus currentStatus;
public:
    DeviceNotReadyException(const std::string& id, DeviceStatus status);
    const std::string& GetDeviceId() const;
    DeviceStatus GetCurrentStatus() const;
};

#endif // DEVICE_COMMON_H
