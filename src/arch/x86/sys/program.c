#include <sys/program.h>

#include <sys/scheduler.h>
#include <sys/syscalls.h>

#include <gfx/vga.h>
#include <gfx/vga_colors.h>

#include <lib/utils.h>

void dirty_sleep()
{
    for(int i=0; i<17000000;i++) 
    {
        __asm("nop;");
    }
}

void program_init(Program *program)
{
  size_t k = 0;
  while(program->program_data[k] != PROGRAM_END &&
    k < MAX_PROGRAM_CAPACITY)
    k++;
  program->program_length = k;
  program->ip=0;
  program->flags=0;

  memset(program->registers, 0, REGISTER_COUNT);
}

size_t pop_instruction(Program *program)
{
  size_t inst = program->program_data[program->ip];
  program->ip++;
  return inst;
}

void program_step(Program *program)
{
  if (program->ip > program->program_length)
    return;

  
  size_t inst = pop_instruction(program);
  switch(inst)
  {
      case PROGRAM_LOAD_IMMEDIATE:
      {
        size_t reg = pop_instruction(program);
        int64_t value  = pop_instruction(program);
        program->registers[reg] = value;
      }
      break;
      
      case PROGRAM_MOV:
      {
        size_t reg0 = pop_instruction(program);
        size_t reg1  = pop_instruction(program);
        program->registers[reg0] = program->registers[reg1];
      }
      break;

      case PROGRAM_ADD:
      {
        size_t reg0 = pop_instruction(program);
        size_t reg1  = pop_instruction(program);
        program->registers[reg0] = program->registers[reg0]+program->registers[reg1];
      }
      break;
    
      case PROGRAM_INC:
      {
        size_t reg0 = pop_instruction(program);
        int64_t value  = pop_instruction(program);
        program->registers[reg0] += value;
      }
      break;
      
      case PROGRAM_PRINT: 
      {
        size_t reg0 = pop_instruction(program);
        int64_t data = program->registers[reg0];

        vga_setcolor(VGA_COLOR_CYAN);
        vga_printf("\n%d<< %d\n", program->pid, data);
        vga_restore_color();
      }
      break;
      
      case PROGRAM_SYSCALL: 
      {
        size_t id = pop_instruction(program);
        size_t reg0 = pop_instruction(program);
        char data = program->registers[reg0];
        program->registers[REGISTER_RESULT]=syscall(id, data);   
      }
      break;
      
      case PROGRAM_SYSCALL_IMMEDIATE: 
      {
        size_t id = pop_instruction(program);
        size_t data = pop_instruction(program);
        program->registers[REGISTER_RESULT]=syscall(id, data);   
      }
      break;

      case PROGRAM_TEST:
      {
        size_t reg0 = pop_instruction(program);
        size_t reg1  = pop_instruction(program);

        if (program->registers[reg0] == program->registers[reg1])
          program->flags |= ZERO_FLAG;
        else
          program->flags &= ~ZERO_FLAG;
      }
      break;
      
      case PROGRAM_RESET:
      {
        program->flags = 0;
      }
      break;
    
      case PROGRAM_JUMP:
      {
        program->ip = pop_instruction(program);
      }
      break;
      
      case PROGRAM_JUMP_IF:
      { 
        if (program->flags & ZERO_FLAG)
          program->ip = pop_instruction(program);
      }
      break;

      case PROGRAM_LOAD_MEMORY_BASE:
      {
        size_t reg0 = pop_instruction(program);
        program->registers[reg0] = 
          (size_t)&(program->program_data[program->program_length+1]);
      }
      break;
      
      case PROGRAM_DEREF:
      {
        size_t reg0 = pop_instruction(program);
        size_t reg1  = pop_instruction(program);

        // TODO: this will crash the os if invalid address
        // is given
        program->registers[reg0] = *((size_t*)program->registers[reg1]);
      }
      break;
    
      case PROGRAM_END: 
      {
        vga_setcolor(VGA_COLOR_CYAN);
        vga_printf("\n%d>> %d\n", program->pid, program->registers[REGISTER_RESULT]);
        vga_restore_color();
      }
      break;
  }
  
  dirty_sleep();
}        
