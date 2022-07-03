#include <gtest/gtest>
#include <function_info.h>

using namespace simple::type_traits;

TEST(simple_function_info, function_type)
{
    static_assert(function_info<int(int));
}


