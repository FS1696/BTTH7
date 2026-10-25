/****************/
// Mã sinh viên: 202419074
// Họ tên: Vũ Thành Lâm
/****************/
#ifndef ALLOCATION_SERVICE_H
#define ALLOCATION_SERVICE_H

#include "TeachingDevice.h"
#include "Repository.h"
#include "SelectionStrategy.h"
#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

// Dich vu cap phat chi phu thuoc vao interface kho va chien luoc.
template <typename T>
class AllocationService {
    static_assert(std::is_base_of<TeachingDevice, T>::value,
                  "T must derive from TeachingDevice.");
private:
    std::shared_ptr<IRepository<T> > repository;
    std::shared_ptr<ISelectionStrategy<T> > strategy;
public:
    AllocationService(
        const std::shared_ptr<IRepository<T> >& repo,
        const std::shared_ptr<ISelectionStrategy<T> >& selectionStrategy)
        : repository(repo), strategy(selectionStrategy) {
        if (!repository || !strategy) {
            throw std::invalid_argument(
                "Repository and strategy must not be null.");
        }
    }

    void ChangeStrategy(
        const std::shared_ptr<ISelectionStrategy<T> >& newStrategy) {
        if (!newStrategy) {
            throw std::invalid_argument("New strategy must not be null.");
        }
        strategy = newStrategy;
    }

    std::shared_ptr<T> Allocate(
        const std::function<bool(const std::shared_ptr<T>&)>& additionalCondition =
            std::function<bool(const std::shared_ptr<T>&)>()) {
        const std::vector<std::shared_ptr<T> > allDevices = repository->GetAll();
        std::vector<std::shared_ptr<T> > candidates;
        for (std::size_t i = 0; i < allDevices.size(); ++i) {
            const std::shared_ptr<T>& device = allDevices[i];
            if (!device->IsReadyForUse()) continue;
            if (additionalCondition && !additionalCondition(device)) continue;
            candidates.push_back(device);
        }
        std::shared_ptr<T> selected = strategy->Select(candidates);
        selected->Borrow();
        return selected;
    }
};

#endif // ALLOCATION_SERVICE_H
