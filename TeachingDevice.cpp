/****************/
// Mã sinh viên: 202419074
// Họ tên: Vũ Thành Lâm
/****************/
#include "TeachingDevice.h"
#include <stdexcept>

TeachingDevice::TeachingDevice(const std::string& deviceId,
                               const std::string& deviceName,
                               const std::string& location)
    : id(deviceId), name(deviceName), storageLocation(location),
      status(DeviceStatus::Available) {
    if (IsBlank(id) || IsBlank(name) || IsBlank(storageLocation)) {
        throw std::invalid_argument(
            "ID, name and storage location must be valid.");
    }
}

const std::string& TeachingDevice::GetId() const { return id; }
const std::string& TeachingDevice::GetName() const { return name; }
const std::string& TeachingDevice::GetStorageLocation() const {
    return storageLocation;
}
DeviceStatus TeachingDevice::GetStatus() const { return status; }

// Chi cho muon neu thiet bi dang san sang theo quy tac cua lop dan xuat.
void TeachingDevice::Borrow() {
    if (!IsReadyForUse()) {
        throw DeviceNotReadyException(id, status);
    }
    status = DeviceStatus::Borrowed;
}

// Chi nhan tra neu thiet bi dang duoc muon.
void TeachingDevice::Return() {
    if (status != DeviceStatus::Borrowed) {
        throw std::logic_error(
            "Cannot return a device that is not borrowed: " + id);
    }
    status = DeviceStatus::Available;
}
