# Chapter 4: Template Metaprogramming & Compile-time Optimization

Template Metaprogramming (TMP) allows us to write code that generates other code at compile time. This is powerful for HFT because it enables **static polymorphism**, avoiding the runtime cost of virtual functions.

## Key Concepts

### 1. CRTP (Curiously Recurring Template Pattern)
A technique where a class `Derived` inherits from a class template `Base<Derived>`.
- *Benefit*: Allows the base class to call methods of the derived class at compile time.
- *Use Case*: Replacing virtual functions (dynamic dispatch) with static dispatch.

### 2. SFINAE (Substitution Failure Is Not An Error)
A technique to enable/disable function templates based on types.
- *Note*: In C++20, this is largely replaced by **Concepts**, which are much cleaner.

### 3. Policy-Based Design
Designing classes that take "policies" (other classes) as template arguments to define behavior.
- *Benefit*: Highly flexible and performant code composition.

## Interactive Exercise
We will implement a simple logging system using CRTP to demonstrate static polymorphism. We will compare it conceptually to a virtual function approach.
