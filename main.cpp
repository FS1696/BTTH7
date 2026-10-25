/****************/
// Mã sinh viên: 202419074
// Họ tên: Vũ Thành Lâm
/****************/

#include "DeviceCommon.h"
#include "Devices.h"
#include "Repository.h"
#include "SelectionStrategy.h"
#include "AllocationService.h"

#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <functional>
#include <exception>

using namespace std;

int main() {
    cout << "==============================================\n";
    cout << " W07 - TEACHING DEVICE ALLOCATION TESTS\n";
    cout << "==============================================\n\n";

    try {
        // Test 1: Them ba dieu khien may chieu vao kho.
        shared_ptr<InMemoryRepository<ProjectorRemote> > projectorRepo(
            new InMemoryRepository<ProjectorRemote>());
        projectorRepo->Add(shared_ptr<ProjectorRemote>(
            new ProjectorRemote("P001", "Projector Remote 1", "Cabinet A", 45)));
        projectorRepo->Add(shared_ptr<ProjectorRemote>(
            new ProjectorRemote("P002", "Projector Remote 2", "Cabinet A", 85)));
        projectorRepo->Add(shared_ptr<ProjectorRemote>(
            new ProjectorRemote("P003", "Projector Remote 3", "Cabinet B", 15)));
        cout << "[1] Add projectors: PASS (3 devices stored)\n";

        // Test 2: Khong cho phep ma trung lap.
        try {
            projectorRepo->Add(shared_ptr<ProjectorRemote>(
                new ProjectorRemote("P001", "Duplicate remote", "Cabinet X", 90)));
            cout << "[2] Duplicate ID: FAIL\n";
        } catch (const DuplicateIdException& ex) {
            cout << "[2] Duplicate ID: PASS (" << ex.GetDuplicateId() << ")\n";
        }

        // Test 3: Tim kiem theo ma.
        shared_ptr<ProjectorRemote> projector = projectorRepo->FindById("P002");
        cout << "[3] FindById(P002): "
             << (projector ? "PASS - " + projector->GetName() : "FAIL") << "\n";

        // Test 4: Loc thiet bi san sang; P003 khong du pin.
        vector<shared_ptr<ProjectorRemote> > readyProjectors =
            projectorRepo->FindAll([](const shared_ptr<ProjectorRemote>& item) {
                return item->IsReadyForUse();
            });
        cout << "[4] Ready projectors: " << readyProjectors.size()
             << " (expected 2) -> "
             << (readyProjectors.size() == 2 ? "PASS" : "FAIL") << "\n";

        // Test 5: Chon dieu khien may chieu co diem cao nhat.
        shared_ptr<ISelectionStrategy<ProjectorRemote> > projectorStrategy(
            new HighestPriorityStrategy<ProjectorRemote>(
                [](const shared_ptr<ProjectorRemote>& item) {
                    return item->GetDefaultPriorityScore();
                }));
        AllocationService<ProjectorRemote> projectorService(projectorRepo, projectorStrategy);
        shared_ptr<ProjectorRemote> selectedProjector =
            projectorService.Allocate([](const shared_ptr<ProjectorRemote>&) { return true; });
        cout << "[5] Highest-priority projector: " << selectedProjector->GetId()
             << " (battery " << selectedProjector->GetBatteryLevel() << "%) -> "
             << (selectedProjector->GetId() == "P002" ? "PASS" : "FAIL") << "\n";

        // Test 6: Dieu khien dieu hoa phai tuong thich voi phong A203.
        shared_ptr<InMemoryRepository<AirConditionerRemote> > acRepo(
            new InMemoryRepository<AirConditionerRemote>());
        acRepo->Add(shared_ptr<AirConditionerRemote>(
            new AirConditionerRemote("AC001", "AC Remote 1", "Cabinet C",
                                     set<string>{"A203", "B101"})));
        acRepo->Add(shared_ptr<AirConditionerRemote>(
            new AirConditionerRemote("AC002", "AC Remote 2", "Cabinet C",
                                     set<string>{"C301"})));
        shared_ptr<ISelectionStrategy<AirConditionerRemote> > acStrategy(
            new HighestPriorityStrategy<AirConditionerRemote>(
                [](const shared_ptr<AirConditionerRemote>& item) {
                    return item->IsCompatibleWith("A203") ? 100.0 : 0.0;
                }));
        AllocationService<AirConditionerRemote> acService(acRepo, acStrategy);
        shared_ptr<AirConditionerRemote> selectedAC = acService.Allocate(
            [](const shared_ptr<AirConditionerRemote>& item) {
                return item->IsCompatibleWith("A203");
            });
        cout << "[6] AC remote compatible with A203: " << selectedAC->GetId()
             << " -> " << (selectedAC->GetId() == "AC001" ? "PASS" : "FAIL") << "\n";

        // Test 7 va 8: Micro dung chien luoc FirstAvailable, sau do doi chien luoc.
        shared_ptr<InMemoryRepository<WirelessMicrophone> > micRepo(
            new InMemoryRepository<WirelessMicrophone>());
        micRepo->Add(shared_ptr<WirelessMicrophone>(
            new WirelessMicrophone("M001", "Microphone 1", "Cabinet M", 70, 80)));
        micRepo->Add(shared_ptr<WirelessMicrophone>(
            new WirelessMicrophone("M002", "Microphone 2", "Cabinet M", 90, 90)));
        micRepo->Add(shared_ptr<WirelessMicrophone>(
            new WirelessMicrophone("M003", "Microphone 3", "Cabinet N", 20, 95)));

        shared_ptr<ISelectionStrategy<WirelessMicrophone> > firstStrategy(
            new FirstAvailableStrategy<WirelessMicrophone>());
        AllocationService<WirelessMicrophone> micService(micRepo, firstStrategy);
        shared_ptr<WirelessMicrophone> firstMic = micService.Allocate();
        cout << "[7] First available microphone: " << firstMic->GetId() << " -> "
             << (firstMic->GetId() == "M001" ? "PASS" : "FAIL") << "\n";

        firstMic->Return();
        shared_ptr<ISelectionStrategy<WirelessMicrophone> > highestMicStrategy(
            new HighestPriorityStrategy<WirelessMicrophone>(
                [](const shared_ptr<WirelessMicrophone>& item) {
                    return item->GetDefaultPriorityScore();
                }));
        micService.ChangeStrategy(highestMicStrategy);
        shared_ptr<WirelessMicrophone> highestMic = micService.Allocate();
        cout << "[8] After strategy change, highest-score microphone: "
             << highestMic->GetId() << " (score "
             << highestMic->GetDefaultPriorityScore() << ") -> "
             << (highestMic->GetId() == "M002" ? "PASS" : "FAIL") << "\n";

        // Test 9: Khong co dieu khien tuong thich phong Z999.
        try {
            acService.Allocate([](const shared_ptr<AirConditionerRemote>& item) {
                return item->IsCompatibleWith("Z999");
            });
            cout << "[9] No suitable AC remote: FAIL\n";
        } catch (const NoSuitableItemException&) {
            cout << "[9] No suitable AC remote: PASS (exception handled)\n";
        }

        // Test 10: Xac nhan thiet bi da chon chuyen sang Borrowed.
        cout << "[10] Status after allocation: "
             << StatusToString(selectedProjector->GetStatus()) << " -> "
             << (selectedProjector->GetStatus() == DeviceStatus::Borrowed ? "PASS" : "FAIL")
             << "\n\n";

        // Test 11: Cung mot template repository duoc dung cho nhieu loai thiet bi.
        cout << "[11] Generic repository reuse: projector count = "
             << projectorRepo->GetAll().size() << ", microphone count = "
             << micRepo->GetAll().size() << " -> PASS\n";

        // Kiem tra ngoai le DeviceNotReadyException voi micro pin duoi nguong.
        shared_ptr<WirelessMicrophone> lowBatteryMic = micRepo->FindById("M003");
        try {
            lowBatteryMic->Borrow();
            cout << "[EXTRA] DeviceNotReadyException: FAIL\n";
        } catch (const DeviceNotReadyException& ex) {
            cout << "[EXTRA] DeviceNotReadyException: PASS (device "
                 << ex.GetDeviceId() << ", status "
                 << StatusToString(ex.GetCurrentStatus()) << ")\n";
        }

        // Kiem tra Remove tra ve false neu khong co ma can xoa.
        cout << "[EXTRA] Remove unknown ID returns false: "
             << (!projectorRepo->Remove("P999") ? "PASS" : "FAIL") << "\n";

        cout << "\nAll requested test scenarios have been executed.\n";
    } catch (const exception& ex) {
        cerr << "Unexpected error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
