#ifndef __VARIANT_H__
#define __VARIANT_H__

#include <variant>
#include <type_traits>
#include <typeindex>

namespace btrack {

// Primary VariantBase class template
template <typename... _Types>
class VariantBase {
public:

    // Final variant type
    using VariantType = std::variant<_Types...>;

    // Default constructor
    VariantBase() = default;

    // Constructor for any type in the variant
    template <typename T>
    VariantBase(const T& value) : data(value) {}

    // Get the type index
    std::type_index type() const {
        return std::visit([](const auto& value) -> std::type_index {
            return std::type_index(typeid(value));
        }, data);
    }

    // Check if it holds a specific type
    template <typename T>
    bool is() const {
        return std::holds_alternative<T>(data);
    }

    // Get value safely
    template <typename T>
    T& get() {
        return std::get<T>(data);
    }

    template <typename T>
    const T& get() const {
        return std::get<T>(data);
    }

private:
    VariantType data;
};



}

#endif // __VARIANT_H__