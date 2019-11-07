#pragma once
struct Cell
{
	bool up = false
		, right = false
		, down = false
		, left = false;

	unsigned steps = 0;
	bool isAssigned = false;

	size_t nbUp = 0;
	size_t nbRight = 0;
	size_t nbDown = 0;
	size_t nbLeft = 0;
};