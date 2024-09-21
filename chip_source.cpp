#include <iostream>
using namespace std;
#include "chip.h"
#include <cstdio>
#include <cstdlib>
#include <time.h>

chip8::chip8() {
    // Constructor can be used for memory allocation or other initial settings
    // Do not perform complete initialization here
}

chip8::~chip8()
{
	// empty
}

void chip8::initialize() {
    // Initialize program counter
    pc = 0x200;  // Programs start at memory location 0x200

    // Initialize opcode and index register
    opcode = 0;  // No opcode fetched yet
    I = 0;       // Reset index register

    // Clear memory
    for (int i = 0; i < 4096; ++i) {
        memory[i] = 0;
    }

    // Load the font set into memory
    loadFontSet();

    // Clear registers V0 to VF
    for (int i = 0; i < 16; ++i) {
        V[i] = 0;
    }

    // Clear stack and reset stack pointer
    sp = 0;      // Reset stack pointer to 0
    for (int i = 0; i < 16; ++i) {
        stack[i] = 0;
    }

    // Clear the gfx (64x32 pixels)
    for (int i = 0; i < 64 * 32; ++i) {
        gfx[i] = 0;
    }

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;

    // Reset key states
    for (int i = 0; i < 16; ++i) {
        key[i] = 0;
    }
    // Clear screen once
	drawFlag = true;

	srand (time(NULL));

}


// Define the CHIP-8 font set
const unsigned char fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0x90, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0x80, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8::loadFontSet() {
    for (int i = 0; i < 80; ++i) {
        memory[0x050 + i] = fontset[i];
    }
}

bool chip8::loadGame(const char * filename) // initialization made in that function therefore only loadGame is used in main.cpp from chip_source.cpp.
{
	initialize();
	printf("Loading: %s\n", filename);

	// Open file
	FILE * pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		fputs ("File error", stderr);
		return false;
	}

	// Check file size
	fseek(pFile , 0 , SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);
	printf("Filesize: %d\n", (int)lSize);

	// Allocate memory to contain the whole file
	char * buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL)
	{
		fputs ("Memory error", stderr);
		return false;
	}

	// Copy the file into the buffer
	size_t result = fread (buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error",stderr);
		return false;
	}

	// Copy buffer to Chip8 memory
	if((4096-512) > lSize)
	{
		for(int i = 0; i < lSize; ++i)
			memory[i + 512] = buffer[i];
	}
	else
		printf("Error: ROM too big for memory");

	// Close file, free buffer
	fclose(pFile);
	free(buffer);

	return true;
}
















/* void chip8::loadGame(const char* fileName) {

    initialize();

    // Open the file in binary mode
    FILE* gameFile = fopen(fileName, "rb");
    if (gameFile == nullptr) {
        printf("Failed to open file: %s\n", fileName);
        return;
    }
    else
        {
        printf("Succesfully created");
        }


    // Find the file size
    fseek(gameFile, 0, SEEK_END);   // Move to the end of the file
    long fileSize = ftell(gameFile); // Get the size of the file
    rewind(gameFile);                // Move back to the start of the file

    // Allocate buffer to hold the game data
    char* buffer = new char[fileSize];

    // Read the file into the buffer
    size_t bytesRead = fread(buffer, 1, fileSize, gameFile);
    if (bytesRead != fileSize) {
        printf("Error reading file: %s\n", fileName);
        delete[] buffer;
        fclose(gameFile);
        return;
    }

    // Load the buffer into CHIP-8 memory starting at address 0x200 (512)
    for (int i = 0; i < fileSize; ++i) {
        memory[i + 0x200] = buffer[i];
    }

    // Clean up
    delete[] buffer; // Free the buffer
    fclose(gameFile); // Close the file
}

*/

