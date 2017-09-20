/*
 * Copyright (C) 2017 massimo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/* 
 * File:   object-pool-example.cpp
 * Author: massimo
 * 
 * Created on April 12, 2017, 8:17 PM
 */

#include "objectPool.h"
#include <cstdint>
#include <iostream>
#include <thread>
#include <future>
#include <exception>
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////
auto main() -> int
{
  std::clog << "\n[" << __func__ << "] "
            << "Object Pool Example STARTED"
            << std::endl;
    
  std::clog << "\n[" << __func__ << "] "
            << "TERMINATED"
            << std::endl;
}  // main
