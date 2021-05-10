#pragma once
#include "Utils/Math.hpp"
#include "Utils/Memory.hpp"
#include "Utils/Platform.hpp"
#include "Utils/SDK.hpp"

#ifndef _WIN32
#include <unistd.h>
#endif

/**
 * return the engine name of this module
 * @param name non-engine name of the module
 */
#define MODULE(name) name MODULE_EXTENSION
#define GAME(x) GAME_PATH(x)

/**
 * Checks if a string ends with another one
 * @param str string to check
 * @param suffix string to search for
 */
static bool ends_with(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && !str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

/**
 * TODO: document
 */
#define REDECL(name) \
    decltype(name) name

#define SAFE_DELETE(ptr) \
    if (ptr) {           \
        delete ptr;      \
        ptr = nullptr;   \
    }

// TODO: wtf is this
#if _WIN32
#define GO_THE_FUCK_TO_SLEEP(ms) Sleep(ms)
#else
#define GO_THE_FUCK_TO_SLEEP(ms) usleep(ms * 1000)
#endif
