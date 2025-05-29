#pragma once

#include "tpsdkbase_Exports.h"

namespace tecplot { 
/**
 * Note:  std::string was avoided because of dll boundary issues with windows.
 */
class tpsdkbase_API MacroDebuggerInterface
{
public:
    // Action Functions
    virtual void        step() = 0;
    virtual void        next() = 0;
    virtual void        out() = 0;
    virtual void        go() = 0;
    virtual void        stop() = 0;
    virtual void        reset() = 0;
    virtual bool        loadMacroFile(char const* fname) = 0;

    // Query Functions
    virtual int         callLevel() const = 0;
    virtual int         curCommandOffset(int callLevel) const = 0;
    virtual int         commandCount(int callLevel) const = 0;
    virtual bool        callLevelName(int callLevel,
                                      char** name) const = 0;
    virtual int         loopDepth() const = 0;
    virtual int         loopValue(int depth) const = 0;
    virtual int         loopEnd(int depth) const = 0;
    virtual bool        isBreakpointSet(int callLevel,
                                        int offset) const = 0;
    virtual bool        anyBreakpointsSet() const = 0;
    virtual bool        isMacroLoaded() const = 0;
    virtual bool        anotherStepIsAvailable() const = 0;
    virtual bool        okToResetMacro() const = 0;
    virtual bool        curCommandIsQuit() const = 0;
    virtual void        macroCommand(int    callLevel,
                                     int    offset,
                                     bool   doExpand,
                                     char** commandString) const = 0;
    virtual bool        macroVarValue(char const* varName,
                                      char**      value) const = 0;
    virtual int         numFunctionParameters(int callLevel) const = 0;
    virtual bool        functionParamValue(int    callLevel,
                                           int    paramNum,
                                           char** value) const = 0;
    virtual int         numUserDefinedMacroVars() const = 0;
    virtual char const* rawMacroVarName(int macroVar) const = 0;

    // Raw macro function/command access (for use with breakpoint management)
    virtual void        rawMacroCommandAccessBegin() const = 0;        // Point to first command in main function
    virtual char const* rawMacroCommandCommandString() const = 0;      // Fetch raw character string for cur command
    virtual char const* rawMacroCommandFunctionString() const = 0;     // Fetch raw character string for cur function name
    virtual bool        rawMacroCommandIsBreakpointSet() const = 0;    // Fetch breakpoint state for cur command
    virtual void        rawMacroCommandSetBreakpoint(bool doSet) = 0;  // set breakpoint state in current command
    virtual bool        rawMacroCommandFetchNextCommand() const = 0;   // fetch the next command in current function
    virtual bool        rawMacroCommandFetchNextFunction() const = 0;  // fetch the next registered function

    // Set functions
    virtual void        setBreakpoint(int  callLevel,
                                      int  offset,
                                      bool doSet) = 0;
    virtual void        setFunctionParamValue(int         callLevel,
                                              int         paramNum,
                                              char const* value) = 0;
    virtual void        setLoopValue(int         depth,
                                     char const* value) = 0;

    virtual ~MacroDebuggerInterface() {} // interfaces need an empty virtual destructor, see coding rules
};

}

//MacroDebugInterface* TecUtilMacroGetDebugger();
