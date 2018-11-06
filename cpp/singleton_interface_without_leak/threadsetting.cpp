// Documentation link:
// https://sourcemaking.com/design_patterns/singleton/cpp/1
// https://codereview.stackexchange.com/questions/197486/singleton-class-and-correct-way-to-access-it-in-c?newreg=c163b1884cf34c9dbb697f41e91ede9e

#include "threadsettinginterface.h"
#include <memory>

// This class is designed to instanciate of singleton object
class ThreadSetting : public ThreadSettingInterface
{
  unsigned m_nbThread;
  static const unsigned m_nbThreadMax = 4;

  // As the ctor is private, is class cannot be instanciate else than through
  // its static method 'instance()'
  ThreadSetting(unsigned v = 0)
  {
    SetNbThread(v);
  }

  ~ThreadSetting()
  {}

public:
  ThreadSetting(const ThreadSetting&) = delete;
  ThreadSetting(ThreadSetting&&) = delete;
  ThreadSetting& operator=(const ThreadSetting&) = delete;
  ThreadSetting& operator=(ThreadSetting&&) = delete;

  unsigned GetNbThread() const
  {
    return m_nbThread;
  }
  void SetNbThread(unsigned v)
  {
    if (v == 0)
      v = m_nbThreadMax;

    m_nbThread = (v <= m_nbThreadMax) ? v : m_nbThreadMax;
  }
  
  static ThreadSetting& instance()
  {
    static ThreadSetting instance;
    return instance;
  }
};

ThreadSettingInterface& ThreadSettingInterface::instance()
{
  return ThreadSetting::instance();
}
