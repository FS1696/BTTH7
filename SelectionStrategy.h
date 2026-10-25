/****************/
// Mã sinh viên: 202419074
// Họ tên: Vũ Thành Lâm
/****************/
#ifndef SELECTION_STRATEGY_H
#define SELECTION_STRATEGY_H

#include "DeviceCommon.h"
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

// Interface cho phep thay doi thuat toan chon luc chay.
template <typename T>
class ISelectionStrategy {
public:
    virtual std::shared_ptr<T> Select(
        const std::vector<std::shared_ptr<T> >& candidates) const = 0;
    virtual ~ISelectionStrategy() {}
};

// Chon ung vien co diem cao nhat.
template <typename T>
class HighestPriorityStrategy : public ISelectionStrategy<T> {
private:
    std::function<double(const std::shared_ptr<T>&)> scoreSelector;
public:
    explicit HighestPriorityStrategy(
        const std::function<double(const std::shared_ptr<T>&)>& selector)
        : scoreSelector(selector) {
        if (!scoreSelector) {
            throw std::invalid_argument(
                "HighestPriorityStrategy requires a score selector.");
        }
    }

    std::shared_ptr<T> Select(
        const std::vector<std::shared_ptr<T> >& candidates) const override {
        if (candidates.empty()) throw NoSuitableItemException();
        std::shared_ptr<T> best = candidates[0];
        double bestScore = scoreSelector(best);
        for (std::size_t i = 1; i < candidates.size(); ++i) {
            double score = scoreSelector(candidates[i]);
            // Neu bang diem, giu ung vien xuat hien truoc.
            if (score > bestScore) {
                best = candidates[i];
                bestScore = score;
            }
        }
        return best;
    }
};

// Chon ung vien dau tien trong danh sach.
template <typename T>
class FirstAvailableStrategy : public ISelectionStrategy<T> {
public:
    std::shared_ptr<T> Select(
        const std::vector<std::shared_ptr<T> >& candidates) const override {
        if (candidates.empty()) throw NoSuitableItemException();
        return candidates[0];
    }
};

#endif // SELECTION_STRATEGY_H
