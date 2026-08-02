#pragma once

typedef struct {
    const char* name;
    const char* meaning;
    const char* detail;
} Card;

#define COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef enum {
    ScreenTitle,
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

typedef struct {
    const char* label;
    const Card* cards;
    int count;
} Deck;

const Deck decks[] = {
    {"Easy", easy_cards, COUNT(easy_cards)},
    {"Medium", medium_cards, COUNT(medium_cards)},
    {"Hard", hard_cards, COUNT(hard_cards)},
    {"Float", float_cards, COUNT(float_cards)},
    {"Cond", cond_cards, COUNT(cond_cards)},
    {"Rule", rule_cards, COUNT(rule_cards)},
};