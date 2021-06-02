#pragma once

#include "../drivers/print_string.c"
#include "../interrupts/floppy.c"

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

void read_boot_sector(){
    unsigned char* sector_in_memory = floppy_read_sector(0);
    struct BOOT_SECTOR* boot_sect = (struct BOOT_SECTOR*)sector_in_memory;
	print_bios_para_block(boot_sect->Bpb);
}