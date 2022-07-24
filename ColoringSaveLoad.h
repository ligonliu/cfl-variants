#pragma once

#include <vector>
#include <unordered_set>
#include <cstdio>

namespace ColoringSaveLoad
{
    void save(const std::vector<std::unordered_set<int>> &p, int numColor, int numFragment, FILE *fp);
	std::vector<std::unordered_set<int>> load(FILE *fp);
};