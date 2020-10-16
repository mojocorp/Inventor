#pragma once

#ifndef _WIN32
#error "This unistd.h compat header should be included on Windows only!"
#endif

#include <getopt.h>
#include <corecrt_io.h> //isatty
