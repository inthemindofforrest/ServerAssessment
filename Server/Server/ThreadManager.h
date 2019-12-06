#pragma once
#include <thread>

using Tup = void(*)();
using Thup = std::thread*;
void RunThreadWithLoop(Thup _Thread, Tup _Function);