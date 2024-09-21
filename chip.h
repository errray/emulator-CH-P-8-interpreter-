#ifndef CHIP_H_INCLUDED
#define CHIP_H_INCLUDED

class chip8 {
public:
    chip8(); // Constructor
    ~chip8(); //Deconstructor
    bool drawFlag;
    void initialize(); // Initialize the emulator
    void loadFontSet(); // Load the CHIP-8 font set into memory
    bool loadGame(const char* fileName); // Load a game into memory
    void emulateCycle(); // Emulate a single cycle of the CHIP-8
    unsigned char key[16];          // Keypad (16 keys)
    unsigned char gfx[64 * 32]; // Display (64x32 pixels)
private:
    unsigned short opcode;          // Current opcode
    unsigned char memory[4096];     // CHIP-8 memory
    unsigned char V[16];            // 16 general-purpose registers (V0 to VF)
    unsigned short I;               // Index register
    unsigned short pc;              // Program counter
    unsigned char delay_timer;      // Delay timer
    unsigned char sound_timer;      // Sound timer
    unsigned short stack[16];       // Stack (16 levels)
    unsigned short sp;              // Stack pointer

};

#endif // CHIP_H_INCLUDED
