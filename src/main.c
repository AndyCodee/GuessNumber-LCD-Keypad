#include <stdio.h>
#include <string.h>
#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "MatrixKey.h"
#include <stdlib.h>


#define GUESS_LIMIT 7  // Maximum number of guesses allowed


// -------------------- Global Variables --------------------
unsigned char g_guessHistory[GUESS_LIMIT][10]; // Stores past guesses and results (e.g., "1234 2A1B")
unsigned char g_historyIndex = 0;              // Current index for history tracking

// -------------------- Function Prototypes --------------------
void GenerateRandomAnswer(unsigned char *ans);
void HandleInput(unsigned char num, unsigned char *input, unsigned char *count);
void Evaluate(unsigned char *input, unsigned char *ans, unsigned char *a, unsigned char *b);
void UpdateHistory(unsigned char *input, unsigned char a, unsigned char b, unsigned char *offset);
void HandleConfirm(unsigned char *ans, unsigned char *count, unsigned char *a, unsigned char *b, unsigned char *tryTimes);
void ShowPreviousHistory(unsigned char *offset);
void ShowNextHistory(unsigned char *offset);
void ResetState(unsigned char *tryTimes, unsigned char *count, unsigned char *a, unsigned char *b);


// -------------------- Function Implementations --------------------


/**
 * @brief Generates a random 4-digit answer string (e.g., "1234").
 * @param ans Pointer to a 5-byte array to store the answer (4 digits + null terminator).
 */
void GenerateRandomAnswer(unsigned char *ans)
{
    unsigned char r;
    unsigned char i;
    for (i = 0; i < 4; i++)
    {
        r = rand() % 10;  
        ans[i] = r + '0';
    }
    ans[4] = '\0';
}


/**
 * @brief Handles a single digit input and updates the current input string.
 * @param num The number input (1~10), where 10 is considered as '0'.
 * @param input The input string to be updated.
 * @param count Pointer to input length counter.
 */
void HandleInput(unsigned char num, unsigned char *input, unsigned char *count)
{
    LCD_ShowString(2,8,"         "); // Clear the second line display
    input[*count] = num % 10 + '0';  
    (*count)++;
    input[*count] = '\0';
    LCD_ShowString(2,1,input);       // Display current input
}


/**
 * @brief Compares the input with the answer and calculates A/B values.
 *        A: correct digit in correct position
 *        B: correct digit in wrong position
 * @param input User's input guess.
 * @param ans The correct answer.
 * @param a Pointer to count of 'A'.
 * @param b Pointer to count of 'B'.
 */
void Evaluate(unsigned char *input, unsigned char *ans, unsigned char *a, unsigned char *b)
{
    unsigned char checked_input[4] = {0};
    unsigned char checked_ans[4] = {0};
    unsigned char i, j;
		
		
    // First pass: check for correct digit and position (A)
    for (i = 0; i < 4; i++)
    {
        if (input[i] == ans[i]){
            (*a)++;
            checked_input[i] = 1;
            checked_ans[i] = 1;
        }
    }
		
    // Second pass: check for correct digit, wrong position (B)
    for (i = 0; i < 4; i++)
    {
        if (checked_input[i]){continue;}   // if checked, pass this index
        for (j = 0; j < 4; j++)
        {
            if (checked_ans[j]){continue;} // if checked, pass this index
            if (input[i] == ans[j])
            {
                (*b)++;
                checked_ans[j] = 1;        // update checked index
                break;
            }
        }
    }
}


/**
 * @brief Updates the guess history buffer with the latest result.
 * @param input User input (4-digit string).
 * @param a Count of 'A' (correct digits, correct positions).
 * @param b Count of 'B' (correct digits, wrong positions).
 * @param offset Pointer to display offset for history navigation.
 */
