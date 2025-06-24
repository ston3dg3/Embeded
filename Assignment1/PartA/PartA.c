#include <xmc_gpio.h>

#define TICKS_PER_SECOND 1000

static volatile uint32_t ticks = 0;
const int dot_len = 100;              // 1
const int space_symbol = dot_len;     // 0
const int space_letter = 3 * dot_len; // 000
const int space_word = 7 * dot_len;   // 0000000
const int space_retransmit = 5000;    // 50 0's
const char *morsecode[36] = {
    // Binary codewords for Morse code letters and digits - 0 represents LED off for 100ms, 1 represents LED on for 100ms
    "10111",               // 65 - A (index 0)
    "111010101",           // 66 - B
    "11101011101",         // 67 - C
    "1110101",             // 68 - D
    "1",                   // 69 - E
    "101011101",           // 70 - F
    "111011101",           // 71 - G
    "1010101",             // 72 - H
    "101",                 // 73 - I
    "1011101110111",       // 74 - J
    "111010111",           // 75 - K
    "101110101",           // 76 - L
    "1110111",             // 77 - M
    "11101",               // 78 - N
    "11101110111",         // 79 - O
    "10111011101",         // 80 - P
    "1110111010111",       // 81 - Q
    "1011101",             // 82 - R
    "10101",               // 83 - S
    "111",                 // 84 - T
    "1010111",             // 85 - U
    "101010111",           // 86 - V
    "101110111",           // 87 - W
    "11101010111",         // 88 - X
    "1110101110111",       // 89 - Y
    "11101110101",         // 90 - Z (index 25)
    "1110111011101110111", // 48 - 0 (index 26)
    "10111011101110111",   // 49 - 1
    "101011101110111",     // 50 - 2
    "1010101110111",       // 51 - 3
    "10101010111",         // 52 - 4
    "101010101",           // 53 - 5
    "11101010101",         // 54 - 6
    "1110111010101",       // 55 - 7
    "111011101110101",     // 56 - 8
    "11101110111011101"    // 57 - 9 (index 35)
};

void delay(uint32_t ms)
{
    uint32_t start = ticks;
    while ((ticks - start) < ms)
    {
    }
}

void SysTick_Handler(void)
{
    ticks++;
}

int process_letter(char letter)
{
    if (letter >= 'a' && letter <= 'z')
        letter -= 32; // Convert lowercase to uppercase
    if (letter >= 'A' && letter <= 'Z')
        return letter - 65; // 'A' or 'a' => 0, ..., 'Z' or 'z' => 25
    if (letter >= '0' && letter <= '9')
        return (letter - 48) + 26; // '0' => 26, ..., '9' => 35
    return -1;                     // Invalid input
}

void transmit_morse(char *icanmorse)
{ // start trasmitting icanmorse array
    for (int i = 0; icanmorse[i] != '\0'; i++)
    {
        if (i == 0)
            delay(100); // Initial delay before starting transmission
        if (icanmorse[i] == ' ')
        {
            delay(space_word);
            continue; // Skip spaces in the input string
        }
        int index = process_letter(icanmorse[i]);
        if (index >= 0 && index < 36)
        {
            char *morse_char = morsecode[index];
            // start tramsmitting one character from icanmorse array
            for (int j = 0; morse_char[j] != '\0'; j++)
            {
                if (morse_char[j] == '0')
                {
                    XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);
                }
                else if (morse_char[j] == '1')
                {
                    XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);
                }
                delay(space_symbol);
            }

            XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);
            if (icanmorse[i + 1] != '\0' && icanmorse[i + 1] != ' ')
                delay(space_letter);
        }
    }
}

int main(void)
{
    const XMC_GPIO_CONFIG_t LED_config = {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};

    XMC_GPIO_Init(XMC_GPIO_PORT1, 1, &LED_config);
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / TICKS_PER_SECOND); // 1 ms tick

    while (1)
    {
        transmit_morse("I Can Morse");
        delay(space_retransmit); // Wait before transmitting again
    }
    return 0;
}
