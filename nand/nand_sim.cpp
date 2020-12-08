#include <iostream>
#include <cstdlib>
#include <cstring>
#include "nand.h"

u32 data[SECTORS_PER_PAGE];
u32 spare;

#define N_BLOCKS		(BLKS_PER_BANK)
#define N_PAGES			(PAGES_PER_BLK)

#define SPARE(i,j,k)	(i * N_BANKS + j * N_BLOCKS + k * N_PAGES)
u32 *DATA(u32 i, u32 j, u32 k) {
	u32 idx = i * N_BANKS + j * N_BLOCKS + k * N_PAGES;	
	if (idx % 0xF == 0x0) memset(data, 0x00, DATA_SIZE); 
	else if (idx % 0xF == 0x1) memset(data, 0x11, DATA_SIZE); 
	else if (idx % 0xF == 0x2) memset(data, 0x22, DATA_SIZE); 
	else if (idx % 0xF == 0x3) memset(data, 0x33, DATA_SIZE); 
	else if (idx % 0xF == 0x4) memset(data, 0x44, DATA_SIZE); 
 	else if (idx % 0xF == 0x5) memset(data, 0x55, DATA_SIZE); 
	else if (idx % 0xF == 0x6) memset(data, 0x66, DATA_SIZE); 
	else if (idx % 0xF == 0x7) memset(data, 0x77, DATA_SIZE); 
	else if (idx % 0xF == 0x8) memset(data, 0x88, DATA_SIZE); 
	else if (idx % 0xF == 0x9) memset(data, 0x99, DATA_SIZE); 
	else if (idx % 0xF == 0xa) memset(data, 0xaa, DATA_SIZE); 
	else if (idx % 0xF == 0xb) memset(data, 0xbb, DATA_SIZE); 
	else if (idx % 0xF == 0xc) memset(data, 0xcc, DATA_SIZE); 
	else if (idx % 0xF == 0xd) memset(data, 0xdd, DATA_SIZE); 
	else if (idx % 0xF == 0xe) memset(data, 0xee, DATA_SIZE); 
	else memset(data, 0xff, DATA_SIZE); 

	return data;
}

int main(int argc, char *argv[])
{
	int i;
	
	Nand nand;

	nand.Nand_Init(N_BANKS, N_BLOCKS, N_PAGES);

	// write a block in full
	for (i = 0; i < N_PAGES; i++)
		nand.Nand_Write(0, 3, i, DATA(0, 3, i), SPARE(0, 3, i));

	// read the block in the reverse order
	for (i = N_PAGES - 1; i >= 0; i--)
		nand.Nand_Read(0, 3, i, DATA(0, 0, 0), &spare);


	nand.Nand_Write(0, 4, 0, DATA(0, 4, 0), SPARE(0, 4, 0));

	// trying to overwrite
	nand.Nand_Write(0, 4, 0, DATA(0, 4, 0), SPARE(0, 4, 0));

	// trying to write non-sequentially
	nand.Nand_Write(0, 4, 2, DATA(0, 4, 2), SPARE(0, 4, 2));

	// trying to read an empty page
	nand.Nand_Read(0, 4, 3, DATA(0, 0, 0), &spare);

	// erase a block
	nand.Nand_Erase(0, 4);

	// trying to erase an empty block
	nand.Nand_Erase(1, 0);

	// trying to write the erased block
	nand.Nand_Write(0, 4, 0, DATA(0, 4, 0), SPARE(0, 4, 0));
	nand.Nand_Write(0, 4, 1, DATA(0, 4, 1), SPARE(0, 4, 1));

	// invalid page number
	nand.Nand_Write(0, 4, -1, DATA(0, 4, -1), SPARE(0, 4, -1));
	nand.Nand_Read(0, 4, -1, DATA(0, 0, 0), &spare);

	// invalid block number
	nand.Nand_Write(0, -1, 0, DATA(0, -1, 0), SPARE(0, -1, 0));
	nand.Nand_Read(0, -1, 0, DATA(0, 0, 0), &spare);
	nand.Nand_Erase(0, -1);
	nand.Nand_Erase(0, N_BLOCKS);

	// invalid bank number
	nand.Nand_Write(-1, 0, 0, DATA(-1, 0, 0), SPARE(-1, 0, 0));
	nand.Nand_Read(-1, 0, 0, DATA(0, 0, 0), &spare);

	nand.Nand_Blkdump(0, 3);
	nand.Nand_Blkdump(0, 4);
	nand.Nand_Blkdump(1, 4);

	return 0;
}

