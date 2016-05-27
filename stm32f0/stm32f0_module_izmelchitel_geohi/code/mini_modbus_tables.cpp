#include "mini_modbus_tables.h"
#include "System.h"


U16 mini_modbus_RO_memory[MINI_MODBUS_RO_TABLE_SIZE];
U16 mini_modbus_RW_memory[MINI_MODBUS_RW_TABLE_SIZE];

#define DEF_CURRENT1	130
#define DEF_CURRENT2	700

#define DEF_CAL_VAL_1	22
#define DEF_CAL_VAL_2	110


U16 mini_modbus_RW_flash_memory[MINI_MODBUS_RW_FLASH_TABLE_SIZE];



RO_table_t & RO_table = *(RO_table_t *) mini_modbus_RO_memory;
RW_table_t & RW_table = *(RW_table_t *) mini_modbus_RW_memory;
ROM_table_t & ROM_table = *(ROM_table_t *) mini_modbus_RW_flash_memory;


void set_init_values()
{
	RW_table.number = ROM_table.default_number_to_execute;
}


bool check_flash(void)
{
	if (ROM_table.period < 500)
			return false;
	return true;
}




const volatile U16 mini_modbus_RW_real_flash_memory[512] __attribute__((at( FLASH_DATA_WITH_MODIFY))) = 
{0x0101, 2000, 1000, 0, 0, 500, 400, 500, 800, 500, 1200, 500, 1600, 500} ;




