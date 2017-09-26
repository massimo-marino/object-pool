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
 * File:   objectCreator.h
 * Author: massimo
 *
 * Created on September 20, 2017, 1:15 PM
 */

#ifndef OBJECTCREATOR_H
#define OBJECTCREATOR_H

#include <memory>
////////////////////////////////////////////////////////////////////////////////
namespace object_creator
{
// create an object of type T and return a std::unique_ptr to it
template <typename T, typename... Args>
auto create_unique_ptr(Args&&... args) -> std::unique_ptr<T>
{
  // since C++14
  return std::make_unique<T>(args...);

  // before C++14:  
  //std::unique_ptr<T> local_ptr(new T(args...));
  //return local_ptr; // local_ptr will surrender ownership;
                    // the compiler should optimize the return as if it was:
                    // return std::move(local_ptr);
}

template <typename T>
using object_creator_fun = std::function<std::unique_ptr<T>(void)>;

template <typename T, typename... Args>
auto create_object_creator_fun(Args&&... args) noexcept -> object_creator_fun<T>
{
  // return a function object for creating T's objects with the given arguments
  // to be passed to its constructor
  return [args...]()
         {
           return create_unique_ptr<T>(std::forward<Args>(args)...);
         };
}
}  // namespace object_creator
#endif /* OBJECTCREATOR_H */

