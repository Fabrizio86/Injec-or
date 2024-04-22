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
___

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

___

## How to contribute

We absolutely welcome and encourage contributions! If you'd like to contribute, here are some guidelines to help you get started.

1. **Fork the repository and clone it locally.** Use git to fork the repository, clone it to your local machine, and add the original repository as an upstream remote.

    ```
    git clone https://github.com/Fabrizio86/Injecttor.git
    git remote add upstream https://github.com/Fabrizio86/Injecttor.git
    ```

2. **Create a new branch for your fix or feature.** Try to name your branch in a way that describes the fix or feature.

    ```
    git checkout -b branch-name
    ```

3. **Write your code.** Make your changes in your new branch. Ensure your code is clean, well-commented, and adheres to the project's style guidelines.

4. **Test your changes.** Make sure your changes don't break any existing functionality, and that they fully address the issue or implement the feature.

5. **Commit and push your changes.** Write a meaningful commit message. Push your changes to your fork on GitHub.

    ```
    git commit -m "Commit message"
    git push origin branch-name
    ```

6. **Create a pull request.** Go to your repository on GitHub, and click the "New Pull Request" button. Fill out the form to create a PR.

Before you create a pull request, please ensure that your code can be merged without conflicts and that all tests pass. After you submit a PR, we will review your contribution. We might ask for changes before merging your code. We appreciate your patience and help in improving our project.

Thank you for your interest in our project, and we are excited to reviewing your contribution!

## Licence
Copyright © 2024 by Aedifex Solutions Inc. All Rights Reserved.


This software is provided under MIT license, please make sure you understand the implication before using this Software, in your own project.


