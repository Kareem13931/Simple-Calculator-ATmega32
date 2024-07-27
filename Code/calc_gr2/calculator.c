/*****************************************************************************************/
/***************************** Name    : Kareem Adel Othman ******************************/
/***************************** Date    : 7/24/2024          ******************************/
/***************************** Version : 1.0                ******************************/
/*****************************************************************************************/
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "keypad.h"
#include "std_macros.h"
#include <util/delay.h>

double num1 = 0, num2 = 0, result;
char op, floated;
signed char length = 0;
char res_length = 0, notloop = 1;
char no_interrupt = 1;

void in_num1()
{
    char x = 0;
    length = 0;
    floated = 0;
    double sign = 1;
    char s = 0;

    if (notloop == 1)
    {
        while (1)
        {
            x = keypad_u8check_press();
            if (x != 0xff) {
                if (x >= '0' && x <= '9' && length < 5)
                {
                    if (!(length == 0 && x == '0'))
                    {
                        LCD_vSend_char(x);
                        length++;
                        if (floated > 0)
                        {
                            double a = (x - '0');
                            for (char i = 0; i < floated; i++)
                                a /= 10;

                            num1 += a;
                            floated++;
                        }
                        else
                        {
                            num1 *= 10;
                            num1 += (x - '0');
                        }
                    }
                }
                else if (x == '-' && length == 0)
                {
                    LCD_vSend_char(x);
                    sign = -1;
                    s = 1;
                }
                else if (length != 0 && (x == '+' || x == '-' || x == '*' || x == '/'))
                {
                    op = x;
                    LCD_vSend_char(x);
                    _delay_ms(500);
                    break;
                }
                else if (floated == 0 && x == '.')
                {
                    LCD_vSend_char(x);
                    floated = 1;
                    length -= 2;
                }
                _delay_ms(500);
            }
        }
        num1 *= sign;
    }
    else
    {
        while (1)
        {
            x = keypad_u8check_press();
            if (x == '+' || x == '-' || x == '*' || x == '/')
            {
                op = x;
                LCD_vSend_char(x);
                _delay_ms(300);
                break;
            }
        }
    }
}

void in_num2()
{
    char x = 0;
    length = 0;
    num2 = 0;
    floated = 0;
    double sign = 1;
    char s = 0;

    while (1)
    {
        x = keypad_u8check_press();
        if (x != 0xff)
        {
            if (x >= '0' && x <= '9' && length < 5)
            {
                if (!(length == 0 && x == '0'))
                {
                    LCD_vSend_char(x);
                    length++;
                    if (floated > 0)
                    {
                        double a = (x - '0');
                        for (char i = 0; i < floated; i++)
                            a /= 10;

                        num2 += a;
                        floated++;
                    }
                    else
                    {
                        num2 *= 10;
                        num2 += (x - '0');
                    }
                }
            }
            else if (x == '-' && length == 0)
            {
                LCD_vSend_char(x);
                sign = -1;
                s = 1;
            }
            else if (floated == 0 && x == '.')
            {
                LCD_vSend_char(x);
                floated = 1;
                length -= 2;
            }
            else if (x == '=')
            {
                break;
            }
            _delay_ms(400);
        }
    }
    num2 *= sign;
}

void do_op()
{
    switch (op)
    {
        case '-':
            result = num1 - num2;
            break;
        case '+':
            result = num1 + num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 == 0)
            {
            	 LCD_movecursor(2, 1);
                LCD_vSend_string("Error: Div by 0");
                result = 0;
            }
            else
            {
                result = num1 / num2;
            }
            break;
        default:
            break;
    }
}

void display_num(char ro, char c)
{
    char n = 0;
    double result1 = result;
    if (result1 < 0)
    {
        n = 1;
        result1 *= -1;
        res_length++;
    }
    res_length = 0;
    char cc = c;
    long long res = (long long)result1;
    long long rem = (result1 - res) * 100.0;
    char point = 0;

    if (res == 0 && rem == 0)
    {
        LCD_movecursor(ro, c--);
        LCD_vSend_char('0');
        res_length++;
    }
    else
    {
        if (rem > 0)
        {
            char y = rem % 10;
            while (rem > 0)
            {
                point++;
                res_length++;
                LCD_movecursor(ro, c--);
                LCD_vSend_char('0' + y);
                rem /= 10;
                y = rem % 10;
            }
            LCD_movecursor(ro, c--);
            LCD_vSend_char('.');
            res_length++;
        }

        char y = res % 10;
        if (res == 0)
        {
            LCD_movecursor(ro, c--);
            LCD_vSend_char('0');
            res_length++;
        }
        else
        {
            while (res > 0)
            {
                res_length++;
                LCD_movecursor(ro, c--);
                LCD_vSend_char('0' + y);
                res /= 10;
                y = res % 10;
            }
        }
    }

    if (n == 1)
    {
        LCD_movecursor(ro, c--);
        LCD_vSend_char('-');
    }
    LCD_movecursor(ro, cc + 1);
}

void buzzer_1(void)
{
    PORTC = 0b00000110;
    _delay_ms(500);
    PORTC = 0b00000000;
    _delay_ms(500);
}

int main(void)
{
    keypad_vInit();
    LCD_vInit();
    LCD_movecursor(1, 1);
    LCD_vSend_string("ENG_Kareem Adel");
    _delay_ms(2000);
    LCD_clearscreen();
    LCD_vSend_string("Calc is ready");
    _delay_ms(1000);
    LCD_clearscreen();

    while (1)
    {
        in_num1();
        in_num2();
        do_op();
        display_num(2, 16);
        buzzer_1();

        LCD_vSend_cmd(CURSOR_OF_DISPLAY_ON);
        _delay_ms(500);
        while (keypad_u8check_press() != '=');
        LCD_vSend_cmd(CLR_SCREEN);
        LCD_vSend_cmd(CURSOR_BLINK_DISPLAY_ON);
        display_num(1, res_length + 1);
        notloop = 0;
        num1 = result;
    }
}
