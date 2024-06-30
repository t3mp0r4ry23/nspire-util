#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <nspire.h>

nspire_handle_t *handle;
struct nspire_devinfo *device_info;
int ret = 0;
int cmd = -1;
const char * cmds[] = {"storage", "ram", "power", "lcd", "send", "get", "rm", "touch", "ls", "mkdir"};

int main(int argc, char * argv[]) {
	device_info = malloc(sizeof(struct nspire_devinfo));
	
	printf("Connecting...\n");
	
	if ((ret = nspire_init(&handle))) {
		fprintf(stderr, "ERROR!: %s\n", nspire_strerror(ret));
		return ret;
	}
	nspire_device_info(handle, device_info);
	
	printf("Connected to %s with ID %s!\n", device_info->device_name, device_info->electronic_id);

	if (argc > 1) {
		for (int i = 0; i < 10; i++) {
			if (!strcmp(argv[1], cmds[i])) {
				cmd = i;
				break;
			}
		}
	}
	else {
		cmd=99;
	}

	if (cmd > 3 && cmd < 8) {
		struct nspire_dir_item *file_info;
		file_info = malloc(sizeof(struct nspire_dir_item));
		if (cmd < 6) {
			FILE * nspire_file;
			char * nspire_file_buffer;
		}
	}

	switch(cmd) {
		case -1:
			printf("Not a command!\n");
			ret = 22;
			break;
		case 0:
			printf("%.2lf MiB of internal storage used out of %.2lf MiB, leaving %.2lf MiB free.\n",
				(float) (device_info->storage.total - device_info->storage.free)/(1024*1024),
				(float) device_info->storage.total/(1024*1024),
				(float) device_info->storage.free/(1024*1024));
			break;
		case 1:
			printf("%.2lf MiB of RAM used out of %.2lf MiB, leaving %.2lf MiB free.\n",
				(float) (device_info->ram.total - info->ram.free)/(1024*1024),
				(float) device_info->ram.total/(1024*1024),
				(float) device_info->ram.free/(1024*1024));
			break;
		case 2:
			printf("Battery status is %#02x and is%scharging\n", device_info->batt.status, device_info->batt.is_charging ? " " : "not");
			break;
		case 3:
			printf("Resolution is %dx%d.\n", device_info->lcd.width, device_info->lcd.height);
			printf("BBP is %d.\n", device_info->lcd.bbp);
			printf("Sample mode is %d.\n", device_info->lcd.sample_mode);
			break;
		case 4:
			if (argc < 4) {
				fprintf(stderr, "Not enough arguments!\n");
				ret = 22;
				break;
			}

			nspire_file = fopen(argv[2], "rb");

			if (nspire_file == NULL) {
				fprintf(stderr, "Couldn't read file %s!", argv[2]);
				ret = 2;
				break;
			}

			fseek(nspire_file, 0, SEEK_END);
			long nspire_file_size = ftell(send_file);
			fseek(nspire_file, 0, SEEK_SET);

			printf("Reading data from host from file at %s...\n", argv[2]);
			char * nspire_file_buffer = malloc(send_file_size + 1);
			fread(nspire_file_buffer, send_file_size, 1, send_file);
			fclose(nspire_file);

			printf("Sending %.3f %s of data to calculator to save to file at %s...\n", nspire_file_size > 999999 ? ((float) send_file_size/(1024*1024)) : ((float) send_file_size/1024), send_file_size > 999999 ? "MiB" : "KiB", argv[3]);
			ret = nspire_file_write(handle, argv[3], nspire_file_buffer, send_file_size);
			if (ret) {
				fprintf(stderr, "Error while writing: %s\n", nspire_strerror(ret));
				break;
			}
			printf("Sent!\n");
			break;
		case 5:
			if (argc < 4) {
				fprintf(stderr, "Not enough arguments!\n");
				ret = 22;
				break;
			}

			printf("Getting file info...\n");
			ret = nspire_attr(handle, argv[2], file_info);
			if (ret) {
				fprintf(stderr, "Error while getting file info: %s\n", nspire_strerror(ret));
				break;
			}
			if (file_info->type) {
				fprintf(stderr, "%s is a directory!\n", argv[2]);
				ret = 2;
				break;
			}

			printf("Initiating transfer...\n");
			nspire_file = fopen(argv[3], "wb");
			nspire_file_buffer = malloc(file_info->size + 1);

			printf("Reading %.3f %s of data from %s...\n",
				file_info->size > 999999 ? ((float) file_info->size/(1024*1024)) :
					file_info->size > 999 ? ((float) file_info->size/1024) :
						(float) file_info->size),
				file_info->size > 999999 ? "MiB" :
					file_info->size > 999 ? "KiB" :
						"bytes",
				argv[2]);
			ret = nspire_file_read(handle, argv[2], nspire_file_buffer, file_info->size, &file_info->size);
			if (ret) {
				fprintf(stderr, "Error while reading file: %s\n", nspire_strerror(ret));
				fclose(nspire_file);
				break;
			}

			printf("Saving to %s...\n", argv[3]);
			fwrite(nspire_file_buffer, file_info->size, 1, get_file);
			fclose(nspire_file);
			printf("Saved!\n");

			break;
		case 6:
			if (argc < 3) {
				fprintf(stderr, "Not enough arguments!\n");
				ret = 22;
				break;

			}

			ret = nspire_attr(handle, argv[2], file_info);
			if (ret) {
				fprintf(stderr, "Error while getting file info: %s\n", nspire_strerror(ret));
				break;
			}

			if (file_info->type) {
				printf("Deleting directory at %s...\n", argv[2]);
				ret = nspire_dir_delete(handle, argv[2]);
			}
			else {
				printf("Deleting file at %s...\n", argv[2]);
				ret = nspire_file_delete(handle, argv[2]);
			}
			if (ret) {
				fprintf(stderr, "Error while deleting directory: %s\n", nspire_strerror(ret));
				break;
			}

			printf("Deleted!\n");
			break;
		case 7:
			if (argc < 3) {
				fprintf(stderr, "Not enough arguments!\n");
				ret = 22;
				break;
			}

			printf("Creating file at %s...\n", argv[2]);
			ret = nspire_file_touch(handle, argv[2]);
			if (ret) {
				fprintf(stderr, "Error while creating file: %s\n", nspire_strerror(ret));
				break;
			}

			printf("Created!\n");
			break;
		case 8:
			if (argc < 3) {
				fprintf(stderr, "Not enough arguments!\n");
				ret = 22;
				break;
			}

			struct nspire_dir_info *dir_info;
			ret = nspire_dirlist(handle, argv[2], &dir_info);
			if (ret) {
				fprintf(stderr, "Error while getting directory info: %s\n", nspire_strerror(ret));
				break;
			}
			printf("Contents of directory '%s':\n", argv[2]);

			for (int item = 0; item < dir_info->num; item++) {
				printf("%.2f %s %s",
					dir_info->items[item].size > 999999 ? ((float) dir_info->items[item].size/(1024*1024)) :
						(dir_info->items[item].size > 999 ? ((float) dir_info->items[item].size/1024) :
							((float) dir_info->items[item].size)),
					dir_info->items[item].size > 999999 ? "MiB" :
						(dir_info->items[item].size > 999 ? "KiB" :
							"bytes"),
					dir_info->items[item].name);
				
				if (dir_info->items[item].type == 1) {
					printf("/");
				}
				printf("\n");
			}

			nspire_dirlist_free(dir_info);
			break;
		case 9:
			if (argc < 3) {
				fprintf(stderr, "Not enough arguments!\n");
				ret = 22;
				break;
			}

			printf("Creating directory at %s...\n", argv[2]);
			ret = nspire_dir_create(handle, argv[2]);
			if (ret) {
				fprintf(stderr, "Error while creating directory: %s\n", nspire_strerror(ret));
				break;
			}

			printf("Created!\n");
			break;
		default:
			printf("Nothing to do.\n");
	}

	printf("Disconnecting...\n");
	nspire_free(handle);
	if (!ret) {
		printf("Done!\n");
	}
	else {
		printf("Exited with error.\n");
	}
	return ret;
}
