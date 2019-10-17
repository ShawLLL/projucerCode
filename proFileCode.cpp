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
 * @date   2019年10月17日15点06分
 **/


#include "ktv_profile.h"


namespace ktv {


class Profile
{
public:

    Profile() = default;

    void startProfile( String name )
    {
		//判断模拟栈中是否为空，如果非空，保存此时栈的时间段
        if( !mProfileStack.isEmpty() )
        {
            auto& lastCallingElement = getLastCallingProfile();
            updateTime( lastCallingElement.name, Time::currentTimeMillis() - lastCallingElement.startTime );
        }
        pushNewProfile( name );
    }

    void endProfile()
    {
        auto& tmp = getLastCallingProfile();
        updateTime( tmp.name, Time::currentTimeMillis() - tmp.startTime );
        if( !mProfileStack.isEmpty() )
        {
            auto& lastCallingElement = getLastCallingProfile();
            lastCallingElement.startTime = Time::currentTimeMillis();
        }
    }

    
	void print()
	{
		;
	}

	/*void sortHash(String name) 
	{
		struct XX {
			String name;
			int64 data;
		};
		Array<XX> XXX;
		
		HashMap<String,int64>::Iterator i(mAllProfileRunTime);
		while (i.next())
		{ 
			XXX.add(i.getKey,i.getValue());
		}
		XXX.sort();
	}*/
   
	static Profile& getGlbProfile()
    {
        static Profile x;
        return x;
    }

private:

    struct Data
    {
        String name;		//函数名
        int64 startTime;	//开始时间
    };

    Array<Data>            mProfileStack;
    HashMap<String, int64> mAllProfileRunTime;

    void pushNewProfile( String name )
    {
        Data data{ name, Time::currentTimeMillis()};
        mProfileStack.add( data );	//尾插
    }

    Data& getLastCallingProfile()
    {
		//getReference:找到某个位置的元素，并且返回该位置的元素
        return mProfileStack.getReference( mProfileStack.size() - 1 );
    }

    Data popName()
    {
        return mProfileStack.removeAndReturn( mProfileStack.size() - 1 );
    }

    void updateTime( String x, int64 newTimeLen )
    {
        jassert( newTimeLen >= 0 );
		//判断x是否在hashmap中
        if( mAllProfileRunTime.contains( x ) )
        {
            mAllProfileRunTime.getReference( x ) += newTimeLen;
        }
        else
        {
            mAllProfileRunTime.set( x, newTimeLen );
        }
    }

};


ScopedProfiling::ScopedProfiling( String name )
{
    Profile::getGlbProfile().startProfile( name );
}

ScopedProfiling::~ScopedProfiling()
{
    Profile::getGlbProfile().endProfile();
}

String ScopedProfiling::printAllProfiles()
{
    return Profile::getGlbProfile().print();
}

} // end of namespace ktv