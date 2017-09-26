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
 * File:   objectPool.cpp
 * Author: massimo
 * 
 * Created on April 12, 2017, 8:17 PM
 */
#include "objectPool.h"
////////////////////////////////////////////////////////////////////////////////
namespace object_pool
{
const int64_t objectPoolBase::m_kDefaultPoolSize {10};
const int64_t objectPoolBase::m_kdefaultHighWaterMark {1'000};

objectPoolBase::objectPoolBase()
:
objectPoolBase(m_kDefaultPoolSize, m_kdefaultHighWaterMark)
{}

objectPoolBase::objectPoolBase(const int64_t poolSize,
                               const int64_t highWaterMark) noexcept(false)
:
m_poolSize(static_cast<size_t>(poolSize)),
m_HighWaterMark(static_cast<size_t>(highWaterMark))
{
  if ( poolSize <= 0 )
  {
    throw std::invalid_argument("pool size must be positive");
  }
  if ( highWaterMark <= 0 )
  {
    throw std::invalid_argument("high water mark must be positive");
  }
  if ( poolSize > highWaterMark )
  {
    throw std::invalid_argument("high water mark must be greater than pool size");
  }
}
}  // namespace object_pool
////////////////////////////////////////////////////////////////////////////////