void UpdateHistory(unsigned char *input, unsigned char a, unsigned char b, unsigned char *offset)
{
    if (g_historyIndex >= GUESS_LIMIT) return;  

    // store 4 digit
    g_guessHistory[g_historyIndex][0] = input[0];
    g_guessHistory[g_historyIndex][1] = input[1];
    g_guessHistory[g_historyIndex][2] = input[2];
    g_guessHistory[g_historyIndex][3] = input[3];
		
    // space
    g_guessHistory[g_historyIndex][4] = ' ';

    // store a,b
    g_guessHistory[g_historyIndex][5] = a + '0';
    g_guessHistory[g_historyIndex][6] = 'A';
    g_guessHistory[g_historyIndex][7] = b + '0';
    g_guessHistory[g_historyIndex][8] = 'B';

    g_guessHistory[g_historyIndex][9] = '\0';

    g_historyIndex++;  
    *offset = 1;
}


/**
 * @brief Handles the result after pressing the confirm key.
 *        Displays result, updates counters, and handles correct/fail conditions.
 */
void HandleConfirm(unsigned char *ans, unsigned char *count, unsigned char *a, unsigned char *b, unsigned char *tryTimes)
{
    (*tryTimes)++; 
	
    // if all correct
    if (*a == 4)
    {
        LCD_ShowString(1,8,"         ");
        LCD_ShowString(1,1,"CORRECT"); 
    }
    else 
    {
        LCD_ShowString(1,8,g_guessHistory[g_historyIndex-1]);
			
        if(*tryTimes == GUESS_LIMIT)
        {
            LCD_ShowString(1,8,"         ");
            LCD_ShowString(1,1,"FAIL  "); 
            LCD_ShowString(2,1,ans); // Reveal the answer
            return;
        }
    }
		
    // reset variable
    *count = 0;
    *a = 0;
    *b = 0;
    LCD_ShowString(2,1,"    ");
}


/**
 * @brief Displays the previous guess history (scrolling upward).
 */
void ShowPreviousHistory(unsigned char *offset)
{
    // move backward by 'offset' indices
    if (*offset >= g_historyIndex){return;} 
    (*offset)++;
    LCD_ShowString(1,8,g_guessHistory[g_historyIndex - (*offset)]); 
    LCD_ShowString(2,8,g_guessHistory[g_historyIndex - (*offset) + 1]); 		
}


/**
 * @brief Displays the next guess history (scrolling downward).
 */
void ShowNextHistory(unsigned char *offset)
{
    // // move forward by 'offset' indices
    if (*offset <= 2){return;} 
    (*offset)--;
    LCD_ShowString(1,8,g_guessHistory[g_historyIndex - (*offset)]); 
    LCD_ShowString(2,8,g_guessHistory[g_historyIndex - (*offset) + 1]); 		
}


/**
 * @brief Resets game state variables and clears the LCD display.
 */
void ResetState(unsigned char *tryTimes, unsigned char *count, unsigned char *a, unsigned char *b)
{
    *tryTimes = 0;
    *count = 0;
    *a = 0;
    *b = 0;
	
    LCD_ShowString(1,1,"Input:          ");
    LCD_ShowString(2,1,"                ");
    memset(g_guessHistory, 0, sizeof(g_guessHistory)); 
    g_historyIndex = 0;
}


// -------------------- Main Program --------------------


void main()
{
    unsigned char KeyNum;
    unsigned char count = 0;
    unsigned char Input[5];
    unsigned char Answer[5];
    unsigned char a = 0, b = 0;
    unsigned char tryTimes = 0;
    unsigned char historyOffset = 1;
	
    LCD_Init();
    LCD_ShowString(1,1,"Input:");
    GenerateRandomAnswer(&Answer); // random 4 digit
	
    while(1)
    {
        KeyNum = MatrixKey();
        if (KeyNum)
        {		
            // 4 digit, 0-9
            if (count < 4 && KeyNum <= 10){HandleInput(KeyNum, Input, &count);}
					
            // confirm button
            if (KeyNum == 13 && count == 4) 
            {		
                Evaluate(Input, Answer, &a, &b); 
                UpdateHistory(Input, a, b, &historyOffset);
                HandleConfirm(Answer, &count, &a, &b, &tryTimes);
            }
					
            // upward history button
            if (KeyNum == 12){ShowPreviousHistory(&historyOffset);}
						
            // downward history button
            if (KeyNum == 16){ShowNextHistory(&historyOffset);}
					
            // reset button
            if (KeyNum == 14)
            {
                ResetState(&tryTimes, &count, &a, &b);
                GenerateRandomAnswer(&Answer);
            }
        }
    }
}