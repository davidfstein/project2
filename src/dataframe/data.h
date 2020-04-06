
//lang::CwC
/*
* Authors: David Stein, Adin Vashi 
* CS4500 A3 (Data definition)
*/

#pragma once
#include "../objects/string.h"

// The accepted data types for a Dataframe.
union Data {
    bool b;
    int i;
    float f;
    double d;
    String* s;
} data;