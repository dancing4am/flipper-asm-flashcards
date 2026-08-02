#pragma once

typedef struct {
    const char* name;
    const char* meaning;
    const char* detail;
} Card;

#define COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef enum {
    ScreenTitle,
    ScreenCategory,
    ScreenMenu,
    ScreenMode,
    ScreenCard,
    ScreenDetail,
    ScreenQuiz,
} Screen;

typedef enum {
    ModeCards,
    ModeQuizNameToMeaning,
    ModeQuizMeaningToName,
} Mode;

typedef struct {
    const char* label;
} ModeItem;

const ModeItem modes[] = {
    {"Cards"},
    {"Quiz N->M"},
    {"Quiz M->N"},
};

const Card easy_cards[] = {
    {"MOV",  "Copy a value", NULL},
    {"ADD",  "Add", NULL},
    {"SUB",  "Subtract", NULL},
    {"XOR",  "Bitwise XOR", NULL},
    {"AND",  "Bitwise AND", NULL},
    {"OR",   "Bitwise OR", NULL},
    {"PUSH", "Push onto stack", NULL},
    {"POP",  "Pop from stack", NULL},
    {"CALL", "Call a function", NULL},
    {"RET",  "Return from call", NULL},
    {"JMP",  "Jump", NULL},
    {"CMP",  "Compare two values", NULL},
    {"INC",  "Increment by 1", NULL},
    {"DEC",  "Decrement by 1", NULL},
    {"LEA",  "Load address", NULL},
    {"NOP",  "Do nothing", NULL},
};

const Card medium_cards[] = {
    {"TEST", "AND, set flags only", "Like AND but throws away\nthe result, only sets\nflags. test eax,eax\nchecks for zero."},
    {"JE",   "Jump if equal", "Taken if ZF=1 (last CMP\nwas equal). Same as JZ."},
    {"JNE",  "Jump if not equal", "Taken if ZF=0. Same as\nJNZ. Very common loop\nexit."},
    {"JG",   "Jump if greater", "Signed >. Used after CMP\non signed values."},
    {"JL",   "Jump if less", "Signed <. The signed\ncounterpart of JB."},
    {"JGE",  "Jump if greater/equal", "Signed >=. Taken when\nSF==OF."},
    {"JLE",  "Jump if less/equal", "Signed <=. ZF=1 or\nSF!=OF."},
    {"MUL",  "Unsigned multiply", "Unsigned multiply.\nResult goes to EDX:EAX\n(high bits in EDX)."},
    {"IMUL", "Signed multiply", "Signed multiply. Has 1,\n2, and 3-operand forms.\nThe usual mul in code."},
    {"DIV",  "Unsigned divide", "Unsigned divide EDX:EAX.\nQuotient in EAX,\nremainder in EDX."},
    {"IDIV", "Signed divide", "Signed divide. Needs\nCDQ/CQO first to sign-\nextend, or it faults."},
    {"NEG",  "Negate (two's comp)", "Two's complement negate.\nNEG x = 0 - x."},
    {"NOT",  "Bitwise NOT", "Flips every bit (~x).\nDoesn't touch flags."},
    {"SHL",  "Shift left", "Shift bits left, fill 0.\nEach shift = multiply\nby 2."},
    {"SHR",  "Shift right (logical)", "Logical shift right, fill\n0. Unsigned divide by 2.\nSigned uses SAR."},
    {"MOVZX","Move, zero-extend", "Move small to large, fill\ntop with zeros. For\nunsigned values."},
    {"MOVSX","Move, sign-extend", "Move small to large, copy\nsign bit up. For signed\nvalues."},
    {"XCHG", "Exchange two values", "Swap two operands. With\nmemory it auto-locks\n(atomic)."},
};

