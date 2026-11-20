#ifndef EXPECTED_H
#define EXPECTED_H

#include <type_traits>
#include <utility>
#include <variant>

/**
 * @brief An error value, so a function can return an error instead of a value.
 *
 * @tparam E The error type.
 */
template<typename E>
struct Unexpected final {
  /**
   * @brief The error value.
   */
  E err;
};

template<typename E>
Unexpected(E) -> Unexpected<E>;

/**
 * @brief A type indicating either a value or an error, mirroring `std::expected`.
 *
 * @tparam T The value type.
 * @tparam E The error type.
 */
template<typename T, typename E, typename = std::enable_if_t<!std::is_same_v<T, void>>>
class Expected final {
  using VariantT = std::variant<T, Unexpected<E>>;

 public:
  /**
   * @brief The value type.
   */
  using value_type = T;

  /**
   * @brief The error type.
   */
  using error_type = E;

  /**
   * @brief The error alternative.
   */
  using unexpected_type = Unexpected<E>;

  /**
   * @brief Rebinds the value type, keeping the error type.
   *
   * @tparam U The new value type.
   */
  template<typename U>
  using rebind = Expected<U, E>;

  /**
   * @brief Constructs an instance by default constructing an instance of `T`.
   *
   * @tparam U The value type; constrained to be default constructible.
   */
  template<typename U = T, typename = std::enable_if_t<std::is_default_constructible_v<U>>>
  Expected() : data{std::in_place_type<T>, T{}} {}

  /**
   * @brief Constructs an instance with a value by moving it into this instance.
   *
   * @param value The value to hold.
   */
  Expected(std::remove_cv_t<T>&& value) : data{std::in_place_type<T>, std::move(value)} {}

  /**
   * @brief Constructs an instance by converting from an `Expected` instance that holds a value of type `T2`.
   *
   * @tparam T2 The source value type.
   * @param other The `Expected` to convert; an error stays an error.
   */
  template<typename T2>
  explicit Expected(const Expected<T2, E>& other) :
      data{
          std::holds_alternative<T2>(other.data)
              ? VariantT{std::in_place_type<T>, std::get<T2>(other.data)}
              : VariantT{std::in_place_type<Unexpected<E>>, std::get<Unexpected<E>>(other.data)},
      } {}

  /**
   * @brief Constructs an instance by converting from an `Expected` instance that holds an error of type `E2`.
   *
   * @tparam E2 The source error type.
   * @param other The `Expected` to convert; a value stays a value.
   */
  template<typename E2>
  explicit Expected(const Expected<T, E2>& other) :
      data{
          std::holds_alternative<T>(other.data)
              ? VariantT{std::in_place_type<T>, std::get<T>(other.data)}
              : VariantT{std::in_place_type<Unexpected<E>>, Unexpected<E>{std::get<Unexpected<E2>>(other.data).err}},
      } {}

  /**
   * @brief Constructs the error alternative.
   *
   * @param other The error to hold.
   */
  Expected(const Unexpected<E>& other) : data{std::in_place_type<Unexpected<E>>, other} {}

  /**
   * @brief Constructs the error alternative.
   *
   * @param other The error to move in.
   */
  Expected(Unexpected<E>&& other) : data{std::in_place_type<Unexpected<E>>, std::move(other)} {}

  /**
   * @return A pointer to the underlying value.
   */
  constexpr const T* operator->() const noexcept { return std::get_if<T>(&data); }
  /**
   * @return A const pointer to the underlying value.
   */
  constexpr T* operator->() noexcept { return std::get_if<T>(&data); }
  /**
   * @return A const lvalue reference to the underlying value.
   */
  constexpr const T& operator*() const& noexcept { return *operator->(); }
  /**
   * @return An lvalue reference to the underlying value.
   */
  constexpr T& operator*() & noexcept { return *operator->(); }
  /**
   * @return A const rvalue reference to the underlying value.
   */
  constexpr const T&& operator*() const&& noexcept { return std::move(*operator->()); }
  /**
   * @return An rvalue reference to the underlying value.
   */
  constexpr T&& operator*() && noexcept { return std::move(*operator->()); }

  /**
   * @return True if this instance holds a value.
   */
  [[nodiscard]] constexpr bool has_value() const noexcept { return std::holds_alternative<T>(data); }

  /**
   * @brief Alias for `has_value()`.
   */
  constexpr explicit operator bool() const noexcept { return has_value(); }

  /**
   * @return An lvalue reference to the underlying value.
   */
  constexpr T& value() & { return std::get<T>(data); }
  /**
   * @return A const lvalue reference to the underlying value.
   */
  constexpr const T& value() const& { return std::get<T>(data); }
  /**
   * @return An rvalue reference to the underlying value.
   */
  constexpr T&& value() && { return std::move(std::get<T>(data)); }
  /**
   * @return A const rvalue reference to the underlying value.
   */
  constexpr const T&& value() const&& { return std::move(std::get<T>(data)); }

  /**
   * @return A const lvalue reference to the held error.
   */
  const E& error() const& noexcept { return std::get_if<Unexpected<E>>(&data)->err; }
  /**
   * @return An lvalue reference to the held error.
   */
  E& error() & noexcept { return std::get_if<Unexpected<E>>(&data)->err; }
  /**
   * @return A const rvalue reference to the held error.
   */
  const E&& error() const&& noexcept { return std::move(std::get_if<Unexpected<E>>(&data)->err); }
  /**
   * @return An rvalue reference to the held error.
   */
  E&& error() && noexcept { return std::move(std::get_if<Unexpected<E>>(&data)->err); }

 private:
  template<typename, typename, typename>
  friend class Expected;

  VariantT data;
};

#endif  // EXPECTED_H
