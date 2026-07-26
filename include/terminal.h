
#pragma once
#include <utility>

void enable_raw_mode();
void disable_raw_mode();

char get_key();

std::pair<int,int> get_terminal_dimension();

