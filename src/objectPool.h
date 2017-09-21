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
 * File:   objectPool.h
 * Author: massimo
 *
 * Created on April 12, 2017, 8:17 PM
 */

#ifndef COBJECTPOOL_H
#define COBJECTPOOL_H

#include "objectCreator.h"
#include <cstddef>
#include <queue>
#include <stdexcept>
#include <memory>
#include <tuple>
#include <utility>
////////////////////////////////////////////////////////////////////////////////
namespace object_pool
{
//
// This code is a fork & extension from the following code:
//
//  http://media.wiley.com/product_ancillary/50/11188580/DOWNLOAD/c25_code.zip
//
// From the original source file:
// --- cit ON
// Provides an object pool that can be used with any class that provides a
// default constructor.
//
// The object pool constructor creates a pool of objects, which it hands out
// to clients when requested via the acquireObject() method. acquireObject()
// returns an Object which is a std::shared_ptr with a custom deleter that
// automatically puts the object back into the object pool when the shared_ptr
// is destroyed and its reference reaches 0.
//
// The constructor and destructor on each object in the pool will be called only
// once each for the lifetime of the program, not once per acquisition and release.
//
// The primary use of an object pool is to avoid creating and deleting objects
// repeatedly. The object pool is most suited to applications that use large 
// numbers of objects with expensive constructors for short periods of time, if
// a profiler tells you that allocating and deallocating these objects is a
// bottleneck.
// --- cit OFF
//
// It is also possible to create an object pool providing a non-default ctor for
// the objects that is registered and used at any new allocation when the pool
// is empty
////////////////////////////////////////////////////////////////////////////////
// Base class that does not depend on the class template
class objectPoolBase
{
 protected:
  static const int64_t m_kDefaultPoolSize;
  static const int64_t m_kdefaultHardLimitMaxObjects;
  size_t m_poolSize;
  size_t m_HardLimitMaxObjects;

 public:
  // delegating ctor
  explicit objectPoolBase();
  explicit objectPoolBase(const int64_t poolSize,
                          const int64_t hardLimitMaxObjects) noexcept(false);
  ~objectPoolBase() = default;
  // Prevent assignment and pass-by-value
  objectPoolBase(const objectPoolBase& src) = delete;
  objectPoolBase& operator=(const objectPoolBase& rhs) = delete;
};  // objectPoolBase

template <typename T>
class objectPool : public objectPoolBase
{
public:
  explicit objectPool() noexcept(false) :
  objectPool(m_kDefaultPoolSize)
  {}

  ~objectPool() = default;

  // Prevent assignment and pass-by-value
  objectPool(const objectPool& src) = delete;
  objectPool& operator=(const objectPool& rhs) = delete;

  // Create an object pool with poolSize objects.
  // Whenever the object pool runs out of objects, poolSize more objects will be
  // added to the pool.
  // The pool only grows: Objects are never removed from the pool (freed),
  // until the pool is destroyed.
  //
  // Throw invalid_argument if poolSize or hardLimitMaxObjects is <= 0 or if
  // poolSize > hardLimitMaxObjects
  // Throw bad_alloc if allocation fails
  explicit objectPool(const int64_t poolSize,
                      const int64_t hardLimitMaxObjects = m_kdefaultHardLimitMaxObjects) noexcept(false)
  :
  objectPoolBase(poolSize, hardLimitMaxObjects)
  {
    // Create poolSize objects to start
    allocatePool();
  }

  // this ctor is used when a non-default ctor for T is provided as an object_creator::object_creator_fun<T> f
  explicit objectPool(object_creator::object_creator_fun<T> f,
                      const int64_t poolSize,
                      const int64_t hardLimitMaxObjects = m_kdefaultHardLimitMaxObjects) noexcept(false)
  :
  objectPoolBase(poolSize, hardLimitMaxObjects),
  m_f(f)
  {
    // Create poolSize objects to start
    allocatePool();
  }
          
  // The type of smart pointer returned by acquireObject()
  using Object = std::shared_ptr<T>;

  // Reserve an object for use
  auto acquireObject() const noexcept(false) -> std::tuple<Object, bool>
  {
    bool isObjectOverflow {};

    if ( m_FreeList.empty() )
    {
      std::tie(std::ignore, std::ignore, isObjectOverflow) = allocatePool();
    }

    // Move next free object from the queue to a local unique_ptr
    std::unique_ptr<T> obj(std::move(m_FreeList.front()));
    m_FreeList.pop();

    // Convert the object pointer to an Object (a shared_ptr with a custom deleter)
    Object smartObject(obj.release(),
            [this](T* t)
            {
              // The custom deleter doesn't actually deallocate the memory,
              // but simply puts the object back on the free list
              m_FreeList.push(std::unique_ptr<T>(t));
            } );

    // Return the tuple {Object smartObject, bool isObjectOverflow}
    return std::make_tuple(smartObject,
                           isObjectOverflow );
  }

  size_t getFreeListSize() const noexcept
  {
    return m_FreeList.size();
  }

  size_t getNumberOfObjectsCreated() const noexcept
  {
    return m_objectsCreated;
  }

  bool checkObjectsOverflow () const noexcept
  {
    return (getNumberOfObjectsCreated() > m_HardLimitMaxObjects);
  }

 private:
  mutable size_t m_objectsCreated {};

  // the lambda that invokes a non-default ctor registered at object pool
  // creation; it's nullptr if not registered
  object_creator::object_creator_fun<T> m_f {};

  // m_FreeList stores the objects that are not currently in use by clients
  mutable std::queue<std::unique_ptr<T>> m_FreeList {};

  // Allocates m_poolSize new objects and adds them to m_FreeList
  auto allocatePool() const noexcept(false) -> std::tuple<size_t, size_t, bool>
  {
    // generate a lambda that will invoke the right ctor
    std::function<std::unique_ptr<T>()> lambdaCtor {};
    if (m_f)
    {
      // this lambda invokes the (non-default) ctor registered at object pool creation
      lambdaCtor = [this]() -> std::unique_ptr<T>
                   {
                     return m_f();
                   };
    }
    else
    {
      // this lambda invokes the default ctor
      lambdaCtor = []() -> std::unique_ptr<T>
                   {
                     return std::make_unique<T>();
                   };
    }

    for (size_t i = 0; i < m_poolSize; ++i)
    {
      m_FreeList.emplace(lambdaCtor());
      ++m_objectsCreated;
    }

    return std::make_tuple(getFreeListSize(),
                           getNumberOfObjectsCreated(),
                           checkObjectsOverflow() );
  }
};  // class ObjectPool
}  // namespace object_pool

#endif  // COBJECTPOOL_H
