#ifndef __ERRLOG_H__
    #define __ERRLOG_H__

    #include <errno.h>
    #include <stdio.h>
    #include <stdarg.h>
    #include <stdlib.h>
    #include <string.h>

    static inline void vError(
        int level,
        bool system,
        const char *format,
        va_list vaList
    ) {
        //int fflush(FILE *stream) Если stream связан с файлом, открытым для записи, то вызов fflush() приводит к физической записи содержимого буфера в файл. Если же stream указывает на вводимый файл, то очищается входной буфер. В обоих случаях файл остается открытым.
        //Возврат 0 означает успех, а возврат ненулевой величины указывает на наличие ошибки по за­писи. Очистка всех буферов производится автоматически при нормальном завершении програм­мы или при заполнении буферов. Закрытие файла также приводит к очистке буферов.
        
        fflush(stdout);
        fflush(stderr);

        bool info = (level==3);
        bool fatal = (level==0);
        bool warning = (level==2);
        const char *msgType =
            info    ? "info"    :
            fatal   ? "fatal"   :
            warning ? "warning" :
                      "error"
        ;

        //stderr -дескриптор файла C,  он представляет собой стандартный вывод ошибок.
        //Функция strerror() возвращает указатель на сообщение об ошибке, связанное с номером ошибки.
        //printf (strerror(errno)) Следующий фрагмент программы в случае ошибки выведет сообщение об ошибке, связанное с глобальной переменной errno:
        //errno – переменная, хранящая целочисленный код последней ошибки. В каждом потоке существует своя локальная версия errno, чем и обусловливается её безопасность в многопоточной среде. Обычно errno реализуется в виде макроса, разворачивающегося в вызов функции, возвращающей указатель на целочисленный буфер. При запуске программы значение errno равно нулю.
        //errno – (Обработка ошибок в C) https://habr.com/ru/post/324642/

        fprintf(
            stderr,
            "%s: %s%s",
            msgType,
            system ? strerror(errno) : "",
            system ? ": " : ""
        );

        //Функции vprintf(), vfprintf() и vsprintf() функционально эквивалентны функциям printf(), fprintf() и sprintf() соответственно. Различие состоит лишь в том, что список аргументов заменен на указа­тель на список аргументов. Этот указатель должен иметь тип va_list, определенный в stdarg.h.

        vfprintf(
            stderr,
            format,
            vaList
        );

        //Функция fputc() записывает символ ch в указанный поток в позицию, соответствующую текуще­му значению указателя положения в файле
        //Функция fputc() возвращает значение записанного символа. В случае ошибки она возвращается EOF http://www.c-cpp.ru/content/fputc

        fputc('\n', stderr);
        fflush(stdout);
        fflush(stderr);

        if(fatal) 
        {
            //Функция abort() вызывает немедленное прекращение программы. Очистка буферов файлов не про­изводится. Функция возвращает вызывающему процессу значение 3 (обычно операционной системе).
            //Основное назначение функции abort() — это предотвращение закрытия файлов некорректно функционирующей программой
            abort();
        }
    }

    static inline void sysErr(
        const char *format,
        ...
    ) {
        va_list vaList;
        va_start(vaList, format);
            vError(1, true, format, vaList);
        va_end(vaList);
    }

    static inline void errFatal(
        const char *format,
        ...
    ) {
        va_list vaList;
        va_start(vaList, format);
            vError(0, false, format, vaList);
        va_end(vaList);
    }

    static inline void sysErrFatal(
        const char *format,
        ...
    ) {
        va_list vaList;
        va_start(vaList, format);
            vError(0, true, format, vaList);
        va_end(vaList);
    }

    static inline void warning(
        const char *format,
        ...
    ) {
        va_list vaList;
        va_start(vaList, format);
            vError(2, false, format, vaList);
        va_end(vaList);
    }

    static inline void info(
        const char *format,
        ...
    ) {
        va_list vaList;
        va_start(vaList, format);
            vError(3, false, format, vaList);
        va_end(vaList);
    }

#endif // __ERRLOG_H__

