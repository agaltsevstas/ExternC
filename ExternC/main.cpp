#include <iostream>

/*
 В C НЕТ перегрузки функций, т.к. компилятор формирует промежуточные объектные файлы (.o)/(.obj) (в зависимости от комплиятора), в которых указаны ТОЛЬКО имена функций без ее типов аргументов, поэтому компоновщик видит ТОЛЬКО имя функции.
 extern "C" - директива, которая помогает загрузить функцию из языка Си в C++. Эта директика не существует для языка Си, но существует для языка C++, поэтому чтобы функция компилировалась сразу на 2 языках (например, общая библиотека): С и C++ нужно обернуть в директиву условной компиляции:
 #ifdef __cplusplus
 extern "C"
 #endif
 */
namespace c
{
    extern "C"
    int Module_int(int number); // _Z10Module_int (_Z10 - метка функции + кол-во символов, Module_int - имя функции)

    extern "C" double Module_double(double number) // _Z13Module_double (_Z13 - метка функции + кол-во символов, Module_double - имя функции)
    {
        return number > 0 ? number : -number;
    }

    #ifdef __cplusplus
    extern "C" // можно без скобок
    {
        float Module_float(float number) // _Z12Module_float (_Z12 - метка функции + кол-во символов, Module_float - имя функции)
        {
            return number > 0 ? number : -number;
        }

        /*
         Ошибка: В С нет перегрузки функции
         int Module(int number) // _Z6Module (_Z6 - метка функции + кол-во символов, Module - имя функции)
         {
             return number > 0 ? number : -number;
         }
         */
    }
    #endif

    int Module_int(int number)
    {
        return number > 0 ? number : -number;
    }

    extern "C" int* Get_Pointer()
    {
        // В C нет new/delete, поэтому лучше использовать malloc/free
        return (int*) malloc(10);
    }
}

/*
 В C++ ЕСТЬ перегрузка функций, поэтому компоновщик видит имя функции + тип переменной. Благодаря искажении имен (name mangling) происходит перегрузка функций - это одно название для нескольких функций с разными типами аргументов. Компилятор формирует промежуточные объектные файлы (.o)/(.obj) (в зависимости от комплиятора), в которых указаны имена функций + типы аргументов, поэтому компоновщик видит аргумент функции. Нельзя начинать название функции с подчеркивания.
 */
namespace cplusplus
{
    auto Module(double number) // _Z6Moduled (_Z6 - метка функции + кол-во символов, Module - имя функции, d - тип аргумента)
    {
        return number > 0 ? number : -number;
    }

    auto Module(int number) // _Z6Modulei (_Z6 - метка функции + кол-во символов, Module - имя функции, i - сокращенный тип аргумента (int))
    {
        return number > 0 ? number : -number;
    }

    auto Module(float number) // _Z6Modulef (_Z6 - метка функции + кол-во символов, Module - имя функции, f - сокращенный тип аргумента (float))
    {
        return number > 0 ? number : -number;
    }
}

int main(int argc, const char * argv[])
{
    // C
    {
        using namespace c;
        
        [[maybe_unused]] auto module1 = Module_int(1);
        [[maybe_unused]] auto module2 = Module_int(-1);
        [[maybe_unused]] auto module3 = Module_double(1.0);
        [[maybe_unused]] auto module4 = Module_double(-1.0);
        [[maybe_unused]] auto module5 = Module_float(1.0f);
        [[maybe_unused]] auto module6 = Module_float(-1.0f);
        
        [[maybe_unused]] auto pointer = Get_Pointer();
        free(pointer);
    }
    // C++
    {
        using namespace cplusplus;
        
        [[maybe_unused]] auto module1 = Module(1);
        [[maybe_unused]] auto module2 = Module(-1);
        [[maybe_unused]] auto module3 = Module(1.0);
        [[maybe_unused]] auto module4 = Module(-1.0);
        [[maybe_unused]] auto module5 = Module(1.0f);
        [[maybe_unused]] auto module6 = Module(-1.0f);
    }

    return 0;
}
