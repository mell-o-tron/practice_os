// Simple (P)Ata HDD (Hard Disk Drive) Polling Driver using PIO Mode (instead of the better DMA)
// Inspirations and Sources: (https://wiki.osdev.org/ATA_PIO_Mode)

#include "disk.h"
#include "port_io.h"
#include "vga_text.h"
#include "../memory/dynamic_mem.h"

#define STATUS_BSY      0x80
#define STATUS_RDY      0x40
#define STATUS_DRQ      0x08
#define STATUS_DF       0x20
#define STATUS_ERR      0x01

// TODO implement something on the lines of "took too long to respond"
void wait_BSY(){
	while(inb(0x1F7) & STATUS_BSY){;};
}

void wait_DRQ(){
	while(!(inb(0x1F7) & STATUS_RDY)){;};
}

bool check_ERR(){
	return ((inb(0x1F7) & STATUS_ERR) != 0);
}


// as specified on https://wiki.osdev.org/ATA_PIO_Mode#IDENTIFY_command
uint16_t* identify_ata(uint8_t drive){
	outb(0x1F6, drive);
	outb(0x1F2, 0);
	outb(0x1F3, 0);
	outb(0x1F4, 0);
	outb(0x1F5, 0);

	outb(0x1F7, 0xEC); // send identify command

	uint8_t status = inb(0x1F7);
	if (status == 0){
		kprint("Error: drive does not exist\n");
		return NULL;
	}
	wait_BSY();

	// check, in case drive does not follow spec
	if((inb(0x1F4) | inb(0x1F5)) != 0){
		kprint("Error: drive is not ATA\n");
		return NULL;
	}
	wait_DRQ();

	if (check_ERR()){
		kprint("ATA Identify Error\n");
		return NULL;
	}

	uint16_t * res = kmalloc(sizeof(uint16_t) * 256);

	for (int i = 0; i < 256; i++){
		res[i] = inw(0x1F0);
	}

	return res;

}



void ata_print_error(uint8_t error) {
    kprint("Error details: ");
    if(error & 0x01) kprint("AMNF ");  // Address Mark Not Found
    if(error & 0x02) kprint("TK0NF "); // Track 0 Not Found
    if(error & 0x04) kprint("ABRT ");  // Aborted Command
    if(error & 0x08) kprint("MCR ");   // Media Change Request
    if(error & 0x10) kprint("IDNF ");  // ID Not Found
    if(error & 0x20) kprint("MC ");    // Media Changed
    if(error & 0x40) kprint("UNC ");   // Uncorrectable Data
    if(error & 0x80) kprint("BBK ");   // Bad Block
    kprint("\n");
}


void check_ata_error(void) {
    uint8_t status = inb(0x1F7); // Read the status register

    // Check the ERR bit (bit 0)
    if (status & 0x01) {
        uint8_t error = inb(0x1F1); // Read the error register
        kprint("ATA command error: status = 0x");
        kprint(tostring_inplace(status, 16));
        kprint(", error = 0x");
        kprint(tostring_inplace(error, 16));
        kprint("\n");
        ata_print_error(error);
    } else {
        // kprint("ATA command completed successfully, status = 0x");
        // kprint(tostring_inplace(status, 16));
        // kprint("\n");
    }
}

uint16_t* fake_disk;