const Card hard_cards[] = {
    {"SAR",    "Shift right (arith)", "Arithmetic shift right,\nkeeps the sign bit.\nSigned divide by 2."},
    {"ROL",    "Rotate left", "Bits pushed off the left\nwrap back on the right.\nNothing is lost."},
    {"ROR",    "Rotate right", "Bits pushed off the right\nwrap around to the left."},
    {"RCL",    "Rotate left w/ carry", "Rotate left through CF;\nthe carry flag joins\nthe bit ring."},
    {"RCR",    "Rotate right w/ carry", "Rotate right through the\ncarry flag."},
    {"CDQ",    "Sign-extend EAX->EDX", "Fills EDX with EAX's sign\nbit. Run before 32-bit\nsigned IDIV."},
    {"CQO",    "Sign-extend RAX->RDX", "64-bit CDQ. Fills RDX\nfrom RAX's sign before\nIDIV."},
    {"BT",     "Bit test", "Copies bit n into CF.\nLeaves the value alone."},
    {"BTS",    "Bit test and set", "Copies bit n to CF, then\nsets that bit to 1."},
    {"BSF",    "Bit scan forward", "Finds the index of the\nlowest set bit (from\nbit 0 up)."},
    {"BSR",    "Bit scan reverse", "Finds the index of the\nhighest set bit."},
    {"CMOVE",  "Move if equal", "Conditional move: copies\nonly if ZF=1. Avoids a\nbranch (harder to trace)."},
    {"SETE",   "Set byte if equal", "Sets a byte to 1 if ZF=1,\nelse 0. Turns a flag\ninto a 0/1 value."},
    {"CMPXCHG","Compare and exchange", "Cmp EAX with dest. Equal:\nstore src. Else: load\ndest into EAX. CAS core."},
    {"LOCK",   "Lock bus prefix", "Prefix that makes the\nnext read-modify-write\natomic across cores."},
    {"CPUID",  "CPU identification", "Returns CPU features/id.\nAlso serializes; shows up\nin timing/anti-debug."},
    {"RDTSC",  "Read timestamp counter", "Reads the CPU cycle\ncounter into EDX:EAX.\nCommon in anti-debug."},
    {"MOVSB",  "Move string byte", "Copies [ESI] to [EDI] and\nadvances both. REP MOVSB\ncopies a block."},
    {"STOSB",  "Store string byte", "Stores AL to [EDI] and\nadvances. REP STOSB\nfills memory (memset)."},
    {"LODSB",  "Load string byte", "Loads [ESI] into AL and\nadvances ESI."},
    {"SCASB",  "Scan string byte", "Compares AL with [EDI].\nREP SCASB scans bytes,\ne.g. strlen."},
};

const Card float_cards[] = {
    {"MOVSS",    "Move scalar float", "Moves one 32-bit float\nin/out of an xmm\nregister."},
    {"MOVSD",    "Move scalar double", "Moves one 64-bit double\ninto/out of xmm."},
    {"MOVAPS",   "Move aligned packed", "Moves 4 floats at once.\nAddress must be 16-byte\naligned or it faults."},
    {"MOVUPS",   "Move unaligned packed", "Moves 4 floats, any\nalignment. Slower but\nsafe."},
    {"ADDSS",    "Add scalar float", "Adds the low float of\ntwo xmm registers."},
    {"SUBSS",    "Sub scalar float", "Subtracts the low\nfloats."},
    {"MULSS",    "Mul scalar float", "Multiplies the low\nfloats."},
    {"DIVSS",    "Div scalar float", "Divides the low floats."},
    {"ADDSD",    "Add scalar double", "Adds the low doubles."},
    {"MULSD",    "Mul scalar double", "Multiplies the low\ndoubles."},
    {"SQRTSS",   "Square root (float)", "Square root of the low\nfloat."},
    {"COMISS",   "Compare, set flags", "Compares two floats and\nsets the CPU flags\n(ordered)."},
    {"UCOMISD",  "Compare double, flags", "Compares two doubles and\nsets flags. Handles NaN\nquietly."},
    {"CVTSI2SS", "Int -> float", "Converts an integer to a\nfloat."},
    {"CVTSS2SI", "Float -> int", "Converts a float to an\ninteger (rounded)."},
    {"CVTTSS2SI","Float -> int (trunc)", "Float to int, truncated\n(chops toward zero)."},
    {"CVTSS2SD", "Float -> double", "Widens a float to a\ndouble."},
    {"CVTSD2SS", "Double -> float", "Narrows a double to a\nfloat."},
    {"XORPS",    "XOR packed (zero reg)", "xorps xmm,xmm makes it\n0.0. Common way to zero\na float register."},
    {"ANDPS",    "AND packed float", "Bitwise AND on floats.\nUsed for abs value and\nsign masks."},
    {"PXOR",     "XOR packed int", "Integer XOR on xmm. Also\na common zeroing idiom."},
    {"MOVD",     "Move dword to/from xmm", "Moves 32 bits between a\ngeneral reg and xmm."},
    {"MOVQ",     "Move qword to/from xmm", "Moves 64 bits to/from\nxmm."},
    {"SHUFPS",   "Shuffle packed float", "Rearranges the 4 floats\nin a vector by a mask."},
    {"ADDPS",    "Add packed (SIMD)", "Adds 4 float pairs at\nonce (SIMD)."},
    {"MULPS",    "Mul packed (SIMD)", "Multiplies 4 float pairs\nat once."},
};

