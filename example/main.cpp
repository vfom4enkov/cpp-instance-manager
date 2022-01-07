#include <cpptoolkit/factory/core.h>

#include <iostream>

#include "root.h"
#include "registrator.h"

namespace cf = cpptoolkit::factory;

int main() {
  std::string error;
  auto core = example::RegisterObjects(error);
  if (!core) {
    std::cout << error << std::endl;
    return -1;
  }

  {
    const auto root = core->GetShared<example::Root>();
    root->Action();
  }

  std::cout << "End program" << std::endl;
}

//	AbstractDefault::Constructor
//	Single::Constructor
//	AbstractCustom::Constructor
//	Pool::Constructor
//	Root::Constructor
//	Root::Action
//	Pool::Action
//	AbstractCustom::Action
//	Single::Action
//	AbstractDefault::Action
//	Root::Destructor
//	End the program
//	Single::Destructor
//	AbstractDefault::Destructor
//	Pool::Destructor
//	AbstractCustom::Destructor
