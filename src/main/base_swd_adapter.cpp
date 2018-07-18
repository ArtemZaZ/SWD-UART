#include "base_swd_adapter.h"

namespace adapter
{
  BaseSwdAdapter::BaseSwdAdapter() : m_swdBus(0)
  {
    
  }
  
  BaseSwdAdapter::BaseSwdAdapter(swd::ISwdBus * swdBus) : m_swdBus(swdBus)
  {
    
  }
  
};