void initialize_fake_disk (){
	fake_disk = kmalloc(5120);		// 10 sectors of fake disk
	for (int i = 0; i < 2560; i++)
		fake_disk[i] = 0;
	
	// setting fake disk to a snapshot of actual disk of mell_os feb 2025 version
	fake_disk[0] = 0x7270;
	fake_disk[1] = 0x6E69;
	fake_disk[2] = 0x2E74;
	fake_disk[3] = 0x6962;
	fake_disk[4] = 0x006E;
	fake_disk[5] = 0x0003;
	fake_disk[7] = 0x0001;
	fake_disk[8] = 0x7277;
	fake_disk[9] = 0x7469;
	fake_disk[10] = 0x2E65;
	fake_disk[11] = 0x6962;
	fake_disk[12] = 0x006E;
	fake_disk[13] = 0x0004;
	fake_disk[15] = 0x0001;
	fake_disk[16] = 0x6162;
	fake_disk[17] = 0x616E;
	fake_disk[18] = 0x616E;
	fake_disk[21] = 0x0005;
	fake_disk[23] = 0x0001;
	fake_disk[24] = 0x696E;
	fake_disk[25] = 0x6867;
	fake_disk[26] = 0x6D74;
	fake_disk[27] = 0x6961;
	fake_disk[28] = 0x006C;
	fake_disk[29] = 0x0006;
	fake_disk[31] = 0x0002;
	fake_disk[256] = 0xFF00;
	fake_disk[512] = 0xB804;
	fake_disk[514] = 0x00BB;
	fake_disk[515] = 0x0100;
	fake_disk[517] = 0xB917;
	fake_disk[518] = 0x0070;
	fake_disk[519] = 0x00BA;
	fake_disk[520] = 0x1000;
	fake_disk[522] = 0xCD80;
	fake_disk[523] = 0xC348;
	fake_disk[524] = 0x656C;
	fake_disk[525] = 0x6C6F;
	fake_disk[526] = 0x2077;
	fake_disk[527] = 0x6569;
	fake_disk[528] = 0x7264;
	fake_disk[529] = 0x6F21;
	fake_disk[530] = 0x0A0D;
	fake_disk[768] = 0xB804;
	fake_disk[770] = 0x00BB;
	fake_disk[771] = 0x0500;
	fake_disk[773] = 0xB917;
	fake_disk[774] = 0x0070;
	fake_disk[775] = 0x00BA;
	fake_disk[776] = 0x1500;
	fake_disk[778] = 0xCD80;
	fake_disk[779] = 0xC348;
	fake_disk[780] = 0x656C;
	fake_disk[781] = 0x6C6F;
	fake_disk[782] = 0x206D;
	fake_disk[783] = 0x6973;
	fake_disk[784] = 0x7465;
	fake_disk[785] = 0x7220;
	fake_disk[786] = 0x6669;
	fake_disk[787] = 0x6C65;
	fake_disk[788] = 0x210A;
	fake_disk[789] = 0x0D00;
	fake_disk[1280] = 0x5468;
	fake_disk[1281] = 0x6973;
	fake_disk[1282] = 0x2069;
	fake_disk[1283] = 0x7320;
	fake_disk[1284] = 0x7468;
	fake_disk[1285] = 0x6520;
	fake_disk[1286] = 0x6E69;
	fake_disk[1287] = 0x6768;
	fake_disk[1288] = 0x7420;
	fake_disk[1289] = 0x6D61;
	fake_disk[1290] = 0x696C;
	fake_disk[1291] = 0x2063;
	fake_disk[1292] = 0x726F;
	fake_disk[1293] = 0x7373;
	fake_disk[1294] = 0x696E;
	fake_disk[1295] = 0x6720;
	fake_disk[1296] = 0x7468;
	fake_disk[1297] = 0x6520;
	fake_disk[1298] = 0x626F;
	fake_disk[1299] = 0x7264;
	fake_disk[1300] = 0x6572;
	fake_disk[1301] = 0x0A42;
	fake_disk[1302] = 0x7269;
	fake_disk[1303] = 0x6E67;
	fake_disk[1304] = 0x696E;
	fake_disk[1305] = 0x6720;
	fake_disk[1306] = 0x7468;
	fake_disk[1307] = 0x6520;
	fake_disk[1308] = 0x6368;
	fake_disk[1309] = 0x6563;
	fake_disk[1310] = 0x6B73;
	fake_disk[1311] = 0x2061;
	fake_disk[1312] = 0x6E64;
	fake_disk[1313] = 0x2074;
	fake_disk[1314] = 0x6865;
	fake_disk[1315] = 0x2070;
	fake_disk[1316] = 0x6F73;
	fake_disk[1317] = 0x7461;
	fake_disk[1318] = 0x6C20;
	fake_disk[1319] = 0x6F72;
	fake_disk[1320] = 0x6465;
	fake_disk[1321] = 0x720A;
	fake_disk[1322] = 0x4C65;
	fake_disk[1323] = 0x7474;
	fake_disk[1324] = 0x6572;
	fake_disk[1325] = 0x7320;
	fake_disk[1326] = 0x666F;
	fake_disk[1327] = 0x7220;
	fake_disk[1328] = 0x7468;
	fake_disk[1329] = 0x6520;
	fake_disk[1330] = 0x7269;
	fake_disk[1331] = 0x6368;
	fake_disk[1332] = 0x2C20;
	fake_disk[1333] = 0x6C65;
	fake_disk[1334] = 0x7474;
	fake_disk[1335] = 0x6572;
	fake_disk[1336] = 0x7320;
	fake_disk[1337] = 0x666F;
	fake_disk[1338] = 0x7220;
	fake_disk[1339] = 0x7468;
	fake_disk[1340] = 0x6520;
	fake_disk[1341] = 0x706F;
	fake_disk[1342] = 0x6F72;
	fake_disk[1343] = 0x0A54;
	fake_disk[1344] = 0x6865;
	fake_disk[1345] = 0x2073;
	fake_disk[1346] = 0x686F;
	fake_disk[1347] = 0x7020;
	fake_disk[1348] = 0x6174;
	fake_disk[1349] = 0x2074;
	fake_disk[1350] = 0x6865;
	fake_disk[1351] = 0x2063;
	fake_disk[1352] = 0x6F72;
	fake_disk[1353] = 0x6E65;
	fake_disk[1354] = 0x722C;
	fake_disk[1355] = 0x2074;
	fake_disk[1356] = 0x6865;
	fake_disk[1357] = 0x2067;
	fake_disk[1358] = 0x6972;
	fake_disk[1359] = 0x6C20;
	fake_disk[1360] = 0x6E65;
	fake_disk[1361] = 0x7874;
	fake_disk[1362] = 0x2064;
	fake_disk[1363] = 0x6F6F;
	fake_disk[1364] = 0x722E;
	
}


