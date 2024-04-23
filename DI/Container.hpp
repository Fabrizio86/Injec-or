// Created by Fabrizio Paino on 2024-04-22.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.

#ifndef INJECTTORTEST_CONTAINER_HPP
#define INJECTTORTEST_CONTAINER_HPP

#include <typeinfo>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <functional>

namespace DI {

    /**
    * @class BaseService
    *
    * @brief The BaseService class provides an interface for erased services.
    *
    * This class serves as a base class for erased services. Erased services are services
    * that can be utilized without knowing the exact type. This class provides a virtual
    * destructor to allow proper destruction of derived classes.
    */
    class BaseService {
    public:
        virtual ~BaseService() = default;
    };

    template<class T>
    using CreatorSharedFnc = std::function<std::shared_ptr<T>()>;

    using MapType = std::unordered_map<std::string, std::shared_ptr<BaseService>>;

    /**
    * @class TypedService
    *
    * @brief The TypedService class provides a typed service implementation.
    *
    * This class is a sub-class of BaseService and provides a way to create and access
    * a typed service. It stores a creator function that can be used to create instances
    * of the service type T. The creator function is a std::function that returns a
    * std::shared_ptr<T>.
    */
    template<class T>
    class TypedService : public BaseService {
    public:

        void SetCreator(CreatorSharedFnc<T> crt) {
            this->creator = crt;
        }

        std::shared_ptr<T> CreateService() {
            return this->creator();
        }

    private:
        CreatorSharedFnc<T> creator;
    };

    /**
    * @class TypedServiceSingleton
    *
    * @brief The TypedServiceSingleton class is a singleton pattern implementation for a specific type of service.
    *
    * This class provides a way to create, retrieve, and manage a single instance of a service of a given type. It
    * follows the singleton pattern, ensuring that only one instance of the service exists throughout the lifetime
    * of the application.
    *
    * @tparam T The type of the service.
    */
    template<class T>
    class TypedServiceSingleton : public BaseService {
    public:
        std::shared_ptr<T> instance;

        void SetCreator(CreatorSharedFnc<T> crt) {
            this->instance = crt();
        }

        std::shared_ptr<T> CreateService() {
            return this->instance;
        }

    private:
        CreatorSharedFnc<T> creator;
    };

    /**
    * @class Container
    *
    * @brief The Container class provides a dependency injection container.
    *
    * The Container class is responsible for managing the registration and resolution
    * of services. It supports three types of services: Singleton, Transient, and Scoped.
    * Singleton services are created only once and shared among all consumers. Transient
    * services are created each time they are requested. Scoped services are created once
    * per scope and shared among all consumers within that scope.
    *
    * Example usage:
    *
    * Container::Instance().RegisterSingleton<IService, Service>();
    * Container::Instance().RegisterTransient<IService, Service>();
    * Container::Instance().RegisterScoped<IService, Service>();
    *
    * auto singletonService = Container::Instance().ResolveSingleton<IService>();
    * auto transientService = Container::Instance().ResolveTransient<IService>();
    * auto scope = Container::Instance().CreateScope();
    * auto scopedService = Container::Instance().ResolveScoped<IService>(scope);
    *
    * @see TypedServiceSingleton
    * @see TypedService
    * @see BaseService
    */
    class Container {
    public:

        /**
        * @class Container
        *
        * @brief The Container class provides a dependency injection container.
        *
        * The Container class is responsible for managing the registration and resolution
        * of services. It supports three types of services: Singleton, Transient, and Scoped.
        * Singleton services are created only once and shared among all consumers. Transient
        * services are created each time they are requested. Scoped services are created once
        * per scope and shared among all consumers within that scope.
        */
        static Container &Instance() {
            static Container instance;
            return instance;
        }

        /**
        * @class Scope
        *
        * @brief The Scope class represents a scope in the Container.
        *
        * The Scope class is responsible for managing the services that are created and shared within a specific scope.
        * Services registered as scoped are created once per scope and are shared among all consumers within that scope.
        */
        class Scope final {
        private:
            friend Container;
            std::unordered_map<std::string, std::shared_ptr<void>> services;
        };

        /**
        * deleted constructor
        */
        Container(const Container &) = delete;

        /**
        * deleted constructor
        */
        Container &operator=(const Container &) = delete;

