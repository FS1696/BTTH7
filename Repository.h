/****************/
// Mã sinh viên: [ĐIỀN MÃ SINH VIÊN]
// Họ tên:       [ĐIỀN HỌ TÊN]
/****************/
#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "DeviceCommon.h"
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

// Kho tong quat chi nhan cac doi tuong co ma dinh danh.
template <typename T>
class IRepository {
    static_assert(std::is_base_of<IIdentifiable, T>::value,
                  "T must implement IIdentifiable.");
public:
    virtual void Add(const std::shared_ptr<T>& item) = 0;
    virtual bool Remove(const std::string& id) = 0;
    virtual std::shared_ptr<T> FindById(const std::string& id) const = 0;
    virtual std::vector<std::shared_ptr<T> > GetAll() const = 0;
    virtual std::vector<std::shared_ptr<T> > FindAll(
        const std::function<bool(const std::shared_ptr<T>&)>& condition) const = 0;
    virtual ~IRepository() {}
};

template <typename T>
class InMemoryRepository : public IRepository<T> {
    static_assert(std::is_base_of<IIdentifiable, T>::value,
                  "T must implement IIdentifiable.");
private:
    // vector giu thu tu them vao; cac ham doc tra ban sao danh sach.
    std::vector<std::shared_ptr<T> > items;
public:
    void Add(const std::shared_ptr<T>& item) override {
        if (!item) {
            throw std::invalid_argument("Repository cannot store a null item.");
        }
        if (FindById(item->GetId())) {
            throw DuplicateIdException(item->GetId());
        }
        items.push_back(item);
    }

    bool Remove(const std::string& id) override {
        typename std::vector<std::shared_ptr<T> >::iterator it =
            std::find_if(items.begin(), items.end(),
                [&id](const std::shared_ptr<T>& item) {
                    return item->GetId() == id;
                });
        if (it == items.end()) return false;
        items.erase(it);
        return true;
    }

    std::shared_ptr<T> FindById(const std::string& id) const override {
        for (std::size_t i = 0; i < items.size(); ++i) {
            if (items[i]->GetId() == id) return items[i];
        }
        return std::shared_ptr<T>();
    }

    std::vector<std::shared_ptr<T> > GetAll() const override {
        return items;
    }

    std::vector<std::shared_ptr<T> > FindAll(
        const std::function<bool(const std::shared_ptr<T>&)>& condition) const override {
        if (!condition) {
            throw std::invalid_argument("FindAll requires a valid condition.");
        }
        std::vector<std::shared_ptr<T> > result;
        for (std::size_t i = 0; i < items.size(); ++i) {
            if (condition(items[i])) result.push_back(items[i]);
        }
        return result;
    }
};

#endif // REPOSITORY_H
