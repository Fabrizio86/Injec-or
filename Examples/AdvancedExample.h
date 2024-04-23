//
// Created by Fabrizio Paino on 2024-04-22.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef INJECTTORTEST_ADVANCEDEXAMPLE_H
#define INJECTTORTEST_ADVANCEDEXAMPLE_H

#include <iostream>
#include <string>
#include "Container.hpp"

namespace AdvancedExample {

    class ILogger {
    public:
        virtual void Log(const std::string &message) = 0;
    };

    class Logger : public ILogger {
    public:
        void Log(const std::string &message) override {
            std::cout << ++logEntries << " - Log: " << message << std::endl;
        }

    private:
        int logEntries;
    };

    class IDatabase {
    public:
        virtual void Save(const std::string &data) = 0;
    };

    class Database : public IDatabase {
    public:
        ~Database() {
            std::cout << "Disposing DB Context\n";
        }

        virtual void Save(const std::string &data) override {
            std::cout << "Saving: " << data << std::endl;
        }
    };

    class IService {
    public:
        virtual void DoSomething() = 0;
    };

    class Service : public IService {
    public:
        Service()
                : logger(DI::Container::Instance().ResolveSingleton<ILogger>()) {}

        void DoSomething() override {
            logger->Log("Service operation");

            {
                auto scope = DI::Container::Instance().CreateScope();
                db = DI::Container::Instance().ResolveScoped<IDatabase>(scope);

                if (auto lock = db.lock()) {
                    lock->Save("Sample data");
                }
            }

            std::cout << "Done working!\n";
        }

    private:
        std::shared_ptr<ILogger> logger;
        std::weak_ptr<IDatabase> db;
    };

    class Service2 : public IService {
    public:
        Service2()
                : logger(DI::Container::Instance().ResolveSingleton<ILogger>()) {}

        void DoSomething() override {
            logger->Log("Service2 operation happening now!");

            {
                auto scope = DI::Container::Instance().CreateScope();
                db = DI::Container::Instance().ResolveScoped<IDatabase>(scope);

                if (auto lock = db.lock()) {
                    lock->Save("Sample data 2");
                }
            }

            std::cout << "Done working!\n";
        }

    private:
        std::shared_ptr<ILogger> logger;
        std::weak_ptr<IDatabase> db;
    };

    void resolveAndUseService() {
        std::cout << "Resolve dependencies\n";

        auto transientService = DI::Container::Instance().ResolveTransient<IService>();
        transientService->DoSomething();
        transientService = DI::Container::Instance().ResolveTransient<IService>("Service2");
        transientService->DoSomething();
    }

    void RunAdvanceExample() {
        std::cout << "Advanced Example, register services\n";

        DI::Container::Instance().RegisterSingleton<AdvancedExample::ILogger, AdvancedExample::Logger>();
        DI::Container::Instance().RegisterScoped<AdvancedExample::IDatabase, AdvancedExample::Database>();
        DI::Container::Instance().RegisterTransient<AdvancedExample::IService, AdvancedExample::Service>();
        DI::Container::Instance().RegisterTransient<AdvancedExample::IService, AdvancedExample::Service2>("Service2");

        resolveAndUseService();

        std::cout << "Done!\n-------\n";
    }

}

#endif //INJECTTORTEST_ADVANCEDEXAMPLE_H