const Card cond_cards[] = {
    {"JE / JZ",   "equal (ZF=1)", "Taken when the compared\nvalues were equal.\nSigned or unsigned."},
    {"JNE / JNZ", "not equal (ZF=0)", "Taken when the values\ndiffer. The usual loop\ncondition."},
    {"JA / JNBE", "unsigned above", "Unsigned >. Uses CF and\nZF. Pair with unsigned\nCMP."},
    {"JAE / JNB", "unsigned above/eq", "Unsigned >=. Taken when\nCF=0."},
    {"JB / JNAE", "unsigned below", "Unsigned <. Taken when\nCF=1."},
    {"JBE / JNA", "unsigned below/eq", "Unsigned <=. CF=1 or\nZF=1."},
    {"JG / JNLE", "signed greater", "Signed >. Uses SF, OF,\nand ZF."},
    {"JGE / JNL", "signed greater/eq", "Signed >=. Taken when\nSF==OF."},
    {"JL / JNGE", "signed less", "Signed <. Taken when\nSF!=OF."},
    {"JLE / JNG", "signed less/eq", "Signed <=. ZF=1 or\nSF!=OF."},
    {"JS / JNS",  "sign flag / not", "JS taken if the result\nis negative (SF=1);\nJNS is the opposite."},
    {"JO / JNO",  "overflow / not", "JO taken on signed\noverflow (OF=1)."},
    {"JC / JNC",  "carry / not", "JC taken if CF=1 (carry,\nor unsigned borrow)."},
    {"JP / JNP",  "parity even / odd", "JP taken if the low byte\nhas an even number of\n1 bits."},
};

const Card rule_cards[] = {
    {"A / B",      "unsigned compare", "Above/Below = unsigned\ncompares. JA/JB means\nthe value is unsigned."},
    {"G / L",      "signed compare", "Greater/Less = signed\ncompares. JG/JL means\nthe value is signed."},
    {"N prefix",   "means NOT", "N inverts a condition.\nJNE = not equal,\nJNB = not below."},
    {"cc suffix",  "same on J/SET/CMOV", "The condition code (E,\nNE, G...) is identical\non Jcc, SETcc, CMOVcc."},
    {"...SS",      "scalar single (1 float)", "SS = one 32-bit float.\nADDSS adds a single\nfloat."},
    {"...SD",      "scalar double (1 dbl)", "SD = one 64-bit double."},
    {"...PS",      "packed single (SIMD)", "PS = four floats at\nonce (SIMD, parallel)."},
    {"...PD",      "packed double (SIMD)", "PD = two doubles at\nonce (SIMD)."},
};

