/****************/
// Mã sinh viên: [ĐIỀN MÃ SINH VIÊN]
// Họ tên:       [ĐIỀN HỌ TÊN]
/****************/
#ifndef DEVICES_H
#define DEVICES_H

#include "TeachingDevice.h"
#include <set>
#include <string>

// Dieu khien may chieu: can pin toi thieu 20% de san sang.
class ProjectorRemote : public TeachingDevice {
private:
    int batteryLevel;
public:
    ProjectorRemote(const std::string& id, const std::string& name,
                    const std::string& location, int battery);
    int GetBatteryLevel() const;
    bool IsReadyForUse() const override;
    double GetDefaultPriorityScore() const override;
};

// Dieu khien dieu hoa: su dung tap phong tuong thich.
class AirConditionerRemote : public TeachingDevice {
private:
    std::set<std::string> compatibleRooms;
public:
    AirConditionerRemote(const std::string& id, const std::string& name,
                         const std::string& location,
                         const std::set<std::string>& rooms);
    bool IsCompatibleWith(const std::string& roomId) const;
    const std::set<std::string>& GetCompatibleRooms() const;
    bool IsReadyForUse() const override;
    double GetDefaultPriorityScore() const override;
};

// Micro khong day: can pin >= 30% va chat luong tin hieu >= 40%.
class WirelessMicrophone : public TeachingDevice {
private:
    int batteryLevel;
    int signalQuality;
public:
    WirelessMicrophone(const std::string& id, const std::string& name,
                       const std::string& location, int battery, int signal);
    int GetBatteryLevel() const;
    int GetSignalQuality() const;
    bool IsReadyForUse() const override;
    double GetDefaultPriorityScore() const override;
};

#endif // DEVICES_H
