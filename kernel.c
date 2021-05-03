
#define VIDEO_MEMORY (unsigned int*)0xb8000

int _start(){
	char* ptr = (char*)VIDEO_MEMORY;
	*ptr = 'a';
}
