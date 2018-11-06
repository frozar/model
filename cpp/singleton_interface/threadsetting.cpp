// Documentation link:
// https://sourcemaking.com/design_patterns/singleton/cpp/1

#include "threadsettinginterface.h"
#include <memory>

// This class is designed to instanciate of singleton object
class ThreadSetting : public ThreadSettingInterface
{
  unsigned m_nbThread;
  static const unsigned m_nbThreadMax = 4;

  static ThreadSetting* s_instance;

  // As the ctor is private, is class cannot be instanciate else than through
  // its static method 'instance()'
  ThreadSetting(unsigned v = 0)
  {
    SetNbThread(v);
  }

public:
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
  
  static ThreadSetting* instance()
  {
    if (!s_instance)
      s_instance = new ThreadSetting;
    return s_instance;
  }
};

ThreadSettingInterface* ThreadSettingInterface::instance()
{
  return ThreadSetting::instance();
}

// Allocating and initializing ThreadSetting's static data member.
// The pointer is being allocated - not the object inself.
ThreadSetting *ThreadSetting::s_instance = 0;
