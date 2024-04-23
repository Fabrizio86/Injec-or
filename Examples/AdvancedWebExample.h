//
// Created by Fabrizio Paino on 2024-04-22.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef INJECTTORTEST_ADVANCEDWEBEXAMPLE_H
#define INJECTTORTEST_ADVANCEDWEBEXAMPLE_H

#include <iostream>
#include "Container.hpp"

namespace AdvancedWebExample {

    class ILogger {
    public:
        virtual ~ILogger() = default;
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
        virtual ~IDatabase() = default;
        virtual void Save(const std::string &data) = 0;
    };

    class Request {
    public:
        explicit Request(std::string actionData) : actionData(std::move(actionData)) {}

        std::string GetActionData() const {
            return actionData;
        }

    private:
        std::string actionData;
    };

    class MySQLDatabase : public IDatabase {
    public:
        ~MySQLDatabase() override {
            std::cout << "Disposing MySQL DB Context\n";
        }

        void Save(const std::string &data) override {
            std::cout << "Saving to MySQL DB: " << data << std::endl;
        }
    };

    class PostgreSQLDatabase : public IDatabase {
    public:
        ~PostgreSQLDatabase() override {
            std::cout << "Disposing PostgreSQL DB Context\n";
        }

        void Save(const std::string &data) override {
            std::cout << "Saving to PostgreSQL DB: " << data << std::endl;
        }
    };

    class IController {
    public:
        virtual void Action1(Request req) = 0;

        virtual void Action2()= 0;
    };

    class UserController : public IController {
    public:
        UserController()
                : logger(DI::Container::Instance().ResolveSingleton<ILogger>()) {}

        void Action1(Request req) override {
            logger->Log("In UserController Action1");

            std::shared_ptr<IDatabase> db;

            db = DI::Container::Instance().ResolveTransient<IDatabase>(req.GetActionData());

            if (!db) {
                throw std::runtime_error("Unsupported database specified");
            }

            db->Save("User data");
        }

        void Action2() override {
            logger->Log("In UserController Action2");
        }

    private:
        std::shared_ptr<ILogger> logger;
    };

    class HomeController : public IController {
    public:
        HomeController() : logger(DI::Container::Instance().ResolveSingleton<ILogger>()) {}

        void Action1(Request req) override {
            logger->Log("In HomeController Action1, requested: " + req.GetActionData());
        }

        void Action2() override {
            logger->Log("In HomeController Action2");
        }

    private:
        std::shared_ptr<ILogger> logger;
    };

    void RegisterWebServerExample() {
        std::cout << "Web Example, register services\n";
        DI::Container::Instance().RegisterSingleton<ILogger, Logger>();
        DI::Container::Instance().RegisterTransient<IDatabase, MySQLDatabase>("MySQL");
        DI::Container::Instance().RegisterTransient<IDatabase, PostgreSQLDatabase>("PostgreSQL");
        DI::Container::Instance().RegisterTransient<IController, HomeController>("Home");
        DI::Container::Instance().RegisterTransient<IController, UserController>("User");
    }

    void RunWebServerExample() {
        auto start = std::chrono::high_resolution_clock::now();

        RegisterWebServerExample();

        std::cout << "Resolve dependencies\n";

        Request mysqlRequest("MySQL");

        auto homeController = DI::Container::Instance().ResolveTransient<IController>("Home");
        homeController->Action1(mysqlRequest);
        homeController->Action2();

        auto userController = DI::Container::Instance().ResolveTransient<IController>("User");
        userController->Action1(mysqlRequest);

        Request postgreRequest("PostgreSQL");
        userController->Action1(postgreRequest);

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> ms_double = end - start;

        std::cout << "Done! Execution time: " << ms_double.count() << " ms\n-------\n";
    }
}

#endif //INJECTTORTEST_ADVANCEDWEBEXAMPLE_H
