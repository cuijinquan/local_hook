#ifndef LOCAL_HOOK_H_
#define LOCAL_HOOK_H_

#define MACRO_TOSTRING_FOR_HOOK(x) MACRO_TOSTRING_FOR_HOOK1(x)
#define MACRO_TOSTRING_FOR_HOOK1(x) #x
#define MACRO_JOIN_FOR_HOOK3(x,y,z) x##y##z
#define MACRO_JOIN_FOR_HOOK4(x,y,z,w) x##y##z##w

//MACRO API:
//LOCAL_HOOK(src_function_name, dest_function_name) : use it out of function
//LOCAL_HOOK_CPP(src_function_name, dest_function_name) : use it out of function
//LOCAL_HOOK_INIT(src_function_name) : use it out of function
//LOCAL_HOOK_START(src_function_name, dest_function_ptr) : use it in a function and the second argument is a function pointer!
//LOCAL_HOOK_FUNCTIONPOINTER_SYNC(src_function_name) : use it out of function

//local_hook is to help you hook a exported "C" function that was compiled into a dynamic library at x86 or x64 platform in elf format  "locally" !
//Your process space can be separated into following space models: an executable file, several dynamic libraries.
//Here "locally" means you can  hook  the exported function only in one unit.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ֲ����ҹ����������㡰�ֲ����ض�һ��elf��̬�⵼����C�������С��ҹ�����
//���ǿ��԰ѽ��̵�ַ�ռ仮��Ϊ���µ�ַģ�飺һ����ִ���ļ��������̬�⡣
//����������˵�ġ��ֲ��������á��ҹ���Ϊ������һ����ַģ������Ч��

//#include <stdio.h>
//LOCAL_HOOK(puts, myputs) 

//That's all��
//������ˣ�


//You have to use LOCAL_HOOK out of function filed. The hook will happen before the main function, or after dlopen finish if the codes is compiled into a dynamic library.
//If your version of GCC does not support __attribute__ ((constructor)), you can use LOCAL_HOOK_CPP in a c++ source file instead.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����ں�������ʹ��LOCAL_HOOK���������ҹ�����Ϊ����main����ִ��֮ǰ��Ч����dlopen���֮���������������Ǳ�������˶�̬�⣩��Ч��
//������GCC�汾��֧��__attribute__ ((constructor)), ��ô�������һ��C++�����ļ���ʹ��LOCAL_HOOK_CPP������LOCAL_HOOK��


//But this is not perfect. If other models use function pointer of puts, our hook may affects them.
//The reason is due to the dynamic library linker wants the function pointer of the one function in every models are same, implemented by GOT copy. 
//But seldom did programmers use function pointer to call a function in GLIBC? Most of our hook goals are there.
//But we provide LOCAL_HOOK_FUNCTIONPOINTER_SYNC(puts) to help you remove the side affect if you face it(Attention the function pointers now are different!) 
//and you need to write it in one of the source file of that model (You can use a new file!), then compile and link it.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����Ⲣ���������������ģ��ʹ��puts�ĺ���ָ��,���ǵġ��ҹ�����Ϊ�п��ܻ�Ӱ�쵽���ǵ�ȡֵ,
//ԭ���Ƕ�̬������ϣ�������ڲ�ͬģ�����ͬһ�����������ͬ�ĺ���ָ��ֵ, ����ص�ģ�����Ǵ��ȼ��ص�ģ���п�������GOT��
//������˵��������������ͨ������ָ��ȥ����GLIBC��ĺ����ɣ����Ǿ�������ҹ���Ϊ��Ŀ�궼����GLIBC���
//���������㹻�������������Ķ�̬�⣬��ô�����ṩLOCAL_HOOK_FUNCTIONPOINTER_SYNC(puts) ȥ�Ƴ������ģ��ĸ����ã���ʱ���ڲ�ͬģ������������puts��õĺ���ָ��ֵ����Ͳ�ͬ�ˣ�
//�������������̫�����г���ʹ��������԰ɣ���������Ҫ�������仰д���Ǹ�ģ���ĳ��Դ���ļ��������¼�һ��Դ���ļ���������仰���в����±�����������ģ�顣
//����������������������Դ��, �Ǿ�û�취�ˡ�


//For more flexible, you can use LOCAL_HOOK_INIT out of function and LOCAL_HOOK_START in any proper function. Then the hook will happen after LOCAL_HOOK_START executed.
//Either the first hook happened in LOCAL_HOOK or LOCAL_HOOK_CPP or LOCAL_HOOK_START, you can call LOCAL_HOOK_START anywhere to hook second times or more, with any other function.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����Ҫ������ʹ�ã�������ں�������ʹ��LOCAL_HOOK_INIT����ĳ�����ʵĺ�����ʹ��LOCAL_HOOK_START�����ҹ�����Ϊ����LOCAL_HOOK_STARTִ�к���Ч��
//���ܵ�һ�εġ��ҹ�����Ϊ������LOCAL_HOOK �� LOCAL_HOOK_CPP ���� LOCAL_HOOK_START֮�У���������κκ����еڶ��ε��û��߸���εĵ���LOCAL_HOOK_START�����¡��ҹ�����


