/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "../objectCreator.h"
#include "../objectPool.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace ::testing;
////////////////////////////////////////////////////////////////////////////////
// Tests
TEST (objectCreator, test_1)
{
  class A
  {
    mutable int _x{};
    mutable int _y{};
    mutable int _z{};

   public:
    explicit
    A() noexcept
    {
      std::cout << "default constructor A(): ";
      display_object();
    }

    explicit
    A(const int x) noexcept
    :
    _x(x)
    {
      std::cout << "constructor-1 A(x): ";
      display_object();
    }

    explicit
    A(const int x, const int y) noexcept
    :
    _x(x),
    _y(y)
    {
      std::cout << "constructor-2 A(x, y): ";
      display_object();
    }

    explicit
    A(const int x, const int y, const int z) noexcept
    :
    _x(x),
    _y(y),
    _z(z)
    {
      std::cout << "constructor-3 A(x, y, z): ";
      display_object();
    }

    int get_x() const noexcept
    {
      return _x;
    }
    int get_y() const noexcept
    {
      return _y;
    }
    int get_z() const noexcept
    {
      return _z;
    }

    void set_x(const int x) const noexcept
    {
      _x = x;
    }
    void set_y(const int y) const noexcept
    {
      _y = y;
    }
    void set_z(const int z) const noexcept
    {
      _z = z;
    }

    void display_object (const std::string& prompt = "") const noexcept
    {
      std::cout << prompt
                << "(x, y, z): ("
                << get_x()
                << ", "
                << get_y()
                << ", "
                << get_z()
                << ")"
                << std::endl;
    }

    A(const A& rhs) = delete;
    A& operator=(const A& rhs) = delete;

    ~A()
    {
      std::cout << "Destroyed object: ";
      display_object();
    }
  };  // class A

  using Object = std::unique_ptr<A>;
  
  object_creator::object_creator_fun<A> l {};

  l = object_creator::create_object_creator_fun<A>();
  Object o0 = l();

  {
    l = object_creator::create_object_creator_fun<A, const int>(123);
  }
  Object o1 = l();

  l = object_creator::create_object_creator_fun<A,const int>(456);
  Object o2 = l();

  l = object_creator::create_object_creator_fun<A,const int, const int>(11, 22);
  Object o3 = l();

  l = object_creator::create_object_creator_fun<A,const int, const int, const int>(11, 22, 33);
  Object o4 = l();

  ASSERT_EQ(o0.get()->get_x(), 0);
  ASSERT_EQ(o0.get()->get_y(), 0);
  ASSERT_EQ(o0.get()->get_z(), 0);
  ASSERT_EQ(o1.get()->get_x(), 123);
  ASSERT_EQ(o1.get()->get_y(), 0);
  ASSERT_EQ(o1.get()->get_z(), 0);
  ASSERT_EQ(o2.get()->get_x(), 456);
  ASSERT_EQ(o2.get()->get_y(), 0);
  ASSERT_EQ(o2.get()->get_z(), 0);
  ASSERT_EQ(o3.get()->get_x(), 11);
  ASSERT_EQ(o3.get()->get_y(), 22);
  ASSERT_EQ(o3.get()->get_z(), 0);
  ASSERT_EQ(o4.get()->get_x(), 11);
  ASSERT_EQ(o4.get()->get_y(), 22);
  ASSERT_EQ(o4.get()->get_z(), 33);

//  o0.get()->display_object("o0: ");
//  o1.get()->display_object("o1: ");
//  o2.get()->display_object("o2: ");
//  o3.get()->display_object("o3: ");
//  o4.get()->display_object("o4: ");

  {
    l = object_creator::create_object_creator_fun<A, const int, const int, const int>(99, 88, 77);

    std::vector<Object> v {};
    for (int i = 1; i <= 5; ++i)
    {
      // create an object that can be used in this scope only
      Object o = l();
      // store the object in the vector v using move semantics;
      // the lifetime of the object is extended over this scope
      v.push_back(std::move(o));
    }
    // all elements in the vector have the same attribute values
    for (auto&& item : v)
    {
      ASSERT_EQ(item.get()->get_x(), 99);
      ASSERT_EQ(item.get()->get_y(), 88);
      ASSERT_EQ(item.get()->get_z(), 77);
    }
  }  // the objects in the vector v are destroyed, going out of scope
}

