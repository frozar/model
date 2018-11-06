
#pragma once

class ThreadSettingInterface
{
public:
  virtual unsigned GetNbThread() const = 0;
  virtual void SetNbThread(unsigned v) = 0;

  static ThreadSettingInterface& instance();

protected:
  ThreadSettingInterface() {}
};

