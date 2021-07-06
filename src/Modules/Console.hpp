#pragma once
#include "Module.hpp"

#include "Tier1.hpp"

#ifdef _WIN32
#define TIER0 "tier0"
#define CONCOLORMSG_SYMBOL "?ConColorMsg@@YAXABVColor@@PBDZZ"
#define DEVMSG_SYMBOL "?DevMsg@@YAXPBDZZ"
#define DEVWARNINGMSG_SYMBOL "?DevWarning@@YAXPBDZZ"
#else
#define TIER0 "libtier0"
#define CONCOLORMSG_SYMBOL "_Z11ConColorMsgRK5ColorPKcz"
#define DEVMSG_SYMBOL "_Z6DevMsgPKcz"
#define DEVWARNINGMSG_SYMBOL "_Z10DevWarningPKcz"
#endif

#define MSG_SYMBOL "Msg"
#define WARNING_SYMBOL "Warning"
#define COMMANDLINE_SYMBOL "CommandLine"

#define CHAOS_PRINT_COLOR Color(4, 197, 211)
#define CHAOS_PRINT_ACTIVE_COLOR Color(110, 247, 76)

class Console : public Module {
public:
    using _Msg = void(__cdecl*)(const char* pMsgFormat, ...);
    using _Warning = void(__cdecl*)(const char* pMsgFormat, ...);
    using _ColorMsg = void(__cdecl*)(const Color& clr, const char* pMsgFormat, ...);
    using _DevMsg = void(__cdecl*)(const char* pMsgFormat, ...);
    using _DevWarning = void(__cdecl*)(const char* pMsgFormat, ...);
    using _CommandLine = ICommandLine * (__cdecl*)();

    /**
     * @param pMsgFormat message to log
     */
    _Msg Msg = nullptr;
	/**
	 * @param clr color of the text
	 * @param pMsgFormat message to log
	 */
    _ColorMsg ColorMsg = nullptr;
	/**
	 * @param pMsgFormat message to log
	 */
    _Warning Warning = nullptr;
	/**
	 * @param pMsgFormat message to log
	 */
    _DevMsg DevMsg = nullptr;
	/**
	 * @param pMsgFormat message to log
	 */
    _DevWarning DevWarning = nullptr;
	/**
	 * Getter for the CommandLine object
	 */
    _CommandLine CommandLine = nullptr;

    Console();
    bool Init() override;
    void Shutdown() override;
    const char* Name() override { return MODULE(TIER0); }

    /**
     * prints a message to console (?)
     * @tparam T
     * @param fmt text to print
     * @param args
     */
    template <typename... T>
    void Print(const char* fmt, T... args)
    {
        this->ColorMsg(CHAOS_PRINT_COLOR, fmt, args...);
    }

    /**
     * prints a message to console (?)
     * @tparam T
     * @param fmt text to print
     * @param args
     */
    template <typename... T>
    void PrintActive(const char* fmt, T... args)
    {
        this->ColorMsg(CHAOS_PRINT_ACTIVE_COLOR, fmt, args...);
    }
};

extern Console* console;
