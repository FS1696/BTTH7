/****************/
// Mã sinh viên: [ĐIỀN MÃ SINH VIÊN]
// Họ tên:       [ĐIỀN HỌ TÊN]
/****************/
#ifndef TEACHING_DEVICE_H
#define TEACHING_DEVICE_H

#include "DeviceCommon.h"
#include <string>

// Lop co so truu tuong chua thong tin chung cua moi thiet bi giang day.
class TeachingDevice : public IIdentifiable {
private:
    std::string id;
    std::string name;
    std::string storageLocation;
    DeviceStatus status;

public:
    TeachingDevice(const std::string& deviceId,
                   const std::string& deviceName,
                   const std::string& location);
    virtual ~TeachingDevice() {}

    const std::string& GetId() const override;
    const std::string& GetName() const;
    const std::string& GetStorageLocation() const;
    DeviceStatus GetStatus() const;

    // Hai ham thuan ao de cac lop cu the cai dat da hinh.
    virtual bool IsReadyForUse() const = 0;
    virtual double GetDefaultPriorityScore() const = 0;

    void Borrow();
    void Return();
};

#endif // TEACHING_DEVICE_H
