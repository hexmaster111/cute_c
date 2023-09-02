#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>

#define NOT_IMPL 0
#define ASSERT(expr)                                                                  \
    if ((expr) == 0)                                                                  \
    {                                                                                 \
        printf("ASSERT FAILED:(%s)==0 FILE:%s LINE:%i\n", #expr, __FILE__, __LINE__); \
        fflush(stdout);                                                               \
        raise(SIGTRAP);                                                               \
        exit(-1);                                                                     \
    }

enum oppcode
{

    OPP_NOOP = 0b0000000000000000, // No registers used
    OPP_EXIT = 0b1000000000000001, // u8_0: Exit code
    OPP_SSTR = 0b0010000000000010  // data: ascii string | u8_0: len
};
#define _OPP_MASK 0b0000000111111111 // 9 bits for opp code encoding
#define u8_0MASK 0b1                 // 7 bits for arg identifiers
#define u8_1MASK 0b01
#define dataMASK 0b001

#define _OPP_GET(x) ((x) & OPP_MASK)
#define u8_0GET(x) ((x) & u8_0MASK)
#define u8_1GET(x) ((x) & u8_1MASK)
#define dataGET(x) ((x) & dataMASK)

typedef struct instruction_t
{
    enum oppcode opp;
    __uint8_t u8_0; // TODO: turn all the sprate aregs into one big block of data and then we can squich things down
    __uint8_t u8_1;
    const __uint8_t data[255];
} *instruction_ptr;

const char *opp_to_string(enum oppcode opp)
{
    if (opp == OPP_EXIT)
        return "EXIT";
    if (opp == OPP_NOOP)
        return "NOOP";
    return "UNKNOWN";
}

const struct instruction_t _DEV_instructions[] = {
    {.opp = OPP_NOOP},
    {.opp = OPP_SSTR, .u8_0 = 11, .data = "Hello world"},
    {.opp = OPP_EXIT, .u8_0 = 1},
};

typedef struct registers_t
{
    size_t u8_0;
    size_t u8_1;
    size_t pc;      // Program counter, Points at the current instruction
    size_t *arr[3]; // array containing pointers to each register in this struct
                    // So that we can index off of it

    __uint8_t data[255]; // the data section of the program, should likely be dynamic
} *registers_ptr_t;

void reigsters_init(registers_ptr_t r)
{
    r->pc = 0;
    r->u8_0 = 0;
    r->u8_1 = 0;
    r->arr[0] = &r->pc;   // First register is the program counter
    r->arr[1] = &r->u8_0; // 2nd register is u8 arg #0
    r->arr[2] = &r->u8_1; // 3rd register is u8 arg #1
    memset(r->data, 0, 255);
}

typedef struct instruction_t **program_ptr_t; // a program is a list of instructions

typedef struct runtime_t
{
    registers_ptr_t reg;
    program_ptr_t prog;
} *runtime_ptr;

void runtime_init(runtime_ptr rt, registers_ptr_t reg, program_ptr_t prog)
{
    ASSERT(rt != NULL);
    ASSERT(reg != NULL);
    ASSERT(prog != NULL);
    rt->reg = reg;
    rt->prog = prog;
}

size_t *_rt_readreg(runtime_ptr rt, int reg)
{
    return rt->reg->arr[reg];
}

size_t *_rt_getpc(runtime_ptr rt)
{
    return _rt_readreg(rt, 0);
}

void _rt_inc_pc(runtime_ptr rt)
{
    size_t *pc = _rt_getpc(rt);
    *pc = (*pc) + 1;
}

instruction_ptr __rt_get_current_instruction(runtime_ptr rt)
{
    size_t pc = *_rt_getpc(rt);
    return rt->prog[pc];
}

// return 1 to be called agine.
int runtime_run(runtime_ptr rt)
{
    instruction_ptr inst = __rt_get_current_instruction(rt);
    _rt_inc_pc(rt);
    if (u8_0GET(inst->opp))
    {
        rt->reg->u8_0 = inst->u8_0;
    }
    if (u8_1GET(inst->opp))
    {
        rt->reg->u8_1 = inst->u8_1;
    }
    if (dataGET(inst->opp))
    {
        //TODO: LEN
        memcpy(rt->reg->data, inst->data, inst->u8_0);
    }

    if (inst->opp == OPP_EXIT)
    {
        return 0;
    }
    else if (inst->opp == OPP_NOOP)
    {
        // do nothing
        printf("NOOP\n");
    }
    else
    {
        ASSERT(false || "Invalid Instruction");
    }

    return 1;
}

char *runtime_dump(runtime_ptr rt)
{
    char *str = (char *)malloc(sizeof(char) * 1024);
    sprintf(str, "PROG: 0x%04lX\tLINE:%s\nu8_0: 0x%04lX\tu8_1: 0x%04lX\n",
            rt->reg->pc,
            opp_to_string(rt->prog[rt->reg->pc - 1]->opp),
            rt->reg->u8_0,
            rt->reg->u8_1);
    return str;
}

int main(int argc, char *argv[])
{
    struct registers_t r;
    reigsters_init(&r);

    // TODO: Replace this with whatever is loading it from a file or whatever.
    struct instruction_t **prog = NULL;
    prog = (struct instruction_t **)malloc(sizeof(struct instruction_t *) * (sizeof(_DEV_instructions) / sizeof(struct instruction_t)));

    // Copy the instructions into the prog array
    for (size_t i = 0; i < sizeof(_DEV_instructions) / sizeof(struct instruction_t); i++)
    {
        prog[i] = (struct instruction_t *)malloc(sizeof(struct instruction_t));
        *prog[i] = _DEV_instructions[i];
    }

    // entry point is pc0,
    struct runtime_t rt = {};
    runtime_init(&rt, &r, prog);

    while (runtime_run(&rt) == 1)
    {
        // Here could be where a debugger gets hooked up, called
        // every instruction here.
    };

    char *dump = NULL;
    dump = runtime_dump(&rt);
    printf("EXIT: %ld\n", rt.reg->u8_0); // exit code stored here when exiting
    printf("\nRUNTIME EXIT DUMP:\n%s\nEND DUMP\n", dump);
    free(dump);
    return rt.reg->u8_0;
}