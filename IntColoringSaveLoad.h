#pragma once

#include <vector>
#include <unordered_set>
#include <cstdio>

namespace IntColoringSaveLoad
{
    void Save(const std::vector<std::unordered_set<int>> &p, int numColor, FILE *fp);
	std::vector<std::unordered_set<int>> Load(FILE *fp);

};