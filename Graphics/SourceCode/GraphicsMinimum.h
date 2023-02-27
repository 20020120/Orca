#pragma once

namespace Graphics
{
    enum POOL_TYPE
    {
        POOL_TYPE_RES = 0,     // CBV / SRV / UAV
        POOL_TYPE_SMP = 1,     // Sampler
        POOL_TYPE_RTV = 2,     // RTV
        POOL_TYPE_DSV = 3,     // DSV
        POOL_COUNT = 4,
    };
}