TEST(objectPool, test_1)
{
  EXPECT_THROW(object_pool::objectPool<bool> aPool(0), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<bool> aPool(-1), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<int8_t> aPool(0), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<int8_t> aPool(-1), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<int16_t> aPool(0), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<int16_t> aPool(-1), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<int32_t> aPool(0), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<int32_t> aPool(-1), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<int64_t> aPool(0), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<int64_t> aPool(-1), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<uint8_t> aPool(0), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<uint8_t> aPool(-1), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<uint16_t> aPool(0), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<uint16_t> aPool(-1), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<uint32_t> aPool(0), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<uint32_t> aPool(-1), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<uint64_t> aPool(0), std::invalid_argument);
  EXPECT_THROW(object_pool::objectPool<uint64_t> aPool(-1), std::invalid_argument);
}

TEST(objectPool, test_1A)
{
  EXPECT_NO_THROW(object_pool::objectPool<bool> aPool(10, 50));
  EXPECT_NO_THROW(object_pool::objectPool<int8_t> aPool(20, 2'000));
  EXPECT_NO_THROW(object_pool::objectPool<int16_t> aPool(30, 3'000));
  EXPECT_NO_THROW(object_pool::objectPool<int32_t> aPool(40));
  EXPECT_NO_THROW(object_pool::objectPool<int64_t> aPool(50, 6'000));
  EXPECT_NO_THROW(object_pool::objectPool<uint8_t> aPool(60));
  EXPECT_NO_THROW(object_pool::objectPool<uint16_t> aPool(70, 7'000));
  EXPECT_NO_THROW(object_pool::objectPool<uint32_t> aPool(80));
  EXPECT_NO_THROW(object_pool::objectPool<uint64_t> aPool(90, 9'000));
}

TEST(objectPool, test_2)
{
  class CA
  {
    int m_x {};

  public:
    CA() : m_x(8) {}
    ~CA()
    {
      std::clog << "CA dtor called: m_x is " << m_x << std::endl;
    }
  };

  using CAop = object_pool::objectPool<CA>;
  EXPECT_THROW(CAop CAPool(0), std::invalid_argument);
  EXPECT_THROW(CAop CAPool(-1), std::invalid_argument);
  EXPECT_THROW(CAop CAPool(-1000), std::invalid_argument);
  EXPECT_THROW(CAop CAPool(10, -10'000), std::invalid_argument);
  EXPECT_THROW(CAop CAPool(-10, 10'000), std::invalid_argument);
  EXPECT_THROW(CAop CAPool(-10, -10'000), std::invalid_argument);
  EXPECT_THROW(CAop CAPool(100, 10), std::invalid_argument);
  EXPECT_NO_THROW(CAop CAPool(10, 10'000));
}

TEST(objectPool, test_3)
{
  static const auto poolSize {1'000};
  static auto ctorCalls {0};
  static auto dtorCalls {0};

  class CA
  {
    int m_id {};

  public:
    CA()
    :
    m_id(++ctorCalls)
    {
      if ( m_id < poolSize )
      {
        std::clog << "Constructed object " << m_id << "\r";
        return;
      }
      std::clog << "Constructed object " << m_id << std::endl;
    }

    ~CA()
    {
      ++dtorCalls;
      if ( m_id < poolSize )
      {
        std::clog << "Destroyed object   " << m_id << "\r";
        return;
      }
      std::clog << "Destroyed object   " << m_id << std::endl;
    }
  };

  using CAop = object_pool::objectPool<CA>;

  EXPECT_NO_THROW(CAop CAPool(poolSize));
  ASSERT_EQ(poolSize, ctorCalls);
  ASSERT_EQ(poolSize, dtorCalls);
}

TEST(objectPool, test_4)
{
  // Let's create a pool of int's
  using int_op = object_pool::objectPool<int>;

  const auto poolSize {1};
  const auto hardMaxObjectsLimit {1};

  // the pool has 1 object, and 1 as the hard max objects limit
  int_op iPool(poolSize, hardMaxObjectsLimit);

  // check the intial conditions
  ASSERT_EQ(poolSize, iPool.getFreeListSize());
  ASSERT_EQ(poolSize, iPool.getNumberOfObjectsCreated());
  ASSERT_EQ(false, iPool.checkObjectsOverflow());

  {
    // let's get an object from the pool
    // C++17
    auto [o, of] = iPool.acquireObject();
    // Equivalent to:
    // int_op::Object o;
    // bool of{};
    // std::tie(o, of) = iPool.acquireObject();

    // check the pool of free elements is empty now
    ASSERT_EQ(0, iPool.getFreeListSize());  
    ASSERT_EQ(poolSize, iPool.getNumberOfObjectsCreated());
    ASSERT_EQ(false, iPool.checkObjectsOverflow());
    ASSERT_EQ(of, iPool.checkObjectsOverflow());
    ASSERT_EQ(false, of);

    // reference to the value
    auto& vr = *(o.get());
    // copying the value in v is allowed
    auto v = *(o.get());

    ASSERT_EQ(0, *(o.get()));
    ASSERT_EQ(0, vr);
    ASSERT_EQ(0, v);

    // change the referenced value
    vr = 123;
    // check it's changed
    ASSERT_EQ(123, *(o.get()));
    ASSERT_EQ(123, vr);
    // check it's not changed here
    ASSERT_EQ(0, v);
  }  // Object o and bool of go out of scope

  // The Object o has been restored in the pool
  // Check the free list pool is not empty now
  ASSERT_EQ(poolSize, iPool.getFreeListSize());  
  ASSERT_EQ(poolSize, iPool.getNumberOfObjectsCreated());
  ASSERT_EQ(false, iPool.checkObjectsOverflow());

  // pool destroyed here when aPool goes out of scope
  std::clog << "Object pool being destroyed now... total objects in the pool: "
            << iPool.getNumberOfObjectsCreated()
            << std::endl;
}

TEST(objectPool, test_5)
{
  const int initDefaultValue {2};

  class CA
  {
    int m_x {};

  public:
    // By default the attribute is set to initDefaultValue
    explicit CA() : m_x(initDefaultValue) {}

    ~CA()
    {
      std::clog << "CA dtor called - m_x set to " << m_x << std::endl;
    }

    CA(const CA& rhs) = delete;
    CA& operator=(const CA& rhs) = delete;

    int get_x () const noexcept
    {
      return m_x;
    }

    void set_x(const int x) noexcept
    {
      m_x = x;
    }
  };  // class CA

  // Let's create a pool of CA's objects
  using ca_op = object_pool::objectPool<CA>;

  const auto poolSize {1};
  const auto hardMaxObjectsLimit {1};

  // the pool has 1 object, and 1 as the hard max objects limit
  ca_op caPool(poolSize,hardMaxObjectsLimit);

  // check the intial conditions
  ASSERT_EQ(poolSize, caPool.getFreeListSize());
  ASSERT_EQ(poolSize, caPool.getNumberOfObjectsCreated());
  ASSERT_EQ(false, caPool.checkObjectsOverflow());

  {
    // let's get an object from the pool
    // C++17
    auto [o1, of1] = caPool.acquireObject();
    // Equivalent to:
    // ca_op::Object o1;
    // bool of1{};
    // std::tie(o1, of1) = caPool.acquireObject();

    // check the pool is empty now
    ASSERT_EQ(0, caPool.getFreeListSize());  
    ASSERT_EQ(poolSize, caPool.getNumberOfObjectsCreated());
    ASSERT_EQ(false, caPool.checkObjectsOverflow());
    ASSERT_EQ(of1, caPool.checkObjectsOverflow());
    ASSERT_EQ(false, of1);

    // let's try to get another object from the pool
    // since the pool is empty a new pool of size 1 is allocated
    auto [o2, of2] = caPool.acquireObject();

    // check the free list pool is still empty
    ASSERT_EQ(0, caPool.getFreeListSize());
    // and a new object was created
    ASSERT_EQ(2, caPool.getNumberOfObjectsCreated());
    // and we are in an overflow condition since the hard max objects limit is 1
    ASSERT_EQ(true, caPool.checkObjectsOverflow());
    ASSERT_EQ(of2, caPool.checkObjectsOverflow());
    ASSERT_EQ(true, of2);
    
    // reference to the value of o1
    auto& vr1 = *(o1.get());
    // copying the value in v1 is not allowed
    //auto v1 = *(o1.get());

    ASSERT_EQ(initDefaultValue, o1.get()->get_x() );
    ASSERT_EQ(initDefaultValue, vr1.get_x() );

    vr1.set_x(345);

    ASSERT_EQ(345, o1.get()->get_x() );
    ASSERT_EQ(345, vr1.get_x() );
  }  // Objects o1, o2 and bool's of1, of2 go out of scope

  // The Objects o1, o2 have been restored in the pool
  // Check the free list pool is not empty now
  ASSERT_EQ(2, caPool.getFreeListSize());  
  ASSERT_EQ(2, caPool.getNumberOfObjectsCreated());
  ASSERT_EQ(true, caPool.checkObjectsOverflow());

  // pool destroyed here when aPool goes out of scope
  std::clog << "Object pool being destroyed now... total objects in the pool: "
            << caPool.getNumberOfObjectsCreated()
            << std::endl;
}

TEST(objectPool, test_6)
{
  const int initDefaultValue {7};

  class CA
  {
    int m_x {};

  public:
    // By default the attribute is set to initDefaultValue
    explicit CA() : m_x(initDefaultValue) {}

    ~CA()
    {
      std::clog << "CA dtor called - m_x set to " << m_x << std::endl;
    }

    CA(const CA& rhs) = delete;
    CA& operator=(const CA& rhs) = delete;

    int get_x () const noexcept
    {
      return m_x;
    }

    void set_x(const int x) noexcept
    {
      m_x = x;
    }
  };  // class CA

  // Let's create a pool of CA's objects
  using ca_op = object_pool::objectPool<CA>;

  const auto poolSize {1};
  const auto hardMaxObjectsLimit {100};

  // the pool has 1 object, and 100 as the hard max objects limit
  ca_op caPool(poolSize,hardMaxObjectsLimit);

  // check the initial conditions
  ASSERT_EQ(poolSize, caPool.getFreeListSize());
  ASSERT_EQ(poolSize, caPool.getNumberOfObjectsCreated());
  ASSERT_EQ(false, caPool.checkObjectsOverflow());

  {
    std::vector<ca_op::Object> v {};

    // Acquire 10 objects from the pool and push back them in vector v
    // Since the object pool has been created with 1 object, 9 more objects will
    // be created
    for (int i = 1; i <= 10; ++i)
    {
      auto [o1, of1] = caPool.acquireObject();
      v.push_back(o1);
    }
  }  // Vector v goes out of scope and all 10 objects are restored back in the pool

  // The Objects have been restored in the pool
  // Check the free list pool is not empty now
  ASSERT_EQ(10, caPool.getFreeListSize());  
  ASSERT_EQ(10, caPool.getNumberOfObjectsCreated());
  ASSERT_EQ(false, caPool.checkObjectsOverflow());

  // pool destroyed here when aPool goes out of scope
  std::clog << "Object pool being destroyed now... total objects in the pool: "
            << caPool.getNumberOfObjectsCreated()
            << std::endl;
}

TEST(objectPool, test_7)
{
  const int initDefaultValue {88};

  class CA
  {
    int m_x {};

  public:
    // By default the attribute is set to initDefaultValue
    explicit CA() : m_x(initDefaultValue) {}

    ~CA()
    {
      std::clog << "CA dtor called - m_x set to " << m_x << std::endl;
    }

    CA(const CA& rhs) = delete;
    CA& operator=(const CA& rhs) = delete;

    int get_x () const noexcept
    {
      return m_x;
    }

    void set_x(const int x) noexcept
    {
      m_x = x;
    }
  };  // class CA

  // Let's create a pool of CA's objects
  using ca_op = object_pool::objectPool<CA>;

  const auto poolSize {2};
  const auto hardMaxObjectsLimit {1'000};

  // the pool has 1 object, and 1000 as the hard max objects limit
  ca_op caPool(poolSize,hardMaxObjectsLimit);

  // check the initial conditions
  ASSERT_EQ(poolSize, caPool.getFreeListSize());
  ASSERT_EQ(poolSize, caPool.getNumberOfObjectsCreated());
  ASSERT_EQ(false, caPool.checkObjectsOverflow());

  const int objectsToAcquire {2};

  {
    std::vector<ca_op::Object> v {};

    // Acquire 1500 objects from the pool and push back them in vector v
    // Since the object pool has been created with 1 object, 1499 more objects will
    // be created
    for (int i = 1; i <= objectsToAcquire; ++i)
    {
      auto [o1, of1] = caPool.acquireObject();
      // Just change the attribute value to the current index value
      o1.get()->set_x(i);
      // Push the object in the vector
      v.push_back(o1);
    }
  }  // Vector v goes out of scope and all 1500 objects are restored back in the pool

  // The Objects have been restored in the pool
  // Check the free list pool is not empty now
  ASSERT_EQ(objectsToAcquire, caPool.getFreeListSize());  
  ASSERT_EQ(objectsToAcquire, caPool.getNumberOfObjectsCreated());
  ASSERT_EQ(false, caPool.checkObjectsOverflow());

  // pool destroyed here when aPool goes out of scope
  std::clog << "Object pool being destroyed now... total objects in the pool: "
            << caPool.getNumberOfObjectsCreated()
            << std::endl;
}

TEST (objectPoolWithCreator, test_1)
{
  class A
  {
    mutable std::string _s{};
    mutable int _x{};
    mutable int _y{};
    mutable int _z{};

   public:
    explicit
    A() noexcept
    {
      std::cout << "default constructor A(): ";
      display_object();
    }

    explicit
    A(const std::string& s) noexcept
    :
    _s(s)
    {
      std::cout << "constructor-1 A(s): ";
      display_object();
    }

    explicit
    A(const std::string& s,
      const int& x) noexcept
    :
    _s(s),
    _x(x)
    {
      std::cout << "constructor-1 A(x): ";
      display_object();
    }

    explicit
    A(const std::string& s,
      const int& x,
      const int& y) noexcept
    :
    _s(s),
    _x(x),
    _y(y)
    {
      std::cout << "constructor-2 A(x, y): ";
      display_object();
    }

    explicit
    A(const std::string& s,
      const int& x,
      const int& y,
      const int& z) noexcept
    :
    _s(s),
    _x(x),
    _y(y),
    _z(z)
    {
      std::cout << "constructor-3 A(x, y, z): ";
      display_object();
    }

    std::string get_s() const noexcept
    {
      return _s;
    }
    int get_x() const noexcept
    {
      return _x;
    }
    int get_y() const noexcept
    {
      return _y;
    }
    int get_z() const noexcept
    {
      return _z;
    }

    void set_s(const std::string& s) const noexcept
    {
      _s = s;
    }
    void set_x(const int& x) const noexcept
    {
      _x = x;
    }
    void set_y(const int& y) const noexcept
    {
      _y = y;
    }
    void set_z(const int& z) const noexcept
    {
      _z = z;
    }

    void display_object (const std::string& prompt = "") const noexcept
    {
      std::cout << prompt
                << "(s, x, y, z): ("
                << get_s()
                << ", "
                << get_x()
                << ", "
                << get_y()
                << ", "
                << get_z()
                << ")"
                << std::endl;
    }

    A(const A& rhs) = delete;
    A& operator=(const A& rhs) = delete;

    ~A()
    {
      std::cout << "Destroyed object: ";
      display_object();
    }
  };  // class A

  auto S {"Object Name"};
  auto X {34};
  const auto Y {56};
  auto Z {78};

  object_creator::object_creator_fun<A> l {};
  l = object_creator::create_object_creator_fun<A,
                                                const decltype(S),
                                                const decltype(X),
                                                const decltype(Y),
                                                const decltype(Z)>
          (std::forward<const decltype(S)>(S),
           std::forward<const decltype(X)>(X),
           std::forward<const decltype(Y)>(Y),
           std::forward<const decltype(Z)>(Z));

  // Let's create a pool of A's objects
  using a_op = object_pool::objectPool<A>;

  const auto poolSize {2};
  const auto hardMaxObjectsLimit {10};

  // the pool has 2 object, and 10 as the hard max objects limit
  // the ctor registered is l()
  a_op aPool(l, poolSize, hardMaxObjectsLimit);

  // check the initial conditions
  ASSERT_EQ(poolSize, aPool.getFreeListSize());
  ASSERT_EQ(poolSize, aPool.getNumberOfObjectsCreated());
  ASSERT_EQ(false, aPool.checkObjectsOverflow());

  // get 2 objects from the pool
  auto [o1, of1] = aPool.acquireObject();
  auto [o2, of2] = aPool.acquireObject();

  // check pool conditions
  ASSERT_EQ(poolSize - 2, aPool.getFreeListSize());
  ASSERT_EQ(poolSize, aPool.getNumberOfObjectsCreated());
  ASSERT_EQ(false, aPool.checkObjectsOverflow());
  
  auto& vr1 = *o1.get();

  // check the object's attribute values set by the non-default ctor invoked by l()
  ASSERT_EQ(S, vr1.get_s());
  ASSERT_EQ(X, vr1.get_x());
  ASSERT_EQ(Y, vr1.get_y());
  ASSERT_EQ(Z, vr1.get_z());

  const int objectsToAcquire {50};
  {
    // create a vector of objects
    std::vector<decltype(o1)> v {};
    {
      // acquire objectsToAcquire objects from the pool; since the object pool is
      // empty then objectsToAcquire more objects are created
      for (int i = 1; i <= objectsToAcquire; ++i)
      {
        auto [o, f] = aPool.acquireObject();
        // Just change the attribute value to the current index value
        o.get()->set_x(i);
        v.push_back(o);
      }

      // check the current conditions
      ASSERT_EQ(objectsToAcquire, v.size());
      ASSERT_EQ(0, aPool.getFreeListSize());
      ASSERT_EQ(objectsToAcquire + poolSize, aPool.getNumberOfObjectsCreated());
      ASSERT_EQ(true, aPool.checkObjectsOverflow());
    }
  }  // the vector is freed, then all objects are restored in the object pool

  // check the current conditions
  ASSERT_EQ(objectsToAcquire, aPool.getFreeListSize());
  ASSERT_EQ(objectsToAcquire + poolSize, aPool.getNumberOfObjectsCreated());
  ASSERT_EQ(true, aPool.checkObjectsOverflow());

  // pool destroyed here when aPool goes out of scope
  std::clog << "Object pool being destroyed now... total objects in the pool: "
            << aPool.getNumberOfObjectsCreated()
            << std::endl;
}

//int main(int argc, char **argv) {
//  ::testing::InitGoogleTest(&argc, argv);
//  return RUN_ALL_TESTS();
//}
