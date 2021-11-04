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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <TString.h>

#include <map>

struct CanvasCfg
{
	bool override{false};
// 	std::string cn;
	int cnt;
	int w;
	int h;
};

enum FilterState { FS_None, FS_Modify, FS_Exclusive };
typedef std::map<std::string, CanvasCfg> FilterMap;

FilterState parser(const std::string & fname, FilterMap & local_fm, const CanvasCfg & default_cfg);
TString generate_cfg_name(const TString & str);

#endif // FUNCTIONS_H
