//
// Created by Fabrizio Paino on 2024-04-22.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef INJECTTORTEST_WEBEXAMPLE_H
#define INJECTTORTEST_WEBEXAMPLE_H

#include "Container.hpp"
#include <iostream>
#include <string>

namespace WebServerExample {

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

    class IController {
    public:
        virtual void Action1() = 0;

        virtual void Action2() = 0;
    };

    class HomeController : public IController {
    public:
        HomeController() : logger(DI::Container::Instance().ResolveSingleton<ILogger>()) {}

        void Action1() override {
            logger->Log("In HomeController Action1");
        }

        void Action2() override {
            logger->Log("In HomeController Action2");
        }

    private:
        std::shared_ptr<ILogger> logger;
    };

    class UserController : public IController {
    public:
        UserController()
                : logger(DI::Container::Instance().ResolveSingleton<ILogger>()),
                  db(DI::Container::Instance().ResolveSingleton<IDatabase>()) {}

        void Action1() override {
            logger->Log("In UserController Action1");
            db->Save("User data");
        }

        void Action2() override {
            logger->Log("In UserController Action2");
        }

    private:
        std::shared_ptr<ILogger> logger;
        std::shared_ptr<IDatabase> db;
    };

    void RegisterWebServerExample() {
        std::cout << "Web Example, register services\n";
        DI::Container::Instance().RegisterSingleton<WebServerExample::ILogger, WebServerExample::Logger>();
        DI::Container::Instance().RegisterSingleton<WebServerExample::IDatabase, WebServerExample::Database>();

        DI::Container::Instance().RegisterSingleton<WebServerExample::IController, WebServerExample::HomeController>("Home");
        DI::Container::Instance().RegisterSingleton<WebServerExample::IController, WebServerExample::UserController>("User");
    }

    void RunWebServerExample() {
        auto start = std::chrono::high_resolution_clock::now();

        RegisterWebServerExample();

        std::cout << "Resolve dependencies\n";
        auto homeController = DI::Container::Instance().ResolveSingleton<IController>("Home");
        homeController->Action1();
        homeController->Action2();

        auto userController = DI::Container::Instance().ResolveSingleton<IController>("User");
        userController->Action1();
        userController->Action2();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms_double = end - start;
        std::cout << "Done! Execution time: " << ms_double.count() << " ms\n-------\n";
    }
}

#endif //INJECTTORTEST_WEBEXAMPLE_H
