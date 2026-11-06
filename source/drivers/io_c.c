/**
 * @file io.c
 * @author Ben Marples
 * @brief
 */

//------------------------------------------------------------------------------
// Includes
#include "err.h"
#include "kbool.h"
#include "type.h"
#include "io.h"
#include "ascii.h"
#include "k_args.h"

//------------------------------------------------------------------------------
// Function Declarations
int sn_printf(int len, char *buff, const char *format, ...);

//------------------------------------------------------------------------------
// Function Implementions

/**
 * @brief Walk the format string until the escape sequence appears until the escape sequence is found.
 * until max length is hit or all text is format the next char is placed into the output buffer
 */
int sn_printf(int len, char *buff, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    char last_char = NULL;
    int count = 0;

    while (count < len && *format != '\0')
    {
        if (last_char == '%')
        {
            switch (*format)
            {
            case 'd':
                char nums[16];
                int_to_char(va_arg(args,int), nums);
                
                char *p = nums;
                buff--;
                while (*p && count < len - 1)
                {
                    *buff++ = *p++;
                    count++;
                }
                format++;
                continue;
                

            default:
                break;
            }
        }
        last_char = *format;
        *buff = *format;
        format++;
        buff++;
    }

    va_end(args);
    return 0;
}
