//
// Created by Fabrizio Paino on 2024-04-22.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef INJECTTORTEST_SIMPLEEXAMPLE_H
#define INJECTTORTEST_SIMPLEEXAMPLE_H

#include <iostream>
#include "Container.hpp"

namespace SimpleExample {

    class IService {
    public:
        virtual void DoSomething() = 0;
    };

    class Service : public IService {
    public:
        void DoSomething() override {
            std::cout << "Service operation\n";
        }
    };

    void Register() {
        std::cout << "Simple Example, register services\n";

        DI::Container::Instance().RegisterSingleton<IService, Service>();
        DI::Container::Instance().RegisterTransient<IService, Service>();
        DI::Container::Instance().RegisterScoped<IService, Service>();
    }

    void RunSimpleService() {
        auto start = std::chrono::high_resolution_clock::now();

        Register();

        std::cout << "Resolve dependencies\n";

        auto singletonService = DI::Container::Instance().ResolveSingleton<IService>();
        singletonService->DoSomething();

        auto transientService = DI::Container::Instance().ResolveTransient<IService>();
        transientService->DoSomething();

        auto scope = DI::Container::Instance().CreateScope();
        auto scopedService = DI::Container::Instance().ResolveScoped<IService>(scope);

        if (auto servicePtr = scopedService.lock()) {
            servicePtr->DoSomething();
        }


        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms_double = end - start;
        std::cout << "Done! Execution time: " << ms_double.count() << " ms\n-------\n";
    }

}

#endif //INJECTTORTEST_SIMPLEEXAMPLE_H
