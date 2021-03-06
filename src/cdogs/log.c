/*
    Copyright (c) 2015, Cong Xu
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/
#include "log.h"

#include "utils.h"


static LogLevel llMain = LL_INFO;
static LogLevel llNet = LL_INFO;


const char *LogModuleName(const LogModule m)
{
	switch (m)
	{
		T2S(LM_MAIN, "MAIN");
		T2S(LM_NET, "NET");
	default:
		return "";
	}
}
LogModule StrLogModule(const char *s)
{
	S2T(LM_MAIN, "MAIN");
	S2T(LM_NET, "NET");
	return LM_MAIN;
}

LogLevel LogModuleGetLevel(const LogModule m)
{
	switch (m)
	{
	case LM_MAIN: return llMain;
	case LM_NET: return llNet;
	default:
		CASSERT(false, "Unknown log module");
		return LL_ERROR;
	}
}
void LogModuleSetLevel(const LogModule m, const LogLevel l)
{
	switch (m)
	{
	case LM_MAIN: llMain = l; break;
	case LM_NET: llNet = l; break;
	default:
		CASSERT(false, "Unknown log module");
		break;
	}
}
const char *LogLevelName(const LogLevel l)
{
	switch (l)
	{
		T2S(LL_TRACE, "TRACE");
		T2S(LL_DEBUG, "DEBUG");
		T2S(LL_INFO, "INFO");
		T2S(LL_WARN, "WARN");
		T2S(LL_ERROR, "ERROR");
	default:
		return "";
	}
}
LogLevel StrLogLevel(const char *s)
{
	S2T(LL_TRACE, "TRACE");
	S2T(LL_DEBUG, "DEBUG");
	S2T(LL_INFO, "INFO");
	S2T(LL_WARN, "WARN");
	S2T(LL_ERROR, "ERROR");
	return LL_ERROR;
}