        /**
        * @brief Registers a singleton service in the Container.
        *
        * This function is responsible for registering a singleton service in the Container. The provided types are used to ensure
        * that the implementation derives from the interface. The function checks if the service has already been registered and throws
        * an exception if it has. If the service has not been registered, it creates a TypedServiceSingleton instance, sets the creator
        * function, and adds it to the singletonServices map.
        *
        * @tparam TInterface The interface type of the service.
        * @tparam TImplementation The implementation type of the service.
        *
        * @throw std::runtime_error if the singleton service is already registered.
        */
        template<class TInterface, class TImplementation>
        void RegisterSingleton(std::string tag = "") {
            static_assert(std::is_base_of<TInterface, TImplementation>::value,
                          "TImplementation should derive from TInterface");

            std::string typeName = typeid(TInterface).name();
            auto mapIt = singletonServices.find(typeName);
            if (mapIt != singletonServices.end()) {
                auto tagIt = mapIt->second.find(tag);
                if (tagIt != mapIt->second.end()) {
                    throw std::runtime_error("Singleton Service already registered");
                }
            }

            auto service = std::make_shared<TypedServiceSingleton<TInterface>>();
            service->SetCreator([] { return std::make_shared<TImplementation>(); });

            singletonServices[typeName][tag] = service;
        }

        /**
        * @fn template<class TInterface, class TImplementation> void RegisterTransient()
        * @brief Registers a transient service in the Container.
        *
        * This function is responsible for registering a transient service in the Container. The provided types are used to ensure
        * that the implementation derives from the interface. The function checks if the service has already been registered and throws
        * an exception if it has. If the service has not been registered, it creates a TypedService instance, sets the creator
        * function, and adds it to the transientServices map.
        *
        * @tparam TInterface The interface type of the service.
        * @tparam TImplementation The implementation type of the service.
        *
        * @throw std::runtime_error if the transient service is already registered.
        */
        template<class TInterface, class TImplementation>
        void RegisterTransient(std::string tag = "") {
            static_assert(std::is_base_of<TInterface, TImplementation>::value,
                          "TImplementation should derive from TInterface");

            std::string typeName = typeid(TInterface).name();
            auto mapIt = transientServices.find(typeName);

            if (mapIt != transientServices.end()) {
                auto tagIt = mapIt->second.find(tag);

                if (tagIt != mapIt->second.end()) {
                    throw std::runtime_error("Transient service already registered with this tag");
                }
            }

            auto service = std::make_shared<TypedService<TInterface>>();
            service->SetCreator([] { return std::make_shared<TImplementation>(); });

            transientServices[typeName][tag] = service;
        }

        /**
        * @fn template<class TInterface, class TImplementation> void RegisterScoped()
        * @brief Registers a scoped service in the Container.
        *
        * This function is responsible for registering a scoped service in the Container. The provided types are used to ensure
        * that the implementation derives from the interface. The function checks if the service has already been registered and throws
        * an exception if it has. If the service has not been registered, it creates a TypedService instance, sets the creator
        * function, and adds it to the scopedServices map.
        *
        * @tparam TInterface The interface type of the service.
        * @tparam TImplementation The implementation type of the service.
        *
        * @throw std::runtime_error if the scoped service is already registered.
        */
        template<class TInterface, class TImplementation>
        void RegisterScoped(std::string tag = "") {
            static_assert(std::is_base_of<TInterface, TImplementation>::value,
                          "TImplementation should derive from TInterface");

            std::string typeName = typeid(TInterface).name();
            auto mapIt = scopedServices.find(typeName);
            if (mapIt != scopedServices.end()) {
                auto tagIt = mapIt->second.find(tag);
                if(tagIt != mapIt->second.end()){
                    throw std::runtime_error("Scoped Service is already registered");
                }
            }

            auto service = std::make_shared<TypedService<TInterface>>();
            service->SetCreator([] { return std::make_shared<TImplementation>(); });

            scopedServices[typeName][tag] = service;
        }