//Attention:
//You can use it in C/C++ language.
//Local hook can be only used to hook a function that was compiled into a dynamic library at x86 or x64 platform in elf format.
//Large code models(SYSTEM V AMD64 ABI) is not supported. (Your GOT is greater than 2GB? The total linked ".so" files should be greater than 50GB in size!)
//How to remove the hook? You need to use dlsym to get the original function address first(after hook you do not have other method to get the original function address),
//and use LOCAL_HOOK_START with the address as 2rd argument.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ע�⣺
//�������C/C++��ʹ�á�
//�ֲ��ҹ�ֻ�ܶ�һ����x86����x64��ƽ̨�±����elf��ʽ�Ķ�̬�Ⲣ�����ĺ������йҹ���
//�����ʹ���ģʽ��(�� SYSTEM V AMD64 ABI) �ǲ�֧�ֵġ�����˵�ɣ��ѵ������ӵĶ�̬�ⳬ��50G�ˣ���
//��Ҫ�Ƴ����ҹ���������Ҫ��ͨ��dlsym��ȡ������ԭʼ��ַ("�ҹ�"֮����û�б�ķ�����ȡ����ԭʼ��ַ��)���ٰ�����Ϊ�ڶ�����������LOCAL_HOOK_START��


#define LOCAL_HOOK(src_function_name, dest_function_name) \
LOCAL_HOOK_INIT(src_function_name) \
__attribute__ ((constructor)) static void MACRO_JOIN_FOR_HOOK4(hook_, src_function_name, dest_function_name, _init)(){\
   LOCAL_HOOK_START(src_function_name, &(dest_function_name)); \
}

#ifdef __cplusplus
#define LOCAL_HOOK_CPP(src_function_name, dest_function_name) \
LOCAL_HOOK_INIT(src_function_name) \
struct MACRO_JOIN_FOR_HOOK4(hook_, src_function_name, dest_function_name, _struct){ \
   MACRO_JOIN_FOR_HOOK4(hook_, src_function_name, dest_function_name, _struct)(){ \
        LOCAL_HOOK_START(src_function_name, &(dest_function_name)); \
   } \
}; \
static MACRO_JOIN_FOR_HOOK4(hook_, src_function_name, dest_function_name, _struct) MACRO_JOIN_FOR_HOOK4(hook_, src_function_name, dest_function_name, _struct_singleton);
#endif

#define LOCAL_HOOK_GOT_FUNCTION_NAME(src_function_name) MACRO_JOIN_FOR_HOOK3(get_, src_function_name, _got_address)
#define LOCAL_HOOK_PLT_FUNCTION_NAME(src_function_name) MACRO_JOIN_FOR_HOOK3(get_, src_function_name, _plt_address)


//.got.plt section can not be deduced by @GOTPLT relocation so we choose to disassemble the plt instruction.
#ifdef __x86_64__

//Attention we do not support large code models! (GOT is greater than 2GB? The "so" may be greater than 50GB!)
#define GET_GOT_PLT_ADDRESS(plt_address, result) \
    do{ \
        unsigned char* c_plt_address = (unsigned char*)(plt_address); \
        union{ \
            int int_content; \
            char content[4]; \
        }a; \
        result = 0; \
        if((c_plt_address[0] & 0xf8) == 0x48){ \
            ++c_plt_address; \
        } \
        if(c_plt_address[0] == 0xff){ \
           if(c_plt_address[1] == 0x25){ \
                const int opsize = 2; \
                const int instruction_size = opsize + 4; \
                a.content[0] = c_plt_address[opsize + 0]; \
                a.content[1] = c_plt_address[opsize + 1]; \
                a.content[2] = c_plt_address[opsize + 2]; \
                a.content[3] = c_plt_address[opsize + 3]; \
                result = (typeof(result))(c_plt_address + instruction_size + a.int_content); \
            } \
            else if(c_plt_address[1] == 0x24 && c_plt_address[2] == 0x25){ \
                const int opsize = 3; \
                a.content[0] = c_plt_address[opsize + 0]; \
                a.content[1] = c_plt_address[opsize + 1]; \
                a.content[2] = c_plt_address[opsize + 2]; \
                a.content[3] = c_plt_address[opsize + 3]; \
                result = (typeof(result))((long long)a.int_content); \
            } \
        } \
    }while(0)

#endif // __x86_64__

#ifdef __i386__

