#include "base_swd_adapter.h"

namespace adapter
{
  BaseSwdAdapter::BaseSwdAdapter()
  {
    
  }
  
  BaseSwdAdapter::BaseSwdAdapter(swd::ISwdBus * swdBus)
  {
    this->swdBus = swdBus;  // ссылка, не новый экземпляр
  }
  
};
