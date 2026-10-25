/****************/
// Mã sinh viên: 202419074
// Họ tên: Vũ Thành Lâm
/****************/
#include "Devices.h"
#include <stdexcept>

ProjectorRemote::ProjectorRemote(const std::string& id,
                                 const std::string& name,
                                 const std::string& location, int battery)
    : TeachingDevice(id, name, location), batteryLevel(battery) {
    if (batteryLevel < 0 || batteryLevel > 100) {
        throw std::invalid_argument(
            "Projector remote battery must be from 0 to 100.");
    }
}

int ProjectorRemote::GetBatteryLevel() const { return batteryLevel; }

bool ProjectorRemote::IsReadyForUse() const {
    return GetStatus() == DeviceStatus::Available && batteryLevel >= 20;
}

double ProjectorRemote::GetDefaultPriorityScore() const {
    return static_cast<double>(batteryLevel);
}

AirConditionerRemote::AirConditionerRemote(
    const std::string& id, const std::string& name,
    const std::string& location, const std::set<std::string>& rooms)
    : TeachingDevice(id, name, location), compatibleRooms(rooms) {}

bool AirConditionerRemote::IsCompatibleWith(const std::string& roomId) const {
    return compatibleRooms.find(roomId) != compatibleRooms.end();
}

const std::set<std::string>&
AirConditionerRemote::GetCompatibleRooms() const {
    return compatibleRooms;
}

bool AirConditionerRemote::IsReadyForUse() const {
    return GetStatus() == DeviceStatus::Available;
}

double AirConditionerRemote::GetDefaultPriorityScore() const {
    // Diem mac dinh: co it nhat mot phong tuong thich thi diem bang 100.
    return compatibleRooms.empty() ? 0.0 : 100.0;
}

WirelessMicrophone::WirelessMicrophone(
    const std::string& id, const std::string& name,
    const std::string& location, int battery, int signal)
    : TeachingDevice(id, name, location),
      batteryLevel(battery), signalQuality(signal) {
    if (batteryLevel < 0 || batteryLevel > 100 ||
        signalQuality < 0 || signalQuality > 100) {
        throw std::invalid_argument(
            "Microphone battery and signal must be from 0 to 100.");
    }
}

int WirelessMicrophone::GetBatteryLevel() const { return batteryLevel; }
int WirelessMicrophone::GetSignalQuality() const { return signalQuality; }

bool WirelessMicrophone::IsReadyForUse() const {
    return GetStatus() == DeviceStatus::Available &&
           batteryLevel >= 30 && signalQuality >= 40;
}

double WirelessMicrophone::GetDefaultPriorityScore() const {
    return (batteryLevel + signalQuality) / 2.0;
}
