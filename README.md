# C++ expression sample

---

- [C++ expression sample](#c-expression-sample)
  - [How to compare function signature](#how-to-compare-function-signature)
  - [How to use fold expression](#how-to-use-fold-expression)

---

## How to compare function signature

- Hard way
  - make signature trait
  - compare return type, arguments type of two signature traits.
- Easy wasy
  - use `std::decay`
    - finally `add_pointer` is applied in case of function.
  - compare with `std::is_same`
- relatives
  - `void(int)`: function type. not FunctionObject, implicit conversion to function pointer.
  - `void(*)(int)`: function pointer. Function Object.
  - `void(&)(int)`: function reference. not FunctionObject, implicit conversion to function pointer.
- code

  ```cpp
  void hello(int) {}

  template<class Signature, class...Args>
  void check(Signature&& callable, Args&&...args)
  {
    std::cout << std::boolalpha;

    std::cout << typeid(Signature).name() << std::endl;
    std::cout << typeid(void(Args...)).name() << std::endl;
    std::cout << typeid(std::decay_t<Signature>).name() << std::endl;
    std::cout << typeid(std::decay_t<void(Args...)>).name() << std::endl;

    std::cout << std::is_same<Signature, void(Args...)>::value << std::endl;
    std::cout << std::is_same<Signature, void(Args...)>::value << std::endl;
    std::cout << std::is_same<std::decay_t<Signature>, std::decay_t<void(Args...)>>::value << std::endl;
  }

  check(hello, 1);
  ```

## How to use fold expression
