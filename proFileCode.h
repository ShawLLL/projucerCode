/**
 * This file is part of the Three-Body Technology code base.
 * Copyright(C), 2017-2027, Three-Body Technology, all rights reserved.
 * 
 * This file is protected by the copyright law of the people's Republic of China.
 * Permission to use this file is only granted for the employee of Three-Body
 * Technology, or other company/group/individual who has expressly authorized by
 * Three-Body Technology.
 * 
 * 此文件属于Three-Body Technology公司代码库.
 * 版权所有 2017-2027, Three-Body Technology
 * 
 * 此文件受中华人民共和国软件著作权法保护, 使用者仅限Three-Body Technology公司员工,
 * 以及由Three-Body Technology公司明确授权的公司/组织/个人.
 * 
 * @author 萌克
 * @date   2019年10月17日14点48分
 */


#pragma once


#include <JuceHeader.h>


namespace ktv {



#if 1 //KTV_ENABLE_PROFILING
#if JUCE_MSVC

class ScopedProfiling
{
public:

    ScopedProfiling( String name );

    ~ScopedProfiling();

    static String printAllProfiles();
};

# define KTV_PROFILING const ScopedProfiling JUCE_JOIN_MACRO(profile,__LINE__){ __FUNCTION__ }
#else
# define KTV_PROFILING const ScopedProfiling JUCE_JOIN_MACRO(profile,__LINE__){ __PRETTY_FUNCTION__ }
#endif
#else
# define KTV_PROFILING
#endif



} // end of namespace ktv