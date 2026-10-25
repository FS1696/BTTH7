/****************/
// Mã sinh viên: [ĐIỀN MÃ SINH VIÊN]
// Họ tên:       [ĐIỀN HỌ TÊN]
/****************/
#include "DeviceCommon.h"

// Chuyen enum trang thai thanh chuoi de hien thi ket qua.
std::string StatusToString(DeviceStatus status) {
    switch (status) {
        case DeviceStatus::Available: return "Available";
        case DeviceStatus::Borrowed: return "Borrowed";
        case DeviceStatus::UnderMaintenance: return "UnderMaintenance";
        default: return "Unknown";
    }
}

// Kiem tra chuoi rong hoac chi gom khoang trang.
bool IsBlank(const std::string& value) {
    return value.find_first_not_of(" \t\r\n") == std::string::npos;
}

DuplicateIdException::DuplicateIdException(const std::string& id)
    : std::runtime_error("Duplicate device ID: " + id), duplicateId(id) {}

const std::string& DuplicateIdException::GetDuplicateId() const {
    return duplicateId;
}

NoSuitableItemException::NoSuitableItemException()
    : std::runtime_error("No suitable device found.") {}

DeviceNotReadyException::DeviceNotReadyException(
    const std::string& id, DeviceStatus status)
    : std::runtime_error("Device is not ready: " + id),
      deviceId(id), currentStatus(status) {}

const std::string& DeviceNotReadyException::GetDeviceId() const {
    return deviceId;
}

DeviceStatus DeviceNotReadyException::GetCurrentStatus() const {
    return currentStatus;
}