// TODO Implement me!
void LBA28_read_sector(uint8_t drive, uint32_t LBA, uint32_t sector, uint16_t *addr){
	// STUB
	
	LBA --;
	
	// kprint(tostring_inplace(LBA, 10));
	// kprint(tostring_inplace(sector, 10));
	
	if (drive == 0xA0) {
		
		if(512 * LBA > 5120 || 512 * LBA + 512 * sector > 5120){
			kprint(" Error: LBA address too large for fake disk\n");
		}
		
		uint32_t j = 0;
		for (int i = 256 * LBA; i < 256 * LBA + 256 * sector; i++){
				addr[j] = fake_disk[i];
				j++;
		}
		
	} else {
		kprint("Error: Fake disk is 0xA0, not 0x");
		kprint(tostring_inplace(drive, 16));
		kprint("\n");
	}
	
}


// TODO Implement me!
void LBA28_write_sector(uint8_t drive, uint32_t LBA, uint32_t sector, uint16_t *buffer){
	// STUB
	
	LBA --;
	
	// kprint(tostring_inplace(LBA, 10));
	// kprint(tostring_inplace(sector, 10));
	
	if (drive == 0xA0) {
		
		if(512 * LBA > 5120 || 512 * LBA + 512 * sector > 5120){
			kprint(" Error: LBA address too large for fake disk\n");
		}
		
		uint32_t j = 0;
		for (int i = 256 * LBA; i < 256 * LBA + 256 * sector; i++){
				fake_disk[i] = buffer[j];
				j++;
		}
		
	} else {
		kprint("Error: Fake disk is 0xA0, not 0x");
		kprint(tostring_inplace(drive, 16));
		kprint("\n");
	}
}


raw_disk_info retrieve_disk_info(){
	raw_disk_info* dinfo = (raw_disk_info*)0x5200;
	return *dinfo;
}

void decode_raw_disk_info(raw_disk_info dinfo, disk_info * result){
	result -> drivetype = dinfo.bl;
	result -> sectors = dinfo.cl & 0b00111111;
	result -> cylinders = ((dinfo.cl & 0b11000000) << 2) | dinfo.ch;
    result -> heads = dinfo.dh;
    result -> drives = dinfo.dl;
}
