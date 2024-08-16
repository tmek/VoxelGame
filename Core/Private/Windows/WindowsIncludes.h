#pragma once

// if you include windef.h before windows.h u get Error C1189 : #error:  "No Target Architecture"

// lean and mean removes some of the less used APIs
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