const Card gp_cards[] = {
    {"RAX",     "Return / accumulator", "Function return value\nlands here. Pairs with\nRDX in MUL/DIV."},
    {"RBX",     "Callee-saved base", "Callee-saved: functions\nmust restore it. Often\nholds a long-lived ptr."},
    {"RCX",     "Counter", "LOOP/REP counter. Also\nan argument register\n(see Args deck)."},
    {"RDX",     "Data / high result", "High half of MUL/DIV\nwith RAX. Also an\nargument register."},
    {"RSI",     "Source index", "Source pointer for\nstring ops (MOVS/LODS).\nSysV: 2nd argument."},
    {"RDI",     "Dest index", "Dest pointer for string\nops (MOVS/STOS/SCAS).\nSysV: 1st argument."},
    {"RSP",     "Stack pointer", "Top of the stack. Moved\nby PUSH/POP/CALL/RET.\nLocals live below it."},
    {"RBP",     "Frame pointer", "Base of the stack frame.\nLocals at RBP-x, args\nat RBP+x (if not\noptimized out)."},
    {"R8-R11",  "Caller-saved scratch", "Volatile: any call may\nclobber them. R10: Linux\nsyscall arg 4. R11:\nSYSCALL scratch."},
    {"R12-R15", "Callee-saved", "Callee-saved. Survive\ncalls, so compilers\nkeep loop vars and\nobjects here."},
    {"RIP",     "Instruction pointer", "Address of the next\ninstruction. RIP-\nrelative addressing is\nhow x64 finds globals."},
};

const Card arg_cards[] = {
    {"RDI",     "Arg 1 (SysV)", "1st argument on Linux/\nmacOS. Windows uses\nRCX instead."},
    {"RSI",     "Arg 2 (SysV)", "2nd argument on Linux/\nmacOS. Windows uses\nRDX."},
    {"RDX",     "Arg 3 SysV / 2 Win", "3rd argument in SysV,\n2nd argument on\nWindows."},
    {"RCX",     "Arg 4 SysV / 1 Win", "4th argument in SysV.\nOn Windows the 1st arg\n(this ptr in C++)."},
    {"R8",      "Arg 5 SysV / 3 Win", "5th argument in SysV,\n3rd argument on\nWindows."},
    {"R9",      "Arg 6 SysV / 4 Win", "6th argument in SysV,\n4th argument on\nWindows."},
    {"RAX",     "Return value", "Return value in both\nconventions. Also the\nsyscall number on\nLinux."},
    {"XMM0-7",  "Float args", "Floats/doubles pass in\nXMM regs, return in\nXMM0. Windows uses\nXMM0-3 only."},
    {"SysV ABI","Linux/macOS call conv", "Args: RDI RSI RDX RCX\nR8 R9. Return: RAX.\nExtras on the stack."},
    {"Win x64", "Windows call conv", "Args: RCX RDX R8 R9.\nRet: RAX. 32-byte\nshadow space. Seen in\ngames/anti-cheat work."},
};

const Card flag_cards[] = {
    {"ZF", "Zero flag", "Set when a result is 0.\nCMP sets it on equal.\nRead by JE/JNE, SETE."},
    {"SF", "Sign flag", "Copy of the result's\ntop bit (negative).\nRead by JS/JNS and the\nsigned jumps."},
    {"CF", "Carry flag", "Unsigned overflow or\nborrow. Set by ADD/SUB,\nshifts. Read by JB/JAE,\nADC/SBB."},
    {"OF", "Overflow flag", "Signed overflow. Read\nby JO/JNO; JG/JL test\nSF!=OF."},
    {"PF", "Parity flag", "Even number of 1 bits\nin the low byte. Read\nby JP/JNP. Shows up\nafter FP compares."},
    {"DF", "Direction flag", "String op direction:\nDF=0 forward, DF=1\nbackward. CLD/STD set\nit. ABI expects DF=0."},
};

typedef struct {
    const char* label;
    const Card* cards;
    int count;
} Deck;

const Deck instruction_decks[] = {
    {"Easy", easy_cards, COUNT(easy_cards)},
    {"Medium", medium_cards, COUNT(medium_cards)},
    {"Hard", hard_cards, COUNT(hard_cards)},
    {"Float", float_cards, COUNT(float_cards)},
    {"Cond", cond_cards, COUNT(cond_cards)},
    {"Rule", rule_cards, COUNT(rule_cards)},
};

const Deck register_decks[] = {
    {"GP", gp_cards, COUNT(gp_cards)},
    {"Args", arg_cards, COUNT(arg_cards)},
    {"Flags", flag_cards, COUNT(flag_cards)},
};

typedef struct {
    const char* label;
    const Deck* decks;
    int count;
} Category;

const Category categories[] = {
    {"Instruction", instruction_decks, COUNT(instruction_decks)},
    {"Register", register_decks, COUNT(register_decks)},
};