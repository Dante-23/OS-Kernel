/**
 *  Each entry in root directory will be 11 + 2 + 2 + 1 = 16
 * 11 bytes for file name and file extension
 * 2 bytes for number of sectors
 * 2 bytes for starting sector number
 * 1 padding bytes
 * 
 * **/

unsigned short sector_size, root_starting_sector, root_num_of_sectors;
unsigned short fat_starting_sector, fat_num_of_sectors;
unsigned short current_root_dir_entries = 5;

struct RDT_ENTRY{
	unsigned char file_name_ext[12];
	unsigned short num_of_sectors;
	unsigned short starting_sector;
} __attribute__((packed));

struct FAT_ENTRY{
	unsigned short status;    // 0 when free, 1 when used
	unsigned short next_sector; // 0 when no next sector, next_sector - 1 otherwise
} __attribute__((packed));

struct BIOS_PARAMETER_BLOCK {
	unsigned char			OEMName[9];
	unsigned short		    BytesPerSector;
	unsigned char			SectorsPerCluster;
	unsigned short		    ReservedSectors;
	unsigned char			NumberOfFats;
	unsigned short		    NumDirEntries;
	unsigned short		    NumSectors;
	unsigned char			Media;
	unsigned short		    SectorsPerFat;
	unsigned short		    SectorsPerTrack;
	unsigned short		    HeadsPerCyl;
	unsigned int		    HiddenSectors;
	unsigned int		    LongSectors;
} __attribute__((packed));

struct BIOS_PARAMETER_BLOCK_EXT {
	unsigned int			SectorsPerFat32;
	unsigned short			Flags;
	unsigned short			Version;
	unsigned int			RootCluster;
	unsigned short			InfoCluster;
	unsigned short			BackupBoot;
	unsigned short			Reserved[6];
} __attribute__((packed));

struct BOOT_SECTOR {

	unsigned char					Ignore[3];			//first 3 bytes are ignored
	struct BIOS_PARAMETER_BLOCK		Bpb;
	struct BIOS_PARAMETER_BLOCK_EXT	BpbExt;
	unsigned char					Filler[447];		//needed to make struct 512 bytes
} __attribute__((packed));

void print_bios_para_block(struct BIOS_PARAMETER_BLOCK bpb);
void init_512_bytes(char* address);
struct RDT_ENTRY get_rdt_at(unsigned short index);
void set_rdt_at(unsigned short index, struct RDT_ENTRY entry);
struct FAT_ENTRY get_fat_at(unsigned short index);
void set_fat_at(unsigned short index, struct FAT_ENTRY entry);
void print_fat();
void init_fat();
void print_root_directory_table();
void read_boot_sector();
void init_filesystem();