#define GET_GOT_PLT_ADDRESS(plt_address, result) \
    do{ \
        unsigned char* c_plt_address = (unsigned char*)(plt_address); \
        union{ \
            int int_content; \
            char content[4]; \
        }a; \
        result = 0; \
        if(c_plt_address[0] == 0xff && c_plt_address[1] == 0x25){ \
            const int opsize = 2; \
            a.content[0] = c_plt_address[opsize + 0]; \
            a.content[1] = c_plt_address[opsize + 1]; \
            a.content[2] = c_plt_address[opsize + 2]; \
            a.content[3] = c_plt_address[opsize + 3]; \
            result = (typeof(result))(a.int_content); \
        } \
    }while(0)

#endif // __i386__

#if defined(__pic__ ) || defined(__PIC__) || defined(__pie__) || defined(__PIE__)

#ifdef __x86_64__

#define LOCAL_HOOK_GOT(src_function_name) \
__attribute__ ((noinline, optimize("O2"))) static void** LOCAL_HOOK_GOT_FUNCTION_NAME(src_function_name)(){ \
    register void **result asm ("rax"); \
    __asm__  (      "leaq      "MACRO_TOSTRING_FOR_HOOK(src_function_name)"@GOTPCREL(%rip), %rax;"); \
    return result; \
}

#define LOCAL_HOOK_PLT(src_function_name) \
__attribute__ ((noinline, optimize("O2"))) static void*  LOCAL_HOOK_PLT_FUNCTION_NAME(src_function_name)(){ \
    register void *result asm ("rax"); \
    __asm__  (      "leaq       "MACRO_TOSTRING_FOR_HOOK(src_function_name)"@PLT(%rip), %rax;"); \
    return result; \
}

#endif // __x86_64__


#ifdef __i386__

//Intel c++ compiler result says call-pop that leads invalidation of stack buffer will be better than call-lea that leads real jump.
#define LOCAL_HOOK_GOT(src_function_name) \
__attribute__ ((noinline, optimize("O2"))) static void** LOCAL_HOOK_GOT_FUNCTION_NAME(src_function_name)(){ \
    register void **result asm ("eax"); \
    __asm__  (      "call       1f;" \
                    "1:" \
                    "popl       %eax;" \
                    "2:" \
                    "leal       _GLOBAL_OFFSET_TABLE_+[2b-1b](%eax), %eax;" \
                    "leal       "MACRO_TOSTRING_FOR_HOOK(src_function_name)"@GOT(%eax), %eax;" \
            ); \
    return result; \
}

#define LOCAL_HOOK_PLT(src_function_name) \
__attribute__ ((noinline, optimize("O2"))) static void*  LOCAL_HOOK_PLT_FUNCTION_NAME(src_function_name)(){ \
    register void *result asm ("eax"); \
    __asm__  (      "call       1f;" \
                    "1:" \
                    "popl       %eax;" \
                    "leal       "MACRO_TOSTRING_FOR_HOOK(src_function_name)"@PLT(%eax), %eax;" \
                    "2:" \
                    "leal       2b-1b(%eax), %eax;" \
            ); \
    return result; \
}

#endif // __i386__

#define LOCAL_HOOK_INIT(src_function_name)\
    LOCAL_HOOK_GOT(src_function_name) \
    LOCAL_HOOK_PLT(src_function_name)

#define LOCAL_HOOK_START(src_function_name, dest_function_ptr) \
    do{ \
        void** got_plt_addr; \
        *LOCAL_HOOK_GOT_FUNCTION_NAME(src_function_name)() = (void*)dest_function_ptr; \
        GET_GOT_PLT_ADDRESS(LOCAL_HOOK_PLT_FUNCTION_NAME(src_function_name)(), got_plt_addr); \
        *got_plt_addr = (void*)dest_function_ptr; \
    }while(0)


#define LOCAL_HOOK_FUNCTIONPOINTER_SYNC(src_function_name) \
	LOCAL_HOOK_INIT(src_function_name) \
	__attribute__ ((constructor)) static void MACRO_JOIN_FOR_HOOK3(hook_, src_function_name , _sync)(){\
		 *LOCAL_HOOK_GOT_FUNCTION_NAME(src_function_name)() = LOCAL_HOOK_PLT_FUNCTION_NAME(src_function_name)();\
	}


#else // defined(__pic__ ) || defined(__PIC__) || defined(__pie__) || defined(__PIE__)

//now so easy

#define LOCAL_HOOK_INIT(src_function_name)

#define LOCAL_HOOK_START(src_function_name, dest_function_ptr) \
    do{ \
        void** got_plt_addr; \
        GET_GOT_PLT_ADDRESS(&src_function_name, got_plt_addr); \
        *got_plt_addr = (void*)dest_function_ptr; \
    }while(0)

#endif // defined(__pic__ ) || defined(__PIC__) || defined(__pie__) || defined(__PIE__)

#endif






