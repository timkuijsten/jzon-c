#include "../jzon.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h> 

typedef struct LoadedFile
{
	size_t size;
	char* data;
} LoadedFile;

LoadedFile load_file(const char *filename)
{ 
	FILE* fp;
	size_t filesize;
	unsigned char* data;
	fp = fopen(filename, "rb");

	assert(fp);

	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	data = (unsigned char*)malloc(filesize);
	assert(data);

	fread(data, 1, filesize, fp);
	data[filesize] = 0;
	fclose(fp);

	LoadedFile lf;
	lf.data = (char*)data;
	lf.size = filesize;
	return lf;
}

void pretty_print(int depth, JzonValue* value)
{
	char prefix[100];
	char new_prefix[100];

	memset(prefix, ' ', depth * 2);
	prefix[depth * 2] = '\0';
	memset(new_prefix, ' ', (depth + 1) * 2);
	new_prefix[(depth + 1) * 2] = '\0';

	if (value->is_object)
	{
		printf("\n");
		printf(prefix);
		printf("{");
		printf("\n");

		for (unsigned i = 0; i < jzon_size(value); ++i)
		{
			printf(new_prefix);
			printf(jzon_key(value, i));
			pretty_print(depth + 1, jzon_get(value, i));
			printf("\n");
		}

		printf(prefix);
		printf("}");
	}
	else if (value->is_array)
	{
		printf("\n");
		printf(prefix);
		printf("[");
		printf("\n");

		for (unsigned i = 0; i < jzon_size(value); ++i)
		{
			printf(new_prefix);
			pretty_print(depth + 1, jzon_get(value, i));
			printf("\n");
		}
		
		printf(prefix);
		printf("]");
	}
	else if (value->is_bool)
	{
		printf(prefix);

		if (jzon_bool(value))
			printf("true");
		else
			printf("false");
	}
	else if (value->is_float)
	{
		printf(prefix);
		printf("%f", jzon_float(value));
	}
	else if (value->is_int)
	{
		printf(prefix);
		printf("%i", jzon_int(value));
	}
	else if (value->is_null)
	{
		printf(prefix);
		printf("null");
	}
	else if (value->is_string)
	{
		printf(prefix);
		printf(jzon_string(value));
	}
}

int main()
{
	LoadedFile file = load_file("test.jzon");
	struct timeb start, end;
	ftime(&start);
	JzonValue* result = jzon_parse(file.data);
	ftime(&end);
	unsigned elapsed = end.time - start.time;
	assert(result != NULL);
	//pretty_print(0, result);
	free(result);
	//(void)trailing_value;
	//jzon_free(result.output);
	getchar();
	return 0;
}
