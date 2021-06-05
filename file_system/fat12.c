#pragma once

#include "../drivers/print_string.c"
#include "../interrupts/floppy.c"

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

void print_bios_para_block(struct BIOS_PARAMETER_BLOCK bpb){
	print("OEMName:                ");
	print(bpb.OEMName);
	print("\n");
	print("Bytes Per Sector:       ");
	print(int_to_string(bpb.BytesPerSector));
	print("\n");
	print("Sectors Per Cluster:    ");
	print(int_to_string(bpb.SectorsPerCluster));
	print("\n");
	print("Reserved Sectors:       ");
	print(int_to_string(bpb.ReservedSectors));
	print("\n");
	print("Number of FATS:         ");
	print(int_to_string(bpb.NumberOfFats));
	print("\n");
	print("Num Dir Entries:        ");
	print(int_to_string(bpb.NumDirEntries));
	print("\n");
	print("Number of Sectors:      ");
	print(int_to_string(bpb.NumSectors));
	print("\n");
	print("Media:                  ");
	print(int_to_string(bpb.Media));
	print("\n");
	print("Sectors Per FAT:        ");
	print(int_to_string(bpb.SectorsPerFat));
	print("\n");
	print("Sectors Per Track:      ");
	print(int_to_string(bpb.SectorsPerTrack));
	print("\n");
	print("Heads Per Cylinder:     ");
	print(int_to_string(bpb.HeadsPerCyl));
	print("\n");
	print("Hidden Sectors:         ");
	print(int_to_string(bpb.HiddenSectors));
	print("\n");
	print("Long Sectors:           ");
	print(int_to_string(bpb.LongSectors));
	print("\n");
}

void init_512_bytes(char* address){
	char* ptr = address;
	while(ptr < (address + 512)){
		*ptr = 0;
		ptr++;
	}
}

struct RDT_ENTRY get_rdt_at(unsigned short index){
	unsigned short sector = index / (sector_size / sizeof(struct RDT_ENTRY));
	sector += root_starting_sector;
	unsigned short sector_index = index % (sector_size / sizeof(struct RDT_ENTRY));
	// print("before sector read\n");	
	struct RDT_ENTRY* root = (struct RDT_ENTRY*) floppy_read_sector(sector);
	// print("before sector read\n");
	root += sector_index;
	struct RDT_ENTRY result = *root;
	return result;
}

void set_rdt_at(unsigned short index, struct RDT_ENTRY entry){
	unsigned short sector = index / (sector_size / sizeof(struct RDT_ENTRY));
	sector += root_starting_sector;
	unsigned short sector_index = index % (sector_size / sizeof(struct RDT_ENTRY));	
	struct RDT_ENTRY* root = (struct RDT_ENTRY*) floppy_read_sector(sector);
	root += sector_index;
	*root = entry;
	floppy_write_sector(sector);
}

struct FAT_ENTRY get_fat_at(unsigned short index){
	unsigned short sector = index / (sector_size / sizeof(struct FAT_ENTRY));
	sector += fat_starting_sector;
	unsigned short sector_index = index % (sector_size / sizeof(struct FAT_ENTRY));	
	struct FAT_ENTRY* fat = (struct FAT_ENTRY*) floppy_read_sector(sector);
	fat += sector_index;
	struct FAT_ENTRY result = *fat;
	return result;
}

void set_fat_at(unsigned short index, struct FAT_ENTRY entry){
	unsigned short sector = index / (sector_size / sizeof(struct FAT_ENTRY));
	sector += fat_starting_sector;
	unsigned short sector_index = index % (sector_size / sizeof(struct FAT_ENTRY));	
	struct FAT_ENTRY* fat = (struct FAT_ENTRY*) floppy_read_sector(sector);
	fat += sector_index;
	*fat = entry;
	floppy_write_sector(sector);
}

void print_fat(){
	// unsigned short current = fat_starting_sector;
	// struct FAT_ENTRY* fat = (struct FAT_ENTRY*) floppy_read_sector(current);
	// current++;
	unsigned short index = 40;
	while(index < 50){
		struct FAT_ENTRY result = get_fat_at(index);
		print(int_to_string(index));
		print(" ");
		print(int_to_string(result.status));
		print(" ");
		print(int_to_string(result.next_sector));
		print("\n");
		index++;
	}
}

void init_fat(){
	unsigned short index = 0;
	while(index < current_root_dir_entries){
		struct RDT_ENTRY file = get_rdt_at(index);
		unsigned short current = file.starting_sector;
		while(current < (file.starting_sector + file.num_of_sectors)){
			struct FAT_ENTRY entry;
			entry.status = 1;
			entry.next_sector = (current == ((file.starting_sector + file.num_of_sectors - 1)) ? 0 : current + 1);
			set_fat_at(current, entry);
			current++;
		}
		index++;
	}
}

void init_filesystem(){
    unsigned char* sector_in_memory = floppy_read_sector(0);
    struct BOOT_SECTOR* boot_sect = (struct BOOT_SECTOR*)sector_in_memory;
	sector_size = boot_sect->Bpb.BytesPerSector;
	root_starting_sector = boot_sect->Bpb.ReservedSectors + boot_sect->Bpb.NumberOfFats * boot_sect->Bpb.SectorsPerFat;
	root_num_of_sectors = (boot_sect->Bpb.NumDirEntries * sizeof(struct RDT_ENTRY)) / sector_size; // change

	// FAT initialize
	fat_starting_sector = boot_sect->Bpb.ReservedSectors;
	fat_num_of_sectors = boot_sect->Bpb.SectorsPerFat;
	init_fat();
}

void read_file(char* filename, unsigned char* file_contents){
	for(unsigned short entry_index = 0; entry_index < current_root_dir_entries; entry_index++){
		struct RDT_ENTRY entry = get_rdt_at(entry_index);
		unsigned short len = strlen(filename);
		for(unsigned short index = 0; index <= len; index++){
			if(entry.file_name_ext[index] == ' ')
				entry.file_name_ext[index] = '\0';
		}
		if(strcmp(filename, entry.file_name_ext)){
			unsigned short current = 0;
			for(unsigned short sector = entry.starting_sector; sector < entry.starting_sector + entry.num_of_sectors; sector++){
				unsigned char* data = floppy_read_sector(sector);
				memory_copy((unsigned char*)data, (unsigned char*)(file_contents + current), sector_size);
				current += sector_size;
			}
			print("\n");
			print("found file\n");
			return ;
		}
		// break;
	}
	print("file not found\n");
}

void write_file(char* filename, unsigned char* file_contents){

}

void create_file(char* filename){
	
}

void print_root_directory_table(){
	unsigned short current = 0;
	// print("Name        ");
	// print("size        ");
	// print("start       ");
	// print("here\n");
	while(current < current_root_dir_entries){
		// print(int_to_string(current));
		// print("\n");
		struct RDT_ENTRY result = get_rdt_at(current);
		print(result.file_name_ext);
		print(" ");
		print(int_to_string(result.num_of_sectors));
		print(" ");
		print(int_to_string(result.starting_sector));
		print("\n");
		current++;
	}
}

void read_boot_sector(){
    unsigned char* sector_in_memory = floppy_read_sector(0);
    struct BOOT_SECTOR* boot_sect = (struct BOOT_SECTOR*)sector_in_memory;
	print_bios_para_block(boot_sect->Bpb);
}