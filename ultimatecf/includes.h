#pragma once

#include <Windows.h>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <random>
#include <iphlpapi.h>
#include <cctype>
#include <locale>
#include <thread>
#include <chrono>
#include <d3d9.h>
#include <intrin.h>

#include "xorstr.hpp"
#include "fnv1.hpp"

#include "d3d.h"

#include "detours.h"
#include "sdk.h"
#include "interfaces.h"
#include "tools.h"
#include "math.h"
#include "game.h"
#include "hacks.h"
#include "esp.h"
#include "aimbot.h"

#include "Menu.h"

#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "detours.lib" )
#pragma comment(lib, "IPHLPAPI.lib")

using namespace std::chrono_literals;