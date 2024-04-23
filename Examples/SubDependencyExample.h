//
// Created by Fabrizio Paino on 2024-04-22.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef INJECTTORTEST_SUBDEPENDENCYEXAMPLE_H
#define INJECTTORTEST_SUBDEPENDENCYEXAMPLE_H

#include <iostream>
#include "Container.hpp"

namespace SubDependencies {

    class ILogger {
    public:
        virtual void Log(const std::string &message) = 0;
    };

    class Logger : public ILogger {
    public:
        void Log(const std::string &message) override {
            std::cout << "Log: " << message << std::endl;
        }
    };

    class IService {
    public:
        virtual void DoSomething() = 0;
    };

    class Service : public IService {
    public:
        explicit Service() :
                logger(DI::Container::Instance().ResolveSingleton<ILogger>()) {}

        void DoSomething() override {
            std::cout << "Service is doing something!\n";
            logger->Log("Service operation.");
        }

    private:
        std::shared_ptr<ILogger> logger;
    };

    void resolveAndUseService() {
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

        std::cout << "Done!\n-------\n";
    }

    void RunSubDependency() {
        std::cout << "Sub Dependencies Example, register services\n";

        DI::Container::Instance().RegisterSingleton<ILogger, Logger>();
        DI::Container::Instance().RegisterTransient<ILogger, Logger>();
        DI::Container::Instance().RegisterScoped<ILogger, Logger>();

        DI::Container::Instance().RegisterSingleton<IService, Service>();
        DI::Container::Instance().RegisterTransient<IService, Service>();
        DI::Container::Instance().RegisterScoped<IService, Service>();

        resolveAndUseService();
    }

}

#endif //INJECTTORTEST_SUBDEPENDENCYEXAMPLE_H
