#pragma once
#include <iostream>
//this is straight out of the claude kitchen :) for logging
#define LOG(...) do { fprintf(stderr, "[mifihook] " __VA_ARGS__); fprintf(stderr, "\n"); } while(0)

