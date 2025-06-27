#include "print.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

struct Char
{
	uint8_t character;
	uint8_t color;
};

struct Char *buffer = (struct Char *)0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clear_row(size_t row)
{
    struct Char empty = {0, color};
    for (size_t col = 0; col < NUM_COLS; col++)
    {
        buffer[col + NUM_COLS * row] = empty;
    }
}

void print_clear()
{
	for (size_t i = 0; i < NUM_ROWS; i++)
	{
		clear_row(i);
	}
}


void print_newline()
{
    col = 0;

    if (row < NUM_ROWS - 1)
    {
        row++;
        return;
    }

    for (size_t r = 1; r < NUM_ROWS; r++)
    {
        for (size_t c = 0; c < NUM_COLS; c++)
        {
            struct Char character = buffer[c + NUM_COLS * r];
            buffer[c + NUM_COLS * (r - 1)] = character;
        }
    }

    clear_row(NUM_ROWS - 1); // Corrected to use NUM_ROWS
}

void print_char(char character)
{
	if (character == '\n')
	{
		print_newline();
		return;
	}

	if (col >= NUM_COLS)
	{
		print_newline();
	}

	buffer[col + NUM_COLS * row] = (struct Char){
		(uint8_t)character,
		color,
	};

	col++;
}

void print_str(char *str)
{
    if (str == NULL) return; // Check for NULL
    for (size_t i = 0; str[i] != '\0'; i++) {
        print_char(str[i]); // Use char directly
    }
}

void print_set_color(uint8_t foreground, uint8_t background)
{
    if (foreground > 15 || background > 15) return; // Check for valid color range
    color = foreground + (background << 4);
}
