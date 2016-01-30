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

#include <sys/stat.h>

#include <string>
#include <iterator>
#include <fstream>
#include <iostream>

#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";

inline bool file_exists(const std::string & file)
{
	struct stat buffer;
	return (stat(file.c_str(), &buffer) == 0);
}

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

FilterState parser(const std::string & fname, FilterMap & local_fm, const CanvasCfg & default_cfg)
{
	// local FilterMap to be add to global one;
	std::string buff;
	std::string inline_name;
	bool parsing_local = false;

	// local setting for current file
	CanvasCfg local_cancfg = default_cfg;

	FilterState fs = FS_Modify;

	if (file_exists(fname))
	{
		std::cout << "Parsing config file " << fname << std::endl;
		std::ifstream local_fm_file(fname.c_str());

		while (!local_fm_file.eof())
		{
			std::getline(local_fm_file, buff);
			if (!buff.length())
				continue;

			// convert tabs into spaces
			size_t tab_pos = 0;
			while (tab_pos = buff.find_first_of('\t', tab_pos), tab_pos != buff.npos)
			{
				buff.replace(tab_pos, 1, " ");
			}

			// find firts non-white character in the line
			// it should be hist name or '*'
			size_t pos_name = buff.find_first_not_of(' ', 0);
			if (pos_name == buff.npos)
				continue;
			else if (buff[pos_name] == '#')
				continue;

			size_t pos = buff.find_first_of(" \t-", pos_name);
			inline_name = buff.substr(pos_name, pos - pos_name).c_str();

			parsing_local = (inline_name == "*");

			// inline setting for current line
			CanvasCfg inline_cancfg = local_cancfg;

			// parse rest of the line to search for w, h, -
			while (pos != buff.npos)
			{
				pos = buff.find_first_not_of(" \t", pos);

				char test_char = buff[pos];

				if (test_char == '-')
				{
					if (parsing_local)
						fs = FS_Exclusive;
					else
						inline_cancfg.cnt = -99;

					++pos;
				}
				else if (test_char == 'w')
				{
					if (buff[pos+1] == '=')
					{
						int old_pos = pos+2;
						pos = buff.find_first_not_of("0123456789", pos+2);
						std::string number = buff.substr(old_pos, pos - old_pos);
						int val_tmp = atoi(number.c_str());
						if (val_tmp)
								inline_cancfg.w = val_tmp;
					}
					else
					{
						std::cerr << "Parsing error, breaking parser" << std::endl;
						continue;
					}
				}
				else if (test_char == 'h')
				{
					if (buff[pos+1] == '=')
					{
						int old_pos = pos+2;
						pos = buff.find_first_not_of("0123456789", pos+2);
						std::string number = buff.substr(old_pos, pos - old_pos);
						int val_tmp = atoi(number.c_str());
						if (val_tmp)
							inline_cancfg.h = val_tmp;
					}
					else
					{
						std::cerr << "Parsing error, breaking parser" << std::endl;
						continue;
					}
				}
				else
				{
					if (pos == buff.npos)
						break;

					std::cerr << "Parsing error at:" << std::endl << " " << buff << std::endl;
					std::fill_n(std::ostream_iterator<char>(std::cerr), pos, ' ');
					std::cerr << "^" << std::endl << " breaking parser" << std::endl;
					break;
				}
			}

			if (parsing_local)
			{
				local_cancfg.w = inline_cancfg.w;
				local_cancfg.h = inline_cancfg.h;
			}

			local_fm[inline_name] = inline_cancfg;
		}
	}

// 	FilterMap & total_map = global_map;
// 	for (FilterMap::const_iterator it = global_map.begin(); it != global_map.end(); ++it)
// 		total_map[it->first] = it->second;

	if (!local_fm.size())
		return FS_None;

	return fs;
}