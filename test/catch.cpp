/// @file catch.cpp
///
/// Catch testing can take a long time to rebuild.  Using a common driver and
/// then linking against it reduces build time.
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