void chip8::emulateCycle()
{
    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    // Decode opcode
    switch(opcode & 0xF000)
    {
        case 0x0000:
            switch(opcode & 0x000F)
            {
                case 0x0000: // 0x00E0: Clears the screen
                    std::fill_n(gfx, sizeof(gfx), 0); // Clear the display
                    pc += 2;
                    break;

                case 0x000E: // 0x00EE: Returns from subroutine
                    --sp; // Decrement stack pointer
                    pc = stack[sp]; // Set PC to the address at the top of the stack
                    pc += 2;
                    break;

                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
                    break;
            }
            break;

        case 0x1000: // 0x1NNN: Jump to address NNN
            pc = opcode & 0x0FFF;
            break;

        case 0x2000: // 0x2NNN: Call subroutine at NNN
            stack[sp] = pc; // Push current PC onto stack
            ++sp; // Increment stack pointer
            pc = opcode & 0x0FFF; // Set PC to address NNN
            break;

        case 0x3000: // 0x3XNN: Skip the next instruction if VX equals NN
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                pc += 4; // Skip next instruction
            else
                pc += 2; // Move to the next instruction
            break;

        case 0x4000: // 0x4XNN: Skip the next instruction if VX does not equal NN
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                pc += 4; // Skip next instruction
            else
                pc += 2; // Move to the next instruction
            break;

        case 0x5000: // 0x5XY0: Skip the next instruction if VX equals VY
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
                pc += 4; // Skip next instruction
            else
                pc += 2; // Move to the next instruction
            break;

        case 0x6000: // 0x6XNN: Set VX to NN
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
            break;

        case 0x7000: // 0x7XNN: Add NN to VX
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
            break;

        case 0x8000:
            switch(opcode & 0x000F)
            {
                case 0x0000: // 0x8XY0: Set VX to the value of VY
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                case 0x0001: // 0x8XY1: Set VX to VX OR VY
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                case 0x0002: // 0x8XY2: Set VX to VX AND VY
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                case 0x0003: // 0x8XY3: Set VX to VX XOR VY
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                case 0x0004: // 0x8XY4: Add VY to VX and set VF to 1 if there's a carry
                    V[0xF] = (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] > 0xFF) ? 1 : 0;
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                case 0x0005: // 0x8XY5: Subtract VY from VX and set VF to 0 if there's a borrow
                    V[0xF] = (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) ? 1 : 0;
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                case 0x0006: // 0x8XY6: Shift VX right by 1 and set VF to the least significant bit of VX before the shift
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    pc += 2;
                    break;

                case 0x0007: // 0x8XY7: Set VX to VY minus VX and set VF to 0 if there's a borrow
                    V[0xF] = (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]) ? 1 : 0;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;

                case 0x000E: // 0x8XYE: Shift VX left by 1 and set VF to the most significant bit of VX before the shift
                    V[0xF] = (V[(opcode & 0x0F00) >> 8] & 0x80) >> 7;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    pc += 2;
                    break;

                default:
                    printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
                    break;
            }
            break;

        case 0x9000: // 0x9XY0: Skip the next instruction if VX does not equal VY
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
                pc += 4; // Skip next instruction
            else
                pc += 2; // Move to the next instruction
            break;

        case 0xA000: // 0xANNN: Set I to the address NNN
            I = opcode & 0x0FFF;
            pc += 2;
            break;

        case 0xB000: // 0xBNNN: Jump to the address NNN plus V0
            pc = (opcode & 0x0FFF) + V[0];
            break;

        case 0xC000: // 0xCXNN: Set VX to a random number AND NN
            V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
            pc += 2;
            break;

        case 0xD000: // 0xDXYN: Draw a sprite at coordinate (VX, VY) with N bytes of sprite data
{
    unsigned short x = V[(opcode & 0x0F00) >> 8];
    unsigned short y = V[(opcode & 0x00F0) >> 4];
    unsigned short height = opcode & 0x000F;
    V[0xF] = 0; // Reset collision flag

    for (unsigned short row = 0; row < height; ++row)
    {
        unsigned char spriteLine = memory[I + row];
        for (unsigned short col = 0; col < 8; ++col)
        {
            if ((spriteLine & (0x80 >> col)) != 0) // Check if the current bit is set
            {
                unsigned short pixel = (x + col) + ((y + row) * 64);
                if (gfx[pixel] == 1)
                    V[0xF] = 1; // Collision detected
                gfx[pixel] ^= 1; // Flip pixel (XOR operation)
            }
        }
    }

    drawFlag = true; // Set drawFlag to true since the screen has been updated
    pc += 2;
}
        break;


        case 0xE000:
            switch(opcode & 0x00FF)
            {
                case 0x009E: // 0xEX9E: Skip the next instruction if the key stored in VX is pressed
                    if (key[V[(opcode & 0x0F00) >> 8]])
                        pc += 4; // Skip next instruction
                    else
                        pc += 2; // Move to the next instruction
                    break;

                case 0x00A1: // 0xEXA1: Skip the next instruction if the key stored in VX is not pressed
                    if (!key[V[(opcode & 0x0F00) >> 8]])
                        pc += 4; // Skip next instruction
                    else
                        pc += 2; // Move to the next instruction
                    break;

                default:
                    printf("Unknown opcode [0xE000]: 0x%X\n", opcode);
                    break;
            }
            break;

        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007: // 0xFX07: Set VX to the value of the delay timer
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                    break;

                case 0x000A: // 0xFX0A: Wait for a key press and store the value of the key in VX
                    {
                        bool keyPressed = false;
                        for (int i = 0; i < 16; ++i)
                        {
                            if (key[i])
                            {
                                V[(opcode & 0x0F00) >> 8] = i;
                                keyPressed = true;
                                break;
                            }
                        }
                        if (!keyPressed)
                            return; // If no key pressed, exit this cycle
                        pc += 2;
                    }
                    break;

                case 0x0015: // 0xFX15: Set the delay timer to VX
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;

                case 0x0018: // 0xFX18: Set the sound timer to VX
                    sound_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;

                case 0x001E: // 0xFX1E: Add VX to I
                    I += V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                
                case 0x0029: // 0xFX29: Set I to the location of the sprite for the character in VX
                    I = V[(opcode & 0x0F00) >> 8] * 5; // Each sprite is 5 bytes
                    pc += 2;
                    break;

                case 0x0033: // 0xFX33: Store the BCD representation of VX in memory at I
                    memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I + 2] = V[(opcode & 0x0F00) >> 8] % 10;
                    pc += 2;
                    break;

                case 0x0055: // 0xFX55: Store registers V0 to VX in memory starting at address I
                    for (int i = 0; i <= (opcode & 0x0F00) >> 8; ++i)
                        memory[I + i] = V[i];
                    pc += 2;
                    break;

                case 0x0065: // 0xFX65: Fill registers V0 to VX with values from memory starting at address I
                    for (int i = 0; i <= (opcode & 0x0F00) >> 8; ++i)
                        V[i] = memory[I + i];
                    pc += 2;
                    break;

                default:
                    printf("Unknown opcode [0xF000]: 0x%X\n", opcode);
                    break;
            }
            break;

        default:
            printf("Unknown opcode: 0x%X\n", opcode);
            break;
    }

    // Update timers
    if(delay_timer > 0)
        --delay_timer;

    if(sound_timer > 0)
    {
        if(sound_timer == 1)
            printf("BEEP!\n");
        --sound_timer;
    }
}

