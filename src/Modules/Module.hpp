#pragma once
#include <vector>

class Module {
public:
    bool hasLoaded;

public:
    virtual ~Module() = default;
    /**
     * Starts this module
     * @return whether this module initialization was successful
     */
    virtual bool Init() = 0;
    /**
     * stops this module
     */
    virtual void Shutdown() = 0;
    /**
     * Get the module name
     */
    virtual const char* Name() = 0;
};

class Modules {
public:
    std::vector<Module*> list;

public:
    Modules();
    /**
     * Registers a module
     * @tparam T module type
     * @param modulePtr pointer to a module
     */
    template <typename T = Module>
    void AddModule(T** modulePtr)
    {
        *modulePtr = new T();
        this->list.push_back(*modulePtr);
    }
    /**
     * Unregisters a module
     * @tparam T module type
     * @param modulePtr pointer to a module
     */
    template <typename T = Module>
    void RemoveModule(T** modulePtr)
    {
        this->list.erase(*modulePtr);
    }
    /**
     * Initialize all registered modules
     */
    void InitAll();
    /**
     * Stops all registered modules
     */
    void ShutdownAll();
    /**
     * Unregister all registered modules
     */
    void DeleteAll();
    ~Modules();
};
