# Injec++or: C++ Dependency Injection Container

Injec++or is a powerful and efficient Dependency Injection(DI) library designed to introduce feature and semantics known to developers in other languages, bringing them to your C++ projects, aiming to simplify your application development by managing your class dependencies, this way improving code readability, modularity and overall software design.

## Features

- Simple registration and resolution of services
- Singleton, Transient, and Scoped services support
- Auto-managed class dependencies

---
## Container

**Injec++or** provides a container class for dependency injection where you can register services and later, recover them. Services in dependency injection are the dependencies that can be injected into components (like classes).

There are three types of services *Injec++or* can manage:

1. **Singleton**: These services are created once and are shared among all who request for it.
2. **Transient**: These services are created each time they are requested.
3. **Scoped**: These services are created once per scope and are shared among all within that same scope.

For example:
- Singleton services could be useful for stateful services.
- Transient services could be useful for stateless services.
- Scoped services could be useful for services where specific instances should be shared in a specific context.

---
## Usage

Here is an example of how to use **Injec++or**:

### Define an Interface and an Implementation

```c++
class IMyService {
public:
    virtual ~IMyService() = default;
    virtual void MyMethod() = 0;
};

class MyService : public IMyService {
public:
    void MyMethod() override {
        // Implementation of MyMethod
    }
};
```
### Register Services in the Dependency Injection Container
Services are registered in main.

```c++
#include "Container.h"

int main() {
    
    DI::Container::Instance().RegisterSingleton<IMyService, MyService>();
    DI::Container::Instance().RegisterTransient<IMyService, MyService>();
    DI::Container::Instance().RegisterScoped<IMyService, MyService>();

    // app logic in here
      
    return 0;
}
```
### Resolve Services from the Dependency Injection Container
To resolve a service, call the appropriate resolve function depending on the type of the registered service.

```c++
auto singletonService = DI::Container::Instance().ResolveSingleton<IMyService>();
auto transientService = DI::Container::Instance().ResolveTransient<IMyService>();

{
  auto scope = DI::Container::Instance().CreateScope();
  auto scopedService = DI::Container::Instance().ResolveScoped<IMyService>(scope);
}

```
Now you can use these service instances in your classes, leaving Injec++or to take care of managing the service life cycles and dependencies.

## Constructor Injection
Here is an example of getting dependencies injected within a constructor.

```c++
class MyConsumerClass {
public:
    // Default constructor that instantiates services via the container
    MyConsumerClass() 
        : service1(DI::Container::Instance().ResolveSingleton<IMyService1>()), 
          service2(DI::Container::Instance().ResolveSingleton<IMyService2>()) 
    {}

    void UseServices() const {
        service1->DoSomething();
        service2->DoAnotherThing();
    }

private:
    std::shared_ptr<IMyService1> service1;
    std::shared_ptr<IMyService2> service2;
};
```

memory is managed internally using smart pointers, in the case of scoped dependencies, weak pointers are returned to regulate the life time scope of the service it holds.
To minimize footprint, the code makes no use of macros nor other "compiler magic" nor runtime post-processing effects either, maximizing the speed you get in your application.
Injec++or is a header only library, for easy incorporation in you program. 

## Contributions
We welcome contributions! Please see our contribution guide for more details.

## Licence
Copyright © 2024 by Aedifex Solutions Inc. All Rights Reserved.
This software is provided under MIT license, please make sure you understand the implication before using this Software, in your own project.