        /**
        * @brief Resolves a singleton service from the Container.
        *
        * This function is responsible for resolving a singleton service from the Container. It looks for the service type in the
        * singletonServices map. If the service is not found, an exception is thrown. If the service is found, it is casted to the
        * TypedServiceSingleton type and the CreateService function is called to retrieve the instance of the service.
        *
        * @tparam TInterface The interface type of the service.
        * @return std::shared_ptr<TInterface> The resolved singleton service.
        * @throw std::runtime_error if the singleton service is not found in the Container.
        */
        template<typename TInterface>
        std::shared_ptr<TInterface> ResolveSingleton(std::string tag = "") {
            std::string typeName = typeid(TInterface).name();

            auto map = singletonServices.find(typeName);
            if (map == singletonServices.end()) {
                throw std::runtime_error("Singleton Service not found: " + typeName);
            }

            auto it = map->second.find(tag);
            if (it == map->second.end()) {
                throw std::runtime_error("Singleton Service not found: " + typeName);
            }

            auto val = std::static_pointer_cast<TypedServiceSingleton<TInterface>>(it->second);
            return val->CreateService();
        }

        /**
        * @brief Resolves a transient service from the Container.
        *
        * This function is responsible for resolving a transient service from the Container. It looks for the service type in the
        * transientServices map. If the service is not found, an exception is thrown. If the service is found, it is casted to the
        * TypedService type for the given interface and the CreateService function is called to retrieve the instance of the service.
        *
        * @tparam TInterface The interface type of the service.
        * @return std::shared_ptr<TInterface> The resolved transient service.
        * @throw std::runtime_error if the transient service is not found in the Container.
        */
        template<typename TInterface>
        std::shared_ptr<TInterface> ResolveTransient(std::string tag = "") {
            std::string typeName = typeid(TInterface).name();
            auto map = transientServices.find(typeName);

            if (map == transientServices.end()) {
                throw std::runtime_error("Transient Service not found: " + typeName);
            }

            auto it = map->second.find(tag);
            if (it == map->second.end()) {
                throw std::runtime_error("Transient Service not found: " + typeName);
            }

            auto val = std::static_pointer_cast<TypedService<TInterface>>(it->second);
            return val->CreateService();
        }

        /**
        * @brief Creates a new scope in the Container.
        *
        * This function creates a new scope in the Container. A scope is used to manage the lifetime and sharing of
        * scoped services. Scoped services are created once per scope and are shared among all consumers within that
        * scope. The scope is represented by a std::shared_ptr<Scope> object.
        *
        * @return std::shared_ptr<Scope> The newly created scope.
        */
        std::shared_ptr<Scope> CreateScope() {
            return std::make_shared<Scope>();
        }

        /**
        * @brief Resolves a scoped service from the Container.
        *
        * This function is responsible for resolving a scoped service from the Container.
        * It checks if the service has already been registered in the provided scope. If it has,
        * it returns an empty std::weak_ptr. If the service has not been registered, it looks for
        * the service type in the scopedServices map. If the service is not found, an exception is
        * thrown. If the service is found, it is casted to the TypedService type for the given interface,
        * and the CreateService function is called to retrieve the instance of the service. The new
        * service is then added to the services map of the provided scope, and a std::weak_ptr to the
        * service is returned.
        *
        * @param scope The scope in which the service is resolved.
        *
        * @return std::weak_ptr<TInterface> A weak_ptr to the resolved scoped service.
        *
        * @throw std::runtime_error if the service was not registered.
        */
        template<typename TInterface>
        std::weak_ptr<TInterface> ResolveScoped(std::shared_ptr<Scope> &scope, std::string tag = "") {
            std::string typeName = typeid(TInterface).name();

            // If the scope already has the service, we don't create a new one
            if (scope->services.count(typeName))
                return {};

            auto map = scopedServices.find(typeName);
            if (map == scopedServices.end()) {
                throw std::runtime_error("Service was not registered: " + typeName);
            }

            auto it = map->second.find(tag);
            if (it == map->second.end()) {
                throw std::runtime_error("Service was not found: " + typeName);
            }

            auto val = std::static_pointer_cast<TypedService<TInterface>>(it->second);
            auto newService = val->CreateService();
            scope->services[typeName] = newService;

            return std::weak_ptr<TInterface>(newService);
        }

    private:
        Container() = default;

        std::unordered_map<std::string, MapType> scopedServices;
        std::unordered_map<std::string, MapType> singletonServices;
        std::unordered_map<std::string, MapType> transientServices;
    };

}

#endif //INJECTTORTEST_CONTAINER_HPP