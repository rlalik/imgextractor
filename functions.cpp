/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "functions.h"

#include <iostream>
#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";

TString generate_cfg_name(const TString & str)
{
	TString par = str;

	if (par.EndsWith(".root"))
		par.ReplaceAll(".root", ".iecfg");
	else
		par.Append(".iecfg");

	int pos = par.Last('/');

	if (pos == -1)
	{
		par.Prepend('.');
	}
	else
	if (pos < par.Length())
	{
		par.Insert(pos+1, '.');
	}

	return par;